#include "api/snes_devkit.h"
// FUTURE: dialog anywhere on screen requires clipping lines have full screen range (0-248)
// but they currently have a bit more than half-screen (0-127), so we will force the dialog
// to be at the bottom for now

typedef struct Dialog
{
    Uint8 pageLeft; // (0 - 31)
    Uint8 pageTop; // (0 - 31)
    Uint8 numLinesPerPage;

    // Copied, massaged
    char* text;
    // one per line break
    Uint16 numLinesInText;
    // index of first character of line
    Uint16* lineStartIndices;
    // index of last character of line, starts 1 before its lineStart,
    // advances until it is after its lineStart and 1 before a line end
    Uint16* lineEndIndices;

    Uint16 firstVisibleLine;
    // 0..7 (0..15 for double-high text); when it exceeds range,
    // topLine ticks up and scrollOffset resets
    Uint8 textScrollOffset;
    SDL_bool tallText;

    hWB portraitWB;
} Dialog;

void ingestText(hDialog result, char* text, Uint8 lineWidth)
{
    result->firstVisibleLine = 0;
    result->textScrollOffset = 0;
    result->text = (char*)SDL_malloc(SDL_strlen(text) + 1);
    SDL_memcpy(result->text, text, SDL_strlen(text) + 1);

    result->numLinesInText = 1;
    Uint16 head = 0;
    Uint8 lineLength = 1;

    while (result->text[head] != 0)
    {
        if (result->text[head] == '\n')
        {
            lineLength = 0;
            result->numLinesInText++;
        }
        if (lineLength > lineWidth)
        {
            while (result->text[head] != ' ')
            {
                head--;
            }
            result->text[head] = '\n';
            lineLength = 0;
            result->numLinesInText++;
        }
        head += 1;
        lineLength += 1;
    }
    result->text[head] = '\n';

    result->lineStartIndices = SDL_malloc(sizeof(Uint16) * result->numLinesInText);
    result->lineEndIndices = SDL_malloc(sizeof(Uint16) * result->numLinesInText);

    Uint16 line = 0;
    for (head = 0; line < result->numLinesInText; head++)
    {
        if (head == 0 || result->text[head - 1] == '\n')
        {
            result->lineStartIndices[line] = head;
            result->lineEndIndices[line] = head;
            line++;
        }
    }
}

hDialog creatBare_Dialog(char* text, Uint8 lineCount, SDL_bool tallText)
{
    hDialog result = (hDialog)SDL_malloc(sizeof(Dialog));
    result->pageLeft = 1;
    result->pageTop = 31 - ((result->tallText == SDL_TRUE ? 2 : 1) * lineCount) - 1;
    result->numLinesPerPage = lineCount;
    result->tallText = tallText;
    result->portraitWB = NULL;
    ingestText(result, text, 29);
    return result;
}

hDialog creatPortrait_Dialog(char* text, hWB portraitWB, SDL_bool portraitOnRight)
{
    SDL_assert(getHeight_WB(portraitWB) < 14);
    SDL_assert(getWidth_WB(portraitWB) < 12);
    Uint8 portraitWidth = (Uint8)getWidth_WB(portraitWB);
    Uint8 portraitHeight = (Uint8)getHeight_WB(portraitWB);
    hDialog result = (hDialog)SDL_malloc(sizeof(Dialog));
    if (portraitOnRight)
    {
        result->pageLeft = 1;
    }
    else
    {
        result->pageLeft = 2 + portraitWidth;
    }
    result->pageTop = 30 - portraitHeight;
    result->numLinesPerPage = portraitHeight;
    result->tallText = SDL_FALSE;
    result->portraitWB = portraitWB;
    ingestText(result, text, 28 - portraitWidth);
    return result;
}

SDL_bool isFinished_Dialog(hDialog dialog)
{
    return dialog->text[dialog->lineEndIndices[dialog->numLinesInText - 1]] == '\n';
}

SDL_bool pageDone_Dialog(hDialog dialog)
{
    for (int i = 0; i < dialog->numLinesPerPage; i++)
    {
        if (dialog->text[dialog->lineEndIndices[i + dialog->firstVisibleLine]] != '\n')
        {
            return SDL_FALSE;
        }
    }

    if (dialog->firstVisibleLine % dialog->numLinesPerPage == 0 && dialog->textScrollOffset == 0
        || dialog->firstVisibleLine + dialog->numLinesPerPage == dialog->numLinesInText)
    {
        return SDL_TRUE;
    }
    else
    {
        return SDL_FALSE;
    }
}

void update_Dialog(hDialog dialog)
{
    if (pageDone_Dialog(dialog) == SDL_TRUE)
    {
        return;
    }

    if (dialog->textScrollOffset > 0)
    {
        dialog->textScrollOffset += 1;
        if (dialog->textScrollOffset == (dialog->tallText == SDL_TRUE ? 16 : 8))
        {
            dialog->textScrollOffset = 0;
            dialog->firstVisibleLine += 1;
        }
    }
    else
    {
        for (int i = 0; i < dialog->numLinesPerPage; i++)
        {
            if (dialog->text[dialog->lineEndIndices[i + dialog->firstVisibleLine]] != '\n')
            {
                dialog->lineEndIndices[i + dialog->firstVisibleLine] += 1;
                return;
            }
        }
        if (dialog->firstVisibleLine + dialog->numLinesPerPage < dialog->numLinesInText)
        {
            dialog->textScrollOffset = 1;
        }
    }
}

void destr_Dialog(hDialog dialog)
{
    if (dialog->portraitWB != NULL)
    {
        destr_WB(dialog->portraitWB);
    }
    SDL_free(dialog->text);
    SDL_free(dialog->lineStartIndices);
    SDL_free(dialog->lineEndIndices);
    SDL_free(dialog);
}

void poke_Dialog(hDialog dialog)
{
    if (pageDone_Dialog(dialog))
    {
        if (dialog->firstVisibleLine + dialog->numLinesPerPage < dialog->numLinesInText)
        {
            dialog->textScrollOffset = 1;
        }
    }
    else
    {
        while (pageDone_Dialog(dialog) == SDL_FALSE)
        {
            update_Dialog(dialog);
        }
    }
}

void renderText_Dialog(hDialog dialog, hRENDER render, Uint8 layerIndex,
    Uint8 bankIndex, Uint8 paletteIndex)
{
    showBackdrop(render, layerIndex, 0, dialog->textScrollOffset % 8, SDL_TRUE);
    setFullLayerClips(render, layerIndex,
        0, 0, 0, 8 * ((dialog->tallText == SDL_TRUE ? 2 : 1) * dialog->numLinesPerPage + 1) - 1,
        SDL_FALSE, SDL_FALSE, SDL_FALSE, SDL_TRUE
    );
    fillBackdrop(render, layerIndex, 0, bankIndex, paletteIndex, SDL_FALSE, SDL_FALSE, SDL_FALSE);

    for (Uint16 line = dialog->firstVisibleLine; line < dialog->numLinesInText; line++)
    {
        Uint8 destY = dialog->pageTop + (dialog->tallText ? 2 : 1)
            * (Uint8)(line - dialog->firstVisibleLine) - dialog->textScrollOffset / 8;
        Uint8 destX = dialog->pageLeft;

        if (destY > 31) break;

        for (int c = dialog->lineStartIndices[line]; c < dialog->lineEndIndices[line]; c++)
        {
            setBackdropStroke(render, layerIndex, destX, destY, dialog->text[c],
                bankIndex, paletteIndex, SDL_FALSE, SDL_FALSE, SDL_FALSE);
            if (dialog->tallText)
            {
                setBackdropStroke(render, layerIndex, destX, destY + 1, dialog->text[c] + 128,
                    bankIndex, paletteIndex, SDL_FALSE, SDL_FALSE, SDL_FALSE);
            }
            destX++;
        }
    }
}

void renderBorder_Dialog(hDialog dialog, hRENDER render, Uint8 layerIndex,
    Uint8 bankIndex, Uint8 paletteIndex)
{
    showBackdrop(render, layerIndex, 0, 0, SDL_TRUE);
    setLayerClips(render, layerIndex, 0, 0, 0, 0);
    fillBackdrop(render, layerIndex, 0,
        bankIndex, paletteIndex, SDL_FALSE, SDL_FALSE, SDL_TRUE);

    setBackdropStroke(render, layerIndex, 0, 29 - dialog->numLinesPerPage, 1,
        bankIndex, paletteIndex, SDL_FALSE, SDL_FALSE, SDL_FALSE);
    setBackdropStroke(render, layerIndex, 30, 29 - dialog->numLinesPerPage, 3,
        bankIndex, paletteIndex, SDL_FALSE, SDL_FALSE, SDL_FALSE);
    setBackdropStroke(render, layerIndex, 0, 30, 6,
        bankIndex, paletteIndex, SDL_FALSE, SDL_FALSE, SDL_FALSE);
    setBackdropStroke(render, layerIndex, 30, 30, 8,
        bankIndex, paletteIndex, SDL_FALSE, SDL_FALSE, SDL_FALSE);

    for (Uint8 x = 1; x < 30; x++)
    {
        setBackdropStroke(render, layerIndex, x, 29 - dialog->numLinesPerPage, 2,
            bankIndex, paletteIndex, SDL_FALSE, SDL_FALSE, SDL_FALSE);
        setBackdropStroke(render, layerIndex, x, 30, 7,
            bankIndex, paletteIndex, SDL_FALSE, SDL_FALSE, SDL_FALSE);
    }

    for (Uint8 y = 30 - dialog->numLinesPerPage; y < 30; y++)
    {
        setBackdropStroke(render, layerIndex, 0, y, 4,
            bankIndex, paletteIndex, SDL_FALSE, SDL_FALSE, SDL_FALSE);
        setBackdropStroke(render, layerIndex, 30, y, 5,
            bankIndex, paletteIndex, SDL_FALSE, SDL_FALSE, SDL_FALSE);
    }

    if (pageDone_Dialog(dialog) && !isFinished_Dialog(dialog))
    {
        setBackdropStroke(render, layerIndex, 29, 30, 9,
            bankIndex, paletteIndex, SDL_FALSE, SDL_FALSE, SDL_FALSE);
    }

    if (dialog->portraitWB != NULL)
    {
        Uint8 portraitWidth = (Uint8)getWidth_WB(dialog->portraitWB);
        Uint8 dividerX;
        if (dialog->pageLeft == 1)
        {
            dividerX = 29 - portraitWidth;
            appendToBackdrop_WB(render, layerIndex, dialog->portraitWB, 30 - portraitWidth, 25,
                bankIndex, paletteIndex, SDL_FALSE);
        }
        else
        {
            dividerX = 1 + portraitWidth;
            appendToBackdrop_WB(render, layerIndex, dialog->portraitWB, 1, 25,
                bankIndex, paletteIndex, SDL_FALSE);
        }

        setBackdropStroke(render, layerIndex, dividerX, 29 - dialog->numLinesPerPage, 10,
            bankIndex, paletteIndex, SDL_FALSE, SDL_FALSE, SDL_FALSE);
        setBackdropStroke(render, layerIndex, dividerX, 30, 12,
            bankIndex, paletteIndex, SDL_FALSE, SDL_FALSE, SDL_FALSE);
        for (Uint8 y = 30 - dialog->numLinesPerPage; y < 30; y++)
        {
            setBackdropStroke(render, layerIndex, dividerX, y, 11,
                bankIndex, paletteIndex, SDL_FALSE, SDL_FALSE, SDL_FALSE);
        }
    }
}

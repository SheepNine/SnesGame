#pragma unmanaged

#include "sc.h"

#pragma managed

using namespace System;

namespace SnesGame
{
    namespace CLR
    {
        public ref class Mixer
        {
        private:
            static void WriteNoteToChannel(Note^ note, hSC sc)
            {

                SilentNote^ silentNote = dynamic_cast<SilentNote^>(note);
                if (silentNote != nullptr)
                {
                    silence_SC(sc);
                }

                SquareNote^ squareNote = dynamic_cast<SquareNote^>(note);
                if (squareNote != nullptr)
                {
                    playNote_SC(sc, squareNote->MaxLength,
                        squareNote->VolumeMin,
                        squareNote->VolumeMax,
                        (ShiftDir)squareNote->VolumeShiftParameters->Direction,
                        squareNote->VolumeShiftParameters->Speed,
                        (::EdgeBehaviour)squareNote->VolumeShiftParameters->EdgeBehaviour,
                        squareNote->PeriodMin,
                        squareNote->PeriodMax,
                        (ShiftDir)squareNote->PeriodShiftParameters->Direction,
                        squareNote->PeriodShiftParameters->Speed,
                        (::EdgeBehaviour)squareNote->PeriodShiftParameters->EdgeBehaviour);
                }

                Noise^ noise = dynamic_cast<Noise^>(note);
                if (noise != nullptr)
                {
                    playNoise_SC(sc, noise->InitialRegister, noise->TapMask,
                        noise->MaxLength, noise->Speed);
                }
            }

        public:
            static array<Int16>^ SampleAudio(Note^ note, Sint32 length)
            {
                array<Int16>^ result = gcnew array<Int16>(length);
                hSC sc = creat_SC();
                WriteNoteToChannel(note, sc);
                for (Sint32 i = 0; i < result->Length; i++)
                {
                    result[i] = getNextSample_SC(sc);
                }
                destr_SC(sc);
                return result;
            }

            static array<Int16>^ SampleAudio(array<Note^>^ notes)
            {
                int numNotes = notes->GetLength(0);
                array<Int16>^ result = gcnew array<Int16>(numNotes * 48000 / 50);

                hSC sc = creat_SC();
                Sint32 writeIndex = 0;
                for (Sint32 n = 0; n < numNotes; n++)
                {
                    Note^ note = notes[n];
                    if (note != nullptr)
                    {
                        WriteNoteToChannel(note, sc);
                    }
                    for (Sint32 j = 0; j < 48000 / 50; j++)
                    {
                        result[writeIndex++] = getNextSample_SC(sc);
                    }
                }
                destr_SC(sc);

                return result;
            }

            static array<Int16>^ SampleAudio(array<Note^, 2>^ notes)
            {
                int numTracks = notes->GetLength(0);
                int numNotes = notes->GetLength(1);
                array<Int16>^ result = gcnew array<Int16>(numNotes * 48000 / 50);

                for (Sint32 t = 0; t < numTracks; t++)
                {
                    hSC sc = creat_SC();
                    Sint32 writeIndex = 0;
                    for (Sint32 n = 0; n < numNotes; n++)
                    {
                        Note^ note = notes[t, n];
                        if (note != nullptr)
                        {
                            WriteNoteToChannel(note, sc);
                        }
                        for (Sint32 j = 0; j < 48000 / 50; j++)
                        {
                            if (t == 0)
                            {
                                result[writeIndex] = 0;
                            }
                            result[writeIndex++] += getNextSample_SC(sc);
                        }
                    }
                    destr_SC(sc);
                }

                return result;
            }

            static array<Int16>^ SampleAudio(ReadOnlyRecording^ recording,
                int startIndex, int endIndex)
            {
                if (startIndex > endIndex)
                    throw gcnew ArgumentException("parameters out of order");

                int playCount = endIndex - startIndex + 1;
                array<Note^, 2>^ notes = gcnew array<Note^, 2>(recording->NumTracks, playCount);

                for (int n = 0; n < playCount; n++)
                    for (int t = 0; t < recording->NumTracks; t++)
                        notes[t, n] = recording[t, n + startIndex];

                return SampleAudio(notes);
            }
        };

    }
}
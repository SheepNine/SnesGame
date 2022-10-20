using System.IO;
using System.Linq;

namespace SnesGame.ResourceCompiler.Pipelines.Palette
{
    public static class PaletteHeaderFile
    {
        public static void WritePaletteHeaderFile(this Manifest manifest)
        {
            PipelineUtils.WriteFileIfStale(
                manifest.GetPalettes().Select(wb => wb.SourcePath),
                Path.Combine(manifest.OutputDirectory, "palettes.h"),
                writer =>
                {
                    writer.WriteLine("#pragma once");
                    writer.WriteLine("#include \"snes_runtime.h\"");
                    writer.WriteLine();

                    int paletteIndex = 0;
                    foreach (var palette in manifest.GetPalettes())
                    {
                        writer.WriteLine("#define PAL_{0} {1}", palette.ID, paletteIndex++);
                    }

                    writer.WriteLine();
                    writer.WriteLine("void setBackdropPaletteFromAsset(hRENDER render, Uint8 paletteIndex, Uint16 paletteId);");
                    writer.WriteLine("void setActorPaletteFromAsset(hRENDER render, Uint8 paletteIndex, Uint16 paletteId);");
                    writer.WriteLine("void initPaletteFromAsset(Uint8* data, Uint16 paletteId);");
                });
        }
    }
}

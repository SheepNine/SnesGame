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
                writer => WritePaletteHeaderFile(manifest, writer));
        }

        static void WritePaletteHeaderFile(Manifest manifest, TextWriter w)
        {
            w.WriteLine("#pragma once");
            w.WriteLine("#include \"snes_runtime.h\"");
            w.WriteLine();

            int paletteIndex = 0;
            foreach (var palette in manifest.GetPalettes())
            {
                w.WriteLine("#define PAL_{0} {1}", palette.ID, paletteIndex++);
            }

            w.WriteLine();
            w.Write("void setBackdropPaletteFromAsset(hRENDER render, Uint8 paletteIndex,");
            w.WriteLine(" Uint16 paletteId);");
            w.Write("void setActorPaletteFromAsset(hRENDER render, Uint8 paletteIndex,");
            w.WriteLine(" Uint16 paletteId);");
            w.WriteLine("void initPaletteFromAsset(Uint8* data, Uint16 paletteId);");
        }
    }
}

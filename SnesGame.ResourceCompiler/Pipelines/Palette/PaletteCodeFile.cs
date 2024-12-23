using System.IO;
using System.Linq;

namespace SnesGame.ResourceCompiler.Pipelines.Palette
{
    public static class PaletteCodeFile
    {
        public static void WritePaletteCodeFile(this Manifest manifest)
        {
            PipelineUtils.WriteFileIfStale(
                manifest.GetPalettes().Select(wb => wb.SourcePath),
                Path.Combine(manifest.OutputDirectory, "palettes.c"),
                writer => WritePaletteCodeFile(manifest, writer)
            );
        }

        static void WritePaletteCodeFile(Manifest manifest, TextWriter w)
        {
            w.WriteLine("#include \"palettes.h\"");
            w.WriteLine();

            w.WriteLine("Uint8 PALETTES[{0}] = {{", manifest.GetPaletteCount() * 32);
            foreach (var palette in manifest.GetPalettes())
            {
                w.Write("\t");
                foreach (var color in palette.LoadPaletteColors())
                {
                    w.Write("0x{0:X2},",
                        (color.B & 0xF8) | ((color.G >> 3) & 0x07));
                    w.Write("0x{0:X2},",
                        (color.R & 0xF8) | (color.G >> 6) | (color.A < 0x80 ? 0x04 : 0x00));
                }
                w.WriteLine();
            }
            w.WriteLine("};");
            w.WriteLine();

            w.Write("void setBackdropPaletteFromAsset(hRENDER render, Uint8 paletteIndex,");
            w.WriteLine(" Uint16 paletteId) {");
            w.WriteLine("\tsetBackdropPalette(render, paletteIndex, PALETTES + 32 * paletteId);");
            w.WriteLine("}");
            w.WriteLine("");
            w.Write("void setActorPaletteFromAsset(hRENDER render, Uint8 paletteIndex,");
            w.WriteLine(" Uint16 paletteId) {");
            w.WriteLine("\tsetActorPalette(render, paletteIndex, PALETTES + 32 * paletteId);");
            w.WriteLine("}");
            w.WriteLine("");
            w.WriteLine("void initPaletteFromAsset(Uint8* data, Uint16 paletteId) {");
            w.WriteLine("\tSDL_memcpy(data, PALETTES + 32 * paletteId, 32);");
            w.WriteLine("}");
        }
    }
}

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
                writer =>
                {
                    writer.WriteLine("#include \"palettes.h\"");
                    writer.WriteLine();

                    writer.WriteLine("Uint8 PALETTES[{0}] = {{", manifest.GetPaletteCount() * 32);
                    foreach (var palette in manifest.GetPalettes())
                    {
                        writer.Write("\t");
                        foreach (var color in palette.LoadPaletteColors())
                        {
                            writer.Write("0x{0:X2},", (color.B & 0xF8) | ((color.G >> 3) & 0x07));
                            writer.Write("0x{0:X2},", (color.R & 0xF8) | (color.G >> 6) | (color.A < 0x80 ? 0x04 : 0x00));
                        }
                        writer.WriteLine();
                    }
                    writer.WriteLine("};");
                    writer.WriteLine();

                    writer.WriteLine("void setBackdropPaletteFromAsset(hRENDER render, Uint8 paletteIndex, Uint16 paletteId) {");
                    writer.WriteLine("\tsetBackdropPalette(render, paletteIndex, PALETTES + 32 * paletteId);");
                    writer.WriteLine("}");
                    writer.WriteLine("");
                    writer.WriteLine("void setActorPaletteFromAsset(hRENDER render, Uint8 paletteIndex, Uint16 paletteId) {");
                    writer.WriteLine("\tsetActorPalette(render, paletteIndex, PALETTES + 32 * paletteId);");
                    writer.WriteLine("}");
                    writer.WriteLine("");
                    writer.WriteLine("void initPaletteFromAsset(Uint8* data, Uint16 paletteId) {");
                    writer.WriteLine("\tSDL_memcpy(data, PALETTES + 32 * paletteId, 32);");
                    writer.WriteLine("}");

                });
        }
    }
}

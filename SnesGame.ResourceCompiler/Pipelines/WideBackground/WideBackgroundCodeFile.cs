using System.IO;
using System.Linq;

namespace SnesGame.ResourceCompiler.Pipelines.WideBackground
{
    public static class WideBackgroundCodeFile
    {
        public static void WriteWideBackgroundCodeFile(this Manifest manifest)
        {
            PipelineUtils.WriteFileIfStale(
                manifest.GetWideBackgrounds().Select(wb => wb.SourcePath),
                Path.Combine(manifest.OutputDirectory, "wide_backgrounds.c"),
                writer =>
                {
                    writer.WriteLine("#include \"wide_backgrounds.h\"");

                    writer.WriteLine();
                    writer.WriteLine("hWB loadWB(Uint16 wbID) {");
                    writer.WriteLine("\tswitch (wbID) {");

                    foreach (var wideBackground in manifest.GetWideBackgrounds())
                    {
                        var arrangement = wideBackground.LoadSourceTileArrangement();

                        foreach (var layerId in Enumerable.Range(0, wideBackground.LayerCount))
                        {
                            var min = 256 * layerId;
                            var max = min + 255;

                            writer.WriteLine("\t\tcase WB_{0}_{1}: {{", wideBackground.ID, layerId);

                            writer.WriteLine("\t\t\tUint8 data[{0}] = {{", arrangement.TileCountX * arrangement.TileCountY);
                            foreach (var y in Enumerable.Range(0, arrangement.TileCountY))
                            {
                                writer.Write("\t\t\t\t");
                                foreach (var x in Enumerable.Range(0, arrangement.TileCountX))
                                {
                                    var bite = arrangement[x, y];
                                    writer.Write("0x{0:x2}, ", bite < min || bite > max ? 0 : bite - min);
                                }
                                writer.WriteLine();
                            }
                            writer.WriteLine("\t\t\t};");
                            writer.WriteLine("\t\t\treturn creat_WB({0}, {1}, data);", arrangement.TileCountX, arrangement.TileCountY);
                            writer.WriteLine("\t\t}");
                        }
                    }

                    writer.WriteLine("\t\tdefault: return NULL;");
                    writer.WriteLine("\t}");
                    writer.WriteLine("}");
                });
        }
    }
}

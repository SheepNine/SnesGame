using System.IO;
using System.Linq;
using UnaryHeap.Mosaic;

namespace SnesGame.ResourceCompiler.Pipelines.WideBackground
{
    public static class WideBackgroundCodeFile
    {
        public static void WriteWideBackgroundCodeFile(this Manifest manifest)
        {
            PipelineUtils.WriteFileIfStale(
                manifest.GetWideBackgrounds().Select(wb => wb.SourcePath),
                Path.Combine(manifest.OutputDirectory, "wide_backgrounds.c"),
                writer => WriteWideBackgroundCodeFile(manifest, writer));
        }

        static void WriteWideBackgroundCodeFile(Manifest manifest, TextWriter w)
        {
            w.WriteLine("#include \"wide_backgrounds.h\"");

            w.WriteLine();
            w.WriteLine("hWB loadWB(Uint16 wbID) {");
            w.WriteLine("\tswitch (wbID) {");

            foreach (var wideBackground in manifest.GetWideBackgrounds())
            {
                var arrangement = wideBackground.LoadSourceTileArrangement();

                foreach (var layerId in Enumerable.Range(0, wideBackground.LayerCount))
                {
                    WriteWideBackgroundLayerCase(w, wideBackground, arrangement, layerId);
                }
            }

            w.WriteLine("\t\tdefault: return NULL;");
            w.WriteLine("\t}");
            w.WriteLine("}");
        }

        private static void WriteWideBackgroundLayerCase(TextWriter w,
            WideBackgroundEntry wideBackground, TileArrangement arrangement, int layerId)
        {
            var min = 256 * layerId;
            var max = min + 255;

            w.WriteLine("\t\tcase WB_{0}_{1}: {{", wideBackground.ID, layerId);
            w.WriteLine("\t\t\tUint8 data[{0}] = {{",
                arrangement.TileCountX * arrangement.TileCountY);
            foreach (var y in Enumerable.Range(0, arrangement.TileCountY))
            {
                w.Write("\t\t\t\t");
                foreach (var x in Enumerable.Range(0, arrangement.TileCountX))
                {
                    var bite = arrangement[x, y];
                    w.Write("0x{0:x2}, ", bite < min || bite > max ? 0 : bite - min);
                }
                w.WriteLine();
            }
            w.WriteLine("\t\t\t};");
            w.WriteLine("\t\t\treturn creat_WB({0}, {1}, data);",
                arrangement.TileCountX, arrangement.TileCountY);
            w.WriteLine("\t\t}");
        }
    }
}

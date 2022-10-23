using System.IO;
using System.Linq;

namespace SnesGame.ResourceCompiler.Pipelines.WideBackground
{
    public static class WideBackgroundHeaderFile
    {
        public static void WriteWideBackgroundHeaderFile(this Manifest manifest)
        {
            PipelineUtils.WriteFileIfStale(
                manifest.GetWideBackgrounds().Select(wb => wb.SourcePath),
                Path.Combine(manifest.OutputDirectory, "wide_backgrounds.h"),
                writer =>
                {
                    writer.WriteLine("#pragma once");
                    writer.WriteLine("#include \"snes_devkit.h\"");
                    writer.WriteLine();

                    int i = 0;
                    foreach (var wideBackground in manifest.GetWideBackgrounds())
                        foreach (var layerId in Enumerable.Range(0, wideBackground.LayerCount))
                            writer.WriteLine("#define WB_{0}_{1} {2}",
                                wideBackground.ID, layerId, i++);

                    writer.WriteLine();
                    writer.WriteLine("hWB loadWB(Uint16 wbID);");
                });
        }
    }
}

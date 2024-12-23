using System.IO;
using System.Linq;

namespace SnesGame.ResourceCompiler.Pipelines.Brushlist
{
    public static class BrushlistHeaderFile
    {
        public static void WriteBrushlistHeaderFile(this Manifest manifest)
        {
            PipelineUtils.WriteFileIfStale(
                manifest.GetBrushlists().Select(wb => wb.SourcePath),
                Path.Combine(manifest.OutputDirectory, "brushlists.h"),
                writer =>
                {
                    writer.WriteLine("#pragma once");
                    writer.WriteLine("#include \"snes_runtime.h\"");
                    writer.WriteLine();

                    int brushListIndex = 0;
                    foreach (var brushlist in manifest.GetBrushlists())
                    {
                        writer.WriteLine("#define BL_{0} {1}", brushlist.ID, brushListIndex++);
                    }

                    writer.WriteLine();
                    writer.WriteLine("void loadBrushListResources(hINIT init);");
                });
        }
    }
}

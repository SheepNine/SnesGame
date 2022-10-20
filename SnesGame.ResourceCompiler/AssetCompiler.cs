using System.IO;
using System;

namespace SnesGame.ResourceCompiler
{
    public static class AssetCompiler
    {
        public static void Compile(string manifestFilename, string outputDirectory, params Pipeline[] pipelines)
        {
            if (!File.Exists(manifestFilename))
                throw new FileNotFoundException("Manifest not found", manifestFilename);

            try
            {
                Directory.CreateDirectory(outputDirectory);
            }
            catch (Exception ex)
            {
                throw new ApplicationException("Failed to create output directory", ex);
            }

            var manifest = new Manifest(manifestFilename, pipelines, outputDirectory);
            foreach (var pipeline in pipelines)
                pipeline.CompileEntries(manifest);
        }
    }
}

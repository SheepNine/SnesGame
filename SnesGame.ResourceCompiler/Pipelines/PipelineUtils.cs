using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;

namespace SnesGame.ResourceCompiler.Pipelines
{
    public static class PipelineUtils
    {

        public static void WriteFileIfStale(IEnumerable<string> inputFilenames,
            string outputFilename, Action<TextWriter> writeMethod)
        {
            Console.Write(Path.GetFileName(outputFilename));

            if (inputFilenames.Count() == 0)
            {
                Console.WriteLine(" has no inputs; skipping");
                return;
            }
            if (OutputIsCurrent(inputFilenames, outputFilename))
            {
                Console.WriteLine(" is current; skipping");
                return;
            }

            try
            {
                using (var writer = File.CreateText(outputFilename))
                    writeMethod(writer);
            }
            catch (Exception)
            {
                Console.WriteLine(" could not be written; an error has occurred");
                throw;
            }

            Console.WriteLine(" written");
        }

        public static bool OutputIsCurrent(IEnumerable<string> sourceFiles, string outputFile)
        {
            return sourceFiles.All(sourceFile => OutputIsCurrent(sourceFile, outputFile));
        }

        public static bool OutputIsCurrent(string sourceFile, string outputFile)
        {
            var sourceInfo = new FileInfo(sourceFile);
            var outputInfo = new FileInfo(outputFile);
            return outputInfo.Exists
                && outputInfo.LastWriteTimeUtc >= sourceInfo.LastWriteTimeUtc;
        }
    }
}

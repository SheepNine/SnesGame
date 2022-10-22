using SnesGame.ResourceCompiler.Pipelines.Brushlist;
using SnesGame.ResourceCompiler.Pipelines.Palette;
using SnesGame.ResourceCompiler.Pipelines.Recording;
using SnesGame.ResourceCompiler.Pipelines.WideBackground;
using System;
using System.IO;

namespace SnesGame.ResourceCompiler
{

    static class Program
    {
        public static void Main(string[] args)
        {
            if (args.Length != 2)
            {
                Console.WriteLine("Usage: AssetPipeline.exe <manifest file> <output root directory>");
                return;
            }

            AssetCompiler.Compile(Path.GetFullPath(args[0]), Path.GetFullPath(args[1]),
                BrushlistPipeline.Instance,
                WideBackgroundPipeline.Instance,
                PalettePipeline.Instance,
                RecordingPipeline.Instance
            );
        }
    }
}

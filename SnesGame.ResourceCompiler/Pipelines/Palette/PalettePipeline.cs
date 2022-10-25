using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Xml;

namespace SnesGame.ResourceCompiler.Pipelines.Palette
{
    public class PalettePipeline : IPipeline
    {
        private PalettePipeline() { }
        private static readonly IPipeline instance = new PalettePipeline();
        public static IPipeline Instance { get { return instance; } }

        public string EntryTag { get { return PaletteEntry.Tag; } }

        public IEnumerable<ManifestEntry> ParseEntries(XmlDocument doc, string baseDirectory,
            string outputDirectory)
        {
            return doc
                .SelectNodes("/AssetManifest/Palettes/Palette")
                .Cast<XmlElement>().Select(node => new
                {
                    id = node.GetAttribute("id"),
                    source = node.GetAttribute("source")
                })
                .Select(data => new PaletteEntry(data.id,
                Path.Combine(baseDirectory, data.source)));
        }

        public void CompileEntries(Manifest manifest)
        {
            manifest.WritePaletteHeaderFile();
            manifest.WritePaletteCodeFile();
        }
    }
}

using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Xml;

namespace SnesGame.ResourceCompiler.Pipelines.Brushlist
{
    public class BrushlistPipeline : IPipeline
    {
        private BrushlistPipeline() { }
        private static readonly IPipeline instance = new BrushlistPipeline();
        public static IPipeline Instance { get { return instance; } }

        public string EntryTag { get { return BrushlistEntry.Tag; } }

        public IEnumerable<ManifestEntry> ParseEntries(XmlDocument doc, string baseDirectory,
            string outputDirectory)
        {
            return doc.SelectNodes("/AssetManifest/BrushLists/BrushList")
                .Cast<XmlElement>().Select(node => new
                {
                    id = node.GetAttribute("id"),
                    source = node.GetAttribute("source")
                }).Select(data => new BrushlistEntry(data.id,
                Path.Combine(baseDirectory, data.source)));
        }

        public void CompileEntries(Manifest manifest)
        {
            manifest.WriteBrushlistHeaderFile();
            manifest.WriteBrushlistCodeFile();
        }
    }
}

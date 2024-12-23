using System.Collections.Generic;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Xml;

namespace SnesGame.ResourceCompiler.Pipelines.WideBackground
{
    public class WideBackgroundPipeline : IPipeline
    {
        private WideBackgroundPipeline() { }
        private static readonly IPipeline instance = new WideBackgroundPipeline();
        public static IPipeline Instance { get { return instance; } }

        public string EntryTag { get { return WideBackgroundEntry.Tag; } }

        public IEnumerable<ManifestEntry> ParseEntries(XmlDocument doc,
            string baseDirectory, string outputDirectory)
        {
            return doc.SelectNodes("/AssetManifest/WideBackgrounds/WideBackground")
                .Cast<XmlElement>().Select(node => new
                {
                    id = node.GetAttribute("id"),
                    source = node.GetAttribute("source"),
                    layerCount = int.Parse(node.GetAttribute("layerCount"),
                        CultureInfo.InvariantCulture)
                }).Select(data => new WideBackgroundEntry(data.id,
                Path.Combine(baseDirectory, data.source), data.layerCount));
        }

        public void CompileEntries(Manifest manifest)
        {
            manifest.WriteWideBackgroundHeaderFile();
            manifest.WriteWideBackgroundCodeFile();
        }
    }
}

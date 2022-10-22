using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Xml;

namespace SnesGame.ResourceCompiler.Pipelines.Recording
{
    public class RecordingPipeline : Pipeline
    {
        private RecordingPipeline() { }
        private static readonly Pipeline instance = new RecordingPipeline();
        public static Pipeline Instance { get { return instance; } }

        public string EntryTag { get { return RecordingEntry.Tag; } }

        public IEnumerable<ManifestEntry> ParseEntries(XmlDocument doc, string baseDirectory, string outputDirectory)
        {
            return doc.SelectNodes("/AssetManifest/Recordings/Recording").Cast<XmlElement>().Select(node => new
            {
                id = node.GetAttribute("id"),
                source = node.GetAttribute("source")
            }).Select(data => new RecordingEntry(data.id, Path.Combine(baseDirectory, data.source)));
        }

        public void CompileEntries(Manifest manifest)
        {
            manifest.WriteRecordingHeaderFile();
            manifest.WriteRecordingCodeFile();
        }
    }
}

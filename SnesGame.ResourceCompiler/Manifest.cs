using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Xml;

namespace SnesGame.ResourceCompiler
{
    public class Manifest
    {
        public string OutputDirectory { get; private set; }

        private IDictionary<string, IDictionary<string, ManifestEntry>> entries;

        public Manifest(string manifestFilename, IEnumerable<Pipeline> pipelines,
            string outputDirectory)
        {
            OutputDirectory = outputDirectory;
            entries = new SortedDictionary<string, IDictionary<string, ManifestEntry>>();
            var baseDirectory = Path.GetDirectoryName(manifestFilename);
            using (var stream = File.OpenRead(manifestFilename))
            {
                var doc = new XmlDocument();
                doc.Load(stream);
                foreach (var pipeline in pipelines)
                    entries[pipeline.EntryTag] = pipeline.ParseEntries(
                        doc, baseDirectory, outputDirectory)
                        .ToDictionary(entry => entry.ID);
            }
        }

        public bool HasEntry(string tag, string id)
        {
            return entries[tag].ContainsKey(id);
        }

        public T GetEntry<T>(string tag, string id) where T : ManifestEntry
        {
            return entries[tag][id] as T;
        }

        public IEnumerable<T> GetEntries<T>(string tag) where T : ManifestEntry
        {
            return entries[tag].Values.Cast<T>();
        }

        public int GetEntryCount(string tag)
        {
            return entries[tag].Count;
        }
    }
}

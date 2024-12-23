using System.Collections.Generic;
using System.Xml;

namespace SnesGame.ResourceCompiler
{
    public interface IPipeline
    {
        string EntryTag { get; }
        IEnumerable<ManifestEntry> ParseEntries(XmlDocument doc,
            string baseDirectory, string outputDirectory);
        void CompileEntries(Manifest manifest);
    }
}

using System.IO;

namespace SnesGame.ResourceCompiler.Pipelines.Recording
{
    public class RecordingEntry : ManifestEntry
    {
        public const string Tag = "recording";

        public string SourcePath { get; private set; }

        public RecordingEntry(string id, string sourcePath) : base(Tag, id)
        {
            SourcePath = sourcePath;
            if (!File.Exists(sourcePath))
                throw new FileNotFoundException(string.Format("Source for recording '{0}' not found", id), sourcePath);
        }
    }
}

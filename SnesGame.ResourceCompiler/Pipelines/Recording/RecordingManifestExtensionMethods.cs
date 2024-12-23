using System.Collections.Generic;

namespace SnesGame.ResourceCompiler.Pipelines.Recording
{
    public static class RecordingManifestExtensionMethods
    {
        public static bool HasRecording(this Manifest manifest, string id)
        {
            return manifest.HasEntry(RecordingEntry.Tag, id);
        }

        public static RecordingEntry GetRecording(this Manifest manifest, string id)
        {
            return manifest.GetEntry<RecordingEntry>(RecordingEntry.Tag, id);
        }

        public static IEnumerable<RecordingEntry> GetRecordings(this Manifest manifest)
        {
            return manifest.GetEntries<RecordingEntry>(RecordingEntry.Tag);
        }

        public static int GetRecordingCount(this Manifest manifest)
        {
            return manifest.GetEntryCount(RecordingEntry.Tag);
        }
    }
}

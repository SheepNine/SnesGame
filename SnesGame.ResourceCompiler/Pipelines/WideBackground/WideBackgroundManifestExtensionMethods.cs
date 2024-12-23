using System.Collections.Generic;

namespace SnesGame.ResourceCompiler.Pipelines.WideBackground
{
    public static class WideBackgroundManifestExtensionMethods
    {
        public static bool HasWideBackground(this Manifest manifest, string id)
        {
            return manifest.HasEntry(WideBackgroundEntry.Tag, id);
        }

        public static WideBackgroundEntry GetWideBackground(this Manifest manifest, string id)
        {
            return manifest.GetEntry<WideBackgroundEntry>(WideBackgroundEntry.Tag, id);
        }

        public static IEnumerable<WideBackgroundEntry> GetWideBackgrounds(this Manifest manifest)
        {
            return manifest.GetEntries<WideBackgroundEntry>(WideBackgroundEntry.Tag);
        }

        public static int GetWideBackgroundCount(this Manifest manifest)
        {
            return manifest.GetEntryCount(WideBackgroundEntry.Tag);
        }
    }
}

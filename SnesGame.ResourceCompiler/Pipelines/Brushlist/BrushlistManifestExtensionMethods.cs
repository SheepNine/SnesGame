using System.Collections.Generic;

namespace SnesGame.ResourceCompiler.Pipelines.Brushlist
{
    public static class BrushlistManifestExtensionMethods
    {
        public static bool HasBrushlist(this Manifest manifest, string id)
        {
            return manifest.HasEntry(BrushlistEntry.Tag, id);
        }

        public static BrushlistEntry GetBrushlist(this Manifest manifest, string id)
        {
            return manifest.GetEntry<BrushlistEntry>(BrushlistEntry.Tag, id);
        }

        public static IEnumerable<BrushlistEntry> GetBrushlists(this Manifest manifest)
        {
            return manifest.GetEntries<BrushlistEntry>(BrushlistEntry.Tag);
        }

        public static int GetBrushlistCount(this Manifest manifest)
        {
            return manifest.GetEntryCount(BrushlistEntry.Tag);
        }
    }
}

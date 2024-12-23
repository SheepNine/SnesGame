using System.Collections.Generic;

namespace SnesGame.ResourceCompiler.Pipelines.Palette
{
    public static class PaletteManifestExtensionMethods
    {
        public static bool HasPalette(this Manifest manifest, string id)
        {
            return manifest.HasEntry(PaletteEntry.Tag, id);
        }

        public static PaletteEntry GetPalette(this Manifest manifest, string id)
        {
            return manifest.GetEntry<PaletteEntry>(PaletteEntry.Tag, id);
        }

        public static IEnumerable<PaletteEntry> GetPalettes(this Manifest manifest)
        {
            return manifest.GetEntries<PaletteEntry>(PaletteEntry.Tag);
        }

        public static int GetPaletteCount(this Manifest manifest)
        {
            return manifest.GetEntryCount(PaletteEntry.Tag);
        }
    }
}

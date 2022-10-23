using System.Drawing;
using System.IO;
using System.Linq;

namespace SnesGame.ResourceCompiler.Pipelines.Palette
{
    public class PaletteEntry : ManifestEntry
    {
        public const string Tag = "palette";

        public string SourcePath { get; private set; }

        public PaletteEntry(string id, string sourcePath) : base(Tag, id)
        {
            SourcePath = sourcePath;

            if (!File.Exists(sourcePath))
                throw new FileNotFoundException(
                    string.Format("Source for palette '{0}' not found", id), sourcePath);
        }

        public Color[] LoadPaletteColors()
        {
            using (var bitmap = Bitmap.FromFile(SourcePath) as Bitmap)
            {
                if (bitmap.Width == 128 && bitmap.Height == 129)
                {
                    return Enumerable.Range(0, 16)
                        .Select(i => bitmap.GetPixel(8 * i, 128)).ToArray();
                }
                if (bitmap.Width == 32 && bitmap.Height == 32)
                {
                    return Enumerable.Range(0, 16)
                        .Select(i => bitmap.GetPixel(8 * (i % 4), 8 * (i / 4))).ToArray();
                }
            }
            throw new InvalidDataException(
                string.Format("Unrecognized format for palette '{0}'", ID));
        }
    }
}

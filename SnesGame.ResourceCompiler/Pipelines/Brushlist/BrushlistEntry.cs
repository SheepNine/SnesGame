using SnesGame.ResourceCompiler.Pipelines.Palette;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Globalization;
using System.IO;
using System.Linq;
using UnaryHeap.Mosaic;

namespace SnesGame.ResourceCompiler.Pipelines.Brushlist
{
    public class BrushlistEntry : ManifestEntry
    {
        public const string Tag = "brushlist";

        public string SourcePath { get; private set; }

        public BrushlistEntry(string id, string sourcePath) : base(Tag, id)
        {
            SourcePath = sourcePath;

            if (!File.Exists(sourcePath))
                throw new FileNotFoundException(
                    string.Format(CultureInfo.InvariantCulture,
                        "Source for brushlist '{0}' not found", id), sourcePath);
        }

        public Bitmap LoadSourceAsBitmap()
        {
            try
            {
                return (Bitmap)Image.FromFile(SourcePath);
            }
            catch (Exception ex)
            {
                throw new InvalidDataException(string.Format(CultureInfo.InvariantCulture,
                    "Error loading bitmap of brushlist '{0}'", ID), ex);
            }
        }

        public ITileset LoadSourceAsTransparentTileset(PaletteEntry palette)
        {
            try
            {
                using (var bitmap = LoadSourceAsBitmap())
                {
                    var paletteColors = palette.LoadPaletteColors();
                    var paletteSwap = new Dictionary<Color, Color>();
                    foreach (var i in Enumerable.Range(0, 16))
                    {
                        var key = bitmap.GetPixel(8 * i, 128);
                        var value = Color.FromArgb(128, paletteColors[i]);
                        if (!paletteSwap.ContainsKey(key))
                            paletteSwap[key] = value;
                    }
                    var maskColor = bitmap.GetPixel(0, 128);
                    paletteSwap[maskColor] = Color.Transparent;

                    using (var croppedBitmap = new Bitmap(128, 128))
                    {
                        using (var g = Graphics.FromImage(croppedBitmap))
                            g.DrawImage(bitmap, 0, 0);

                        foreach (var y in Enumerable.Range(0, 128))
                            foreach (var x in Enumerable.Range(0, 128))
                                croppedBitmap.SetPixel(x, y,
                                    paletteSwap[croppedBitmap.GetPixel(x, y)]);

                        return new ImageTileset(croppedBitmap, 8);
                    }
                }
            }
            catch (Exception ex)
            {
                throw new InvalidDataException(string.Format(CultureInfo.InvariantCulture,
                    "Error loading tileset of brushlist '{0}'", ID), ex);
            }
        }
    }
}

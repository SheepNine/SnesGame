using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Linq;

namespace ResourceCompiler
{
    static class GlyphCompiler
    {
        public static void Compile(string imageFilename)
        {
            Compile(imageFilename, Path.ChangeExtension(imageFilename, "glyphset"));
        }

        private static void Compile(string imageFilename, string outputFilename)
        {
            using (var outStream = File.Create(outputFilename))
            using (var bitmap = (Bitmap)Image.FromFile(imageFilename))
            {
                var paletteMap = new Dictionary<Color, int>();
                foreach (var i in Enumerable.Range(0, 16))
                {
                    var color = bitmap.GetPixel(8 * i, 128);
                    if (!paletteMap.ContainsKey(color))
                        paletteMap[color] = paletteMap.Count;
                }

                var glyph = new Glyph();
                foreach (var glyphIndex in Enumerable.Range(0, 256))
                {
                    var glyphOffsetX = (glyphIndex % 16) * 8;
                    var glyphOffsetY = (glyphIndex / 16) * 8;

                    foreach (var y in Enumerable.Range(0, 8))
                        foreach (var x in Enumerable.Range(0, 8))
                            glyph.SetIndex(x, y, paletteMap[bitmap.GetPixel(glyphOffsetX + x, glyphOffsetY + y)]);

                    outStream.Write(glyph.Bytes.ToArray(), 0, 32);
                }
            }
        }
    }
}

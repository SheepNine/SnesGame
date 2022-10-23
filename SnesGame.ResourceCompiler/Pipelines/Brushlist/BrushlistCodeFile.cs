using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Linq;

namespace SnesGame.ResourceCompiler.Pipelines.Brushlist
{
    public static class BrushlistCodeFile
    {
        public static void WriteBrushlistCodeFile(this Manifest manifest)
        {
            PipelineUtils.WriteFileIfStale(
                manifest.GetBrushlists().Select(wb => wb.SourcePath),
                Path.Combine(manifest.OutputDirectory, "brushlists.c"),
                writer =>
            {
                writer.WriteLine("#include \"brushlists.h\"");
                writer.WriteLine();

                foreach (var brushlist in manifest.GetBrushlists())
                {
                    var brushListBytes = new BrushListBytes(brushlist);
                    writer.WriteLine("Uint8 {0}_DATA[8192] = {{", brushlist.ID);
                    foreach (var brushBytes in brushListBytes.Brushes)
                    {
                        writer.Write('\t');
                        foreach (var bite in brushBytes.Bytes)
                        {
                            writer.Write("0x{0:x2}, ", bite);
                        }
                        writer.WriteLine();
                    }
                    writer.WriteLine("};");
                }

                writer.WriteLine();
                writer.WriteLine("void loadBrushListResources(hINIT init) {");
                writer.WriteLine("\tallocBrushLists(init, {0});", manifest.GetBrushlistCount());

                foreach (var brushlist in manifest.GetBrushlists())
                {
                    writer.WriteLine("\tinitBrushList(init, BL_{0}, {0}_DATA);", brushlist.ID);
                }

                writer.WriteLine("}");
            });
        }
    }

    class BrushListBytes
    {
        private BrushBytes[] brushes = new BrushBytes[256];

        public BrushListBytes(BrushlistEntry manifestBrushlist)
        {
            using (var bitmap = manifestBrushlist.LoadSourceAsBitmap())
            {
                var paletteMap = new Dictionary<Color, int>();
                foreach (var i in Enumerable.Range(0, 16))
                {
                    var color = bitmap.GetPixel(8 * i, 128);
                    if (!paletteMap.ContainsKey(color))
                        paletteMap[color] = paletteMap.Count;
                }

                foreach (var glyphIndex in Enumerable.Range(0, 256))
                {
                    var glyphOffsetX = (glyphIndex % 16) * 8;
                    var glyphOffsetY = (glyphIndex / 16) * 8;

                    var brush = new BrushBytes();
                    brushes[glyphIndex] = brush;
                    foreach (var y in Enumerable.Range(0, 8))
                        foreach (var x in Enumerable.Range(0, 8))
                        {
                            var sourceColor = bitmap.GetPixel(glyphOffsetX + x, glyphOffsetY + y);
                            if (!paletteMap.ContainsKey(sourceColor))
                            {
                                throw new InvalidDataException(string.Format(
                                    "Brush list {0} has unexpected color {1}",
                                    manifestBrushlist.ID, sourceColor));
                            }
                            brush.SetIndex(x, y, paletteMap[sourceColor]);
                        }
                }
            }
        }

        public IEnumerable<BrushBytes> Brushes
        {
            get { return brushes; }
        }
    }

    class BrushBytes
    {
        private byte[] data;

        public BrushBytes()
        {
            data = Enumerable.Repeat((byte)0, 32).ToArray();
        }

        public int GetIndex(int x, int y)
        {
            if (x < 0 || x >= 8)
                throw new ArgumentOutOfRangeException("x");
            if (y < 0 || y >= 8)
                throw new ArgumentOutOfRangeException("y");

            var mask = (byte)(0x1 << x);
            var result = (byte)0;
            if ((data[4 * y + 0] & mask) != 0)
                result |= 0x1;
            if ((data[4 * y + 1] & mask) != 0)
                result |= 0x2;
            if ((data[4 * y + 2] & mask) != 0)
                result |= 0x4;
            if ((data[4 * y + 3] & mask) != 0)
                result |= 0x8;

            return result;
        }

        public void SetIndex(int x, int y, int value)
        {
            if (x < 0 || x >= 8)
                throw new ArgumentOutOfRangeException("x");
            if (y < 0 || y >= 8)
                throw new ArgumentOutOfRangeException("y");
            if (value < 0 || value > 0xF)
                throw new ArgumentOutOfRangeException("value");

            var mask = (byte)(0x1 << x);

            if ((value & 0x1) != 0)
                data[4 * y + 0] |= mask;
            else
                data[4 * y + 0] &= (byte)~mask;
            if ((value & 0x2) != 0)
                data[4 * y + 1] |= mask;
            else
                data[4 * y + 1] &= (byte)~mask;
            if ((value & 0x4) != 0)
                data[4 * y + 2] |= mask;
            else
                data[4 * y + 2] &= (byte)~mask;
            if ((value & 0x8) != 0)
                data[4 * y + 3] |= mask;
            else
                data[4 * y + 3] &= (byte)~mask;
        }

        public IEnumerable<byte> Bytes
        {
            get
            {
                return data;
            }
        }
    }
}

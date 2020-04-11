using System;
using System.Linq;

namespace ReferenceImplementations
{
    class TestHarness
    {
        static void Main(string[] args)
        {
            GlyphTests.AllZero();
            GlyphTests.AllOne();
            GlyphTests.AllTwo();
            GlyphTests.AllFour();
            GlyphTests.AllEight();
            GlyphTests.AllFifteen();
            GlyphTests.DiagonalStripe();
        }
    }

    class GlyphTests
    {
        private static void ExpectData(Glyph sut, string expected)
        {
            var actual = String.Join(string.Empty, sut.Bytes.Select(b => b.ToString("X2")));
            expected = expected.Replace(" ", string.Empty);
            if (!String.Equals(actual, expected))
            {
                System.Diagnostics.Debugger.Break();
            }
        }

        private static void WriteAndRead(Glyph sut, int x, int y, int value)
        {
            sut.SetIndex(x, y, value);
            if (sut.GetIndex(x, y) != value)
                System.Diagnostics.Debugger.Break();
        }

        public static void AllZero()
        {
            var sut = new Glyph();
            for (int y = 0; y < 8; y++)
                for (int x = 0; x < 8; x++)
                    WriteAndRead(sut, x, y, 0);

            ExpectData(sut, "00 00 00 00  00 00 00 00  00 00 00 00  00 00 00 00  00 00 00 00  00 00 00 00  00 00 00 00  00 00 00 00");
        }

        public static void AllOne()
        {
            var sut = new Glyph();
            for (int y = 0; y < 8; y++)
                for (int x = 0; x < 8; x++)
                    WriteAndRead(sut, x, y, 1);

            ExpectData(sut, "FF 00 00 00  FF 00 00 00  FF 00 00 00  FF 00 00 00  FF 00 00 00  FF 00 00 00  FF 00 00 00  FF 00 00 00");
        }

        public static void AllTwo()
        {
            var sut = new Glyph();
            for (int y = 0; y < 8; y++)
                for (int x = 0; x < 8; x++)
                    WriteAndRead(sut, x, y, 2);

            ExpectData(sut, "00 FF 00 00  00 FF 00 00  00 FF 00 00  00 FF 00 00  00 FF 00 00  00 FF 00 00  00 FF 00 00  00 FF 00 00");
        }

        public static void AllFour()
        {
            var sut = new Glyph();
            for (int y = 0; y < 8; y++)
                for (int x = 0; x < 8; x++)
                    WriteAndRead(sut, x, y, 4);

            ExpectData(sut, "00 00 FF 00  00 00 FF 00  00 00 FF 00  00 00 FF 00  00 00 FF 00  00 00 FF 00  00 00 FF 00  00 00 FF 00");
        }

        public static void AllEight()
        {
            var sut = new Glyph();
            for (int y = 0; y < 8; y++)
                for (int x = 0; x < 8; x++)
                    WriteAndRead(sut, x, y, 8);

            ExpectData(sut, "00 00 00 FF  00 00 00 FF  00 00 00 FF  00 00 00 FF  00 00 00 FF  00 00 00 FF  00 00 00 FF  00 00 00 FF");
        }

        public static void AllFifteen()
        {
            var sut = new Glyph();
            for (int y = 0; y < 8; y++)
                for (int x = 0; x < 8; x++)
                    WriteAndRead(sut, x, y, 15);

            ExpectData(sut, "FF FF FF FF  FF FF FF FF  FF FF FF FF  FF FF FF FF  FF FF FF FF  FF FF FF FF  FF FF FF FF  FF FF FF FF");
        }

        public static void DiagonalStripe()
        {
            var sut = new Glyph();
            for (int i = 0; i < 8; i++)
                WriteAndRead(sut, i, i, 15);

            ExpectData(sut, "01 01 01 01  02 02 02 02  04 04 04 04  08 08 08 08  10 10 10 10  20 20 20 20  40 40 40 40  80 80 80 80");
        }
    }
}
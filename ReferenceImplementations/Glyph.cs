using System.Linq;
using System.Collections.Generic;
using System;

namespace ReferenceImplementations
{
    public class Glyph
    {
        private byte[] data;

        public Glyph()
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

using System.Collections.Generic;
using UnaryHeap.Mosaic;

namespace Patchwork
{
    public class Stamp
    {
        private static Stamp[] _stamps =
        {
            Rectangular(2, 2),
            Rectangular(4, 4),
            Rectangular(8, 8),
            Rectangular(2, 4),
            Rectangular(4, 2),
            Rectangular(4, 8),
            Rectangular(8, 4),
            RectangularFill(2, 2),
            RectangularFill(4, 4),
            RectangularFill(8, 8),
            OneBySix(),
            XEdge(),
            XWallLow(),
            XWall(),
            YEdge(),
            YWallLow(),
            YWall(),
            WallSeam(),
        };

        public static IEnumerable<Stamp> Stamps
        {
            get { return _stamps; }
        }

        int[] dX, dY, dTileX, dTileY;
        public string Title { get; private set; }

        private Stamp(int[] dX, int[] dY, int[] dTileX, int[] dTileY, string title)
        {
            this.dX = dX;
            this.dY = dY;
            this.dTileX = dTileX;
            this.dTileY = dTileY;
            Title = title;
        }

        public void Apply(TileArrangement m, int x, int y, int tile, int tileStride)
        {
            for (int i = 0; i < dX.Length; i++)
            {
                int destX = x + dX[i];
                int destY = y + dY[i];
                int destTile = tile + dTileX[i] + dTileY[i] * tileStride;
                if (destX < 0 || destX >= m.TileCountX)
                    continue;
                if (destY < 0 || destY >= m.TileCountY)
                    continue;
                if (destTile < 0)
                    return;

                m[destX, destY] = destTile;
            }
        }

        // -----------------------------------------------------------------------------------

        public static Stamp Rectangular(int sizeX, int sizeY)
        {
            var sizeI = sizeX * sizeY;
            var dX = new int[sizeI];
            var dY = new int[sizeI];
            var dTileX = new int[sizeI];
            var dTileY = new int[sizeI];

            for (var i = 0; i < sizeI; i++)
            {
                var x = i % sizeX;
                var y = i / sizeX;
                dX[i] = x;
                dY[i] = y;
                dTileX[i] = x;
                dTileY[i] = y;
            }
            var title = string.Format("{0}x{1}", sizeX, sizeY);
            return new Stamp(dX, dY, dTileX, dTileY, title);
        }

        public static Stamp RectangularFill(int sizeX, int sizeY)
        {
            var sizeI = sizeX * sizeY;
            var dX = new int[sizeI];
            var dY = new int[sizeI];
            var dTileX = new int[sizeI];
            var dTileY = new int[sizeI];

            for (var i = 0; i < sizeI; i++)
            {
                var x = i % sizeX;
                var y = i / sizeX;
                dX[i] = x;
                dY[i] = y;
                dTileX[i] = 0;
                dTileY[i] = 0;
            }
            var title = string.Format("{0}x{1} Fill", sizeX, sizeY);
            return new Stamp(dX, dY, dTileX, dTileY, title);
        }

        public static Stamp XEdge()
        {
            int[] dX = { 0, 1, 2, 3 };
            int[] dY = { 0, 0, 1, 1 };
            int[] dTileX = { 0, 1, 2, 3 };
            int[] dTileY = { 0, 0, 1, 1 };
            return new Stamp(dX, dY, dTileX, dTileY, "X-Edge");
        }

        public static Stamp XWallLow()
        {
            int[] dX = new int[6];
            int[] dY = new int[6];
            int[] dTileX = new int[6];
            int[] dTileY = new int[6];

            for (var i = 0; i < 6; i++)
            {
                var x = 3 - i % 4;
                var y = 6 - (i / 4);
                dX[i] = x;
                dY[i] = y - 6 + (x / 2);
                dTileX[i] = x;
                dTileY[i] = y - 6 + (x / 2);
            }
            return new Stamp(dX, dY, dTileX, dTileY, "Low X-Wall");
        }

        public static Stamp XWall()
        {
            int[] dX = new int[28];
            int[] dY = new int[28];
            int[] dTileX = new int[28];
            int[] dTileY = new int[28];

            for (var i = 0; i < 28; i++)
            {
                var x = i % 4;
                var y = i / 4;
                dX[i] = x;
                dY[i] = y - 6 + (x / 2);
                dTileX[i] = x;
                dTileY[i] = y - 6 + (x / 2);
            }
            return new Stamp(dX, dY, dTileX, dTileY, "X-Wall");
        }

        public static Stamp YEdge()
        {
            int[] dX = { 0, -1, -2, -3 };
            int[] dY = { 0, 0, 1, 1 };
            int[] dTileX = { 0, -1, -2, -3 };
            int[] dTileY = { 0, 0, 1, 1 };
            return new Stamp(dX, dY, dTileX, dTileY, "Y-Edge");
        }

        public static Stamp YWallLow()
        {
            int[] dX = new int[6];
            int[] dY = new int[6];
            int[] dTileX = new int[6];
            int[] dTileY = new int[6];

            for (var i = 0; i < 6; i++)
            {
                var x = (i % 4);
                var y = 6 - (i / 4);
                dX[i] = x - 3;
                dY[i] = y - 5 - (x / 2);
                dTileX[i] = x - 3;
                dTileY[i] = y - 5 - (x / 2);
            }
            return new Stamp(dX, dY, dTileX, dTileY, "Low Y-Wall");
        }

        public static Stamp YWall()
        {
            int[] dX = new int[28];
            int[] dY = new int[28];
            int[] dTileX = new int[28];
            int[] dTileY = new int[28];

            for (var i = 0; i < 28; i++)
            {
                var x = i % 4;
                var y = i / 4;
                dX[i] = x - 3;
                dY[i] = y - 5 - (x / 2);
                dTileX[i] = x - 3;
                dTileY[i] = y - 5 - (x / 2);
            }
            return new Stamp(dX, dY, dTileX, dTileY, "Y-Wall");
        }

        public static Stamp WallSeam()
        {
            int[] dX = new int[14];
            int[] dY = new int[14];
            int[] dTileX = new int[14];
            int[] dTileY = new int[14];

            for (var i = 0; i < 14; i++)
            {
                var x = i % 2;
                var y = i / 2;
                dX[i] = x;
                dY[i] = y - 6;
                dTileX[i] = dX[i];
                dTileY[i] = dY[i];
            }
            return new Stamp(dX, dY, dTileX, dTileY, "Wall Seam");
        }

        public static Stamp OneBySix()
        {
            int[] dX = new int[6];
            int[] dY = new int[6];
            int[] dTileX = new int[6];
            int[] dTileY = new int[6];

            for (var i = 0; i < 6; i++)
            {
                dX[i] = 0;
                dY[i] = -5 + i;
                dTileX[i] = 0;
                dTileY[i] = dY[i];
            }
            return new Stamp(dX, dY, dTileX, dTileY, "1x6");
        }
    }
}

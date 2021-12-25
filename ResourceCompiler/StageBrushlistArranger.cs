using System;
using System.Drawing;
using System.Drawing.Imaging;

namespace ResourceCompiler
{
    class StageBrushlistArranger
    {
        private static void Arrange()
        {
            using (var sourceImage = (Bitmap)Bitmap.FromFile(@"C:\Users\marsh\Desktop\Sample.png"))
            using (var floor = new Bitmap(64, 32))
            using (var destImage = new Bitmap(128, 129))
            {
                ClearImage(floor, Color.Transparent);
                ClearImage(destImage, Color.CornflowerBlue);

                StripeFloor(sourceImage, floor, 0, 0, 0, 0);
                ConstructFloorTiles(floor, destImage, 32, 0);


                for (var i = 0; i < 48; i++)
                {
                    Stripe(32,
                        sourceImage, 32, i, 1, 0,
                        destImage, 0, 48 + i, 1, -2);
                    Stripe(32,
                        sourceImage, 64, i, 1, 0,
                        destImage, 32, 33 + i, 1, 2);
                }

                using (var g = Graphics.FromImage(destImage))
                {
                    g.DrawImage(floor, 0, 88, new Rectangle(0, 8, 16, 8), GraphicsUnit.Pixel);
                    g.DrawImage(floor, 16, 80, new Rectangle(16, 0, 16, 8), GraphicsUnit.Pixel);

                    g.DrawImage(floor, 32, 80, new Rectangle(32, 0, 16, 8), GraphicsUnit.Pixel);
                    g.DrawImage(floor, 48, 88, new Rectangle(48, 8, 16, 8), GraphicsUnit.Pixel);
                }

                destImage.Save(@"C:\Users\marsh\Desktop\SampleOut.png", ImageFormat.Png);
            }
        }

        private static void ConstructFloorTiles(Bitmap floor, Bitmap destImage, int x, int y)
        {
            using (var g = Graphics.FromImage(destImage))
            {
                g.DrawImage(floor, x, y, new Rectangle(16, 0, 32, 32), GraphicsUnit.Pixel);
                g.DrawImage(floor, x, y, new Rectangle(48, 16, 16, 16), GraphicsUnit.Pixel);
                g.DrawImage(floor, x + 16, y, new Rectangle(0, 16, 16, 16), GraphicsUnit.Pixel);
                g.DrawImage(floor, x, y + 16, new Rectangle(48, 0, 16, 16), GraphicsUnit.Pixel);
                g.DrawImage(floor, x + 16, y + 16, new Rectangle(0, 0, 16, 16), GraphicsUnit.Pixel);

                g.DrawImage(floor, x + 32, y, new Rectangle(32, 0, 16, 8), GraphicsUnit.Pixel);
                g.DrawImage(floor, x + 48, y, new Rectangle(16, 0, 16, 8), GraphicsUnit.Pixel);

                g.DrawImage(floor, x + 32, y + 8, new Rectangle(0, 8, 16, 16), GraphicsUnit.Pixel);
                g.DrawImage(floor, x + 48, y + 8, new Rectangle(48, 8, 16, 16), GraphicsUnit.Pixel);

                g.DrawImage(floor, x + 32, y + 24, new Rectangle(32, 24, 16, 8), GraphicsUnit.Pixel);
                g.DrawImage(floor, x + 48, y + 24, new Rectangle(16, 24, 16, 8), GraphicsUnit.Pixel);
            }
        }

        private static void ClearImage(Image image, Color c)
        {
            using (var g = Graphics.FromImage(image))
                g.Clear(c);
        }

        private static void StripeFloor(Bitmap source, Bitmap dest, int tileX, int tileY, int pgX, int pgY)
        {
            for (int i = 0; i < 16; i++)
            {
                Stripe(32 - i * 2,
                    source, tileX + i, tileY + 31 - i, 1, 0,
                    dest, pgX + 32, pgY + 31 - i, 1, -2);
                Stripe(32 - i * 2,
                    source, tileX + i, tileY + i, 1, 0,
                    dest, pgX + 2 * i, pgY + 16, 1, -2);
                Stripe(30 - i * 2,
                    source, tileX + i, tileY + 1 + i, 0, 1,
                    dest, pgX + 2 * (i + 1), pgY + 17, 1, 2);
                Stripe(30 - i * 2,
                    source, tileX + 31 - i, tileY + 1 + i, 0, 1,
                    dest, pgX + 32, pgY + 1 + i, 1, 2);
            }
        }

        private static void Stripe(int len, Bitmap source, int sX, int sY, int sStepX, int sStepY, Bitmap dest, int dX, int dY, int dStepX, int dStepY)
        {
            for (int i = 0; i < len; i++)
            {
                dest.SetPixel(dX + Math.Sign(dStepX) * (dStepX != 0 ? i / Math.Abs(dStepX) : 0), dY + Math.Sign(dStepY) * (dStepY != 0 ? i / Math.Abs(dStepY) : 0),
                    source.GetPixel(sX + Math.Sign(sStepX) * (sStepX != 0 ? i / Math.Abs(sStepX) : 0), sY + Math.Sign(sStepY) * (sStepY != 0 ? i / Math.Abs(sStepY) : 0)));
            }
        }
    }
}

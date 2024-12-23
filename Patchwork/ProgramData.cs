using System.Drawing;
using UnaryHeap.Mosaic;

namespace Patchwork
{
    static class ProgramData
    {
        #region Tileset

        static string DefaultTileImageFile
        {
            get { return "tileset_template_1x.png"; }
        }

        public static ITileset LoadTileset()
        {
            return new ImageTileset(new Bitmap(DefaultTileImageFile), 8);
        }

        #endregion
    }
}

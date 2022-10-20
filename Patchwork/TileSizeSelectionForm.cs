using System;
using System.Collections.Generic;
using System.Drawing;
using System.Windows.Forms;

namespace Patchwork
{
    public partial class TileSizeSelectionForm : Form
    {
        public TileSizeSelectionForm(Bitmap bitmap, List<int> tileSizeOptions)
        {
            InitializeComponent();
            FormBorderStyle = FormBorderStyle.FixedDialog;

            tilesPictureBox.Image = bitmap;

            tileSizeOptionsComboBox.BeginUpdate();
            foreach (var tileSizeOption in tileSizeOptions)
            {
                tileSizeOptionsComboBox.Items.Add(
                    string.Format("{0} pixels ({1}x{2} tiles)",
                        tileSizeOption, bitmap.Width / tileSizeOption,
                        bitmap.Height / tileSizeOption));
            }
            tileSizeOptionsComboBox.EndUpdate();
            tileSizeOptionsComboBox.SelectedIndex = 0;
        }

        public int SelectedTileSizeIndex
        {
            get { return tileSizeOptionsComboBox.SelectedIndex; }
        }

        public static int? QueryForTileSize(string fileName)
        {
            using (var bitmap = new Bitmap(fileName))
            {
                var width = bitmap.Width;
                var height = bitmap.Height;

                // Special case for brush list images
                if (width == 128 && height == 129)
                    return 8;

                List<int> tileSizeOptions = new List<int>();
                for (int i = 8; i < Math.Min(width, height); i++)
                    if (width % i == 0 && height % i == 0)
                        tileSizeOptions.Add(i);

                if (tileSizeOptions.Count == 0)
                {
                    MessageBox.Show("Image '' is not a valid tileset file. " +
                        "Tilesets must have width and height equal to " +
                        "a multiple of eight or greater.", "Change Tileset",
                        MessageBoxButtons.OK, MessageBoxIcon.Warning,
                        MessageBoxDefaultButton.Button1);
                    return null;
                }

                if (tileSizeOptions.Count == 1)
                    return tileSizeOptions[0];

                using (var dialog = new TileSizeSelectionForm(bitmap, tileSizeOptions))
                    if (dialog.ShowDialog() == DialogResult.OK)
                        return tileSizeOptions[dialog.SelectedTileSizeIndex];

                return null;
            }
        }
    }
}

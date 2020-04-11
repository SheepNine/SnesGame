using System;
using System.Drawing;
using System.Linq;
using System.Windows.Forms;
using ReferenceImplementations;

namespace Scribe
{
    public partial class MainForm : Form
    {
        Glyph glyph = new Glyph();
        Color[] palette = new Color[]
        {
            Color.FromArgb(0x00, 0x00, 0x00),
            Color.FromArgb(0x00, 0x00, 0xAA),
            Color.FromArgb(0x00, 0xAA, 0x00),
            Color.FromArgb(0x00, 0xAA, 0xAA),

            Color.FromArgb(0xAA, 0x00, 0x00),
            Color.FromArgb(0xAA, 0x00, 0xAA),
            Color.FromArgb(0xAA, 0x55, 0x00),
            Color.FromArgb(0xAA, 0xAA, 0xAA),

            Color.FromArgb(0x55, 0x55, 0x55),
            Color.FromArgb(0x55, 0x55, 0xFF),
            Color.FromArgb(0x55, 0xFF, 0x55),
            Color.FromArgb(0x55, 0xFF, 0xFF),

            Color.FromArgb(0xFF, 0x55, 0x55),
            Color.FromArgb(0xFF, 0x55, 0xFF),
            Color.FromArgb(0xFF, 0xFF, 0x55),
            Color.FromArgb(0xFF, 0xFF, 0xFF),
        };
        int activeColorIndex = 0;

        public MainForm()
        {
            InitializeComponent();
            UpdateLiteralText();
        }

        private void glyphPanel_Paint(object sender, PaintEventArgs e)
        {
            for (int y = 0; y < 8; y++)
                for (int x = 0; x < 8; x++)
                {
                    using (var brush = new SolidBrush(palette[glyph.GetIndex(x, y)]))
                        e.Graphics.FillRectangle(brush, 32 * x, 32 * y, 32, 32);

                    e.Graphics.DrawRectangle(Pens.Gray, 32 * x, 32 * y, 31, 31);
                }
        }

        private void palettePanel_Paint(object sender, PaintEventArgs e)
        {
            for (int i = 0; i < 16; i++)
            {
                using (var brush = new SolidBrush(palette[i]))
                    e.Graphics.FillRectangle(brush, 0, 16 * i, 16, 16);
                e.Graphics.DrawRectangle(Pens.Black, 0, 16 * i, 15, 15);
            }

            e.Graphics.DrawRectangle(Pens.White, 0, 16 * activeColorIndex, 15, 15);
        }

        private void palettePanel_MouseUp(object sender, MouseEventArgs e)
        {
            activeColorIndex = Math.Max(0, Math.Min(15, e.Y / 16));
            palettePanel.Invalidate();
        }

        private void glyphPanel_MouseUp(object sender, MouseEventArgs e)
        {
            PaintActiveColor(e);
        }

        private void UpdateLiteralText()
        {
            textBox1.Text = "Uint8 data[32] = { "
                + string.Join(", ", glyph.Bytes.Select(b => string.Format("0x{0:X2}", b)))
                + " };";
        }

        private void glyphPanel_MouseDown(object sender, MouseEventArgs e)
        {
            PaintActiveColor(e);
        }

        private void glyphPanel_MouseMove(object sender, MouseEventArgs e)
        {
            PaintActiveColor(e);
        }

        private void PaintActiveColor(MouseEventArgs e)
        {
            if (e.Button == MouseButtons.None)
                return;

            var x = Math.Max(0, Math.Min(7, e.X / 32));
            var y = Math.Max(0, Math.Min(7, e.Y / 32));

            if (glyph.GetIndex(x, y) == activeColorIndex)
                return;

            glyph.SetIndex(x, y, activeColorIndex);
            glyphPanel.Invalidate();
            UpdateLiteralText();
        }
    }
}

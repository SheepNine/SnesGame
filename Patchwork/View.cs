using System;
using System.Drawing;
using System.Drawing.Imaging;
using System.IO;
using System.Text;
using System.Windows.Forms;

namespace Patchwork
{

    public partial class View : Form
    {
        IViewModel viewModel;

        public View(IViewModel viewModel)
        {
            InitializeComponent();

            this.viewModel = viewModel;
            viewModel.HookUpToView(
                editorPanel, editorGestures,
                tilesetPanel, tilesetGestures,
                cursorPositionLabel);
            viewModel.CurrentFilenameChanged += viewModel_CurrentFilenameChanged;
            viewModel.IsModifiedChanged += viewModel_IsModifiedChanged;
            UpdateDialogText();
        }

        private void viewModel_IsModifiedChanged(object sender, EventArgs e)
        {
            UpdateDialogText();
        }

        private void viewModel_CurrentFilenameChanged(object sender, EventArgs e)
        {
            UpdateDialogText();
        }

        void UpdateDialogText()
        {
            var builder = new StringBuilder();

            if (viewModel.IsModified)
                builder.Append("*");

            if (null != viewModel.CurrentFileName)
            {
                builder.Append(Path.GetFileNameWithoutExtension(viewModel.CurrentFileName));
                builder.Append(" - ");
            }

            builder.Append("Patchwork");

            Text = builder.ToString();
        }

        #region File Menu Handlers

        private void fileToolStripMenuItem_DropDownOpening(object sender, EventArgs e)
        {
            viewModel.SyncMruList(openRecentToolStripMenuItem);
        }

        private void newToolStripMenuItem_Click(object sender, EventArgs e)
        {
            viewModel.NewArrangement();
        }

        private void loadToolStripMenuItem_Click(object sender, EventArgs e)
        {
            viewModel.OpenArrangement();
        }

        private void saveToolStripMenuItem_Click(object sender, EventArgs e)
        {
            viewModel.SaveArrangement();
        }

        private void saveAsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            viewModel.SaveArrangementAs();
        }

        private void exportToPNGToolStripMenuItem_Click(object sender, EventArgs e)
        {
            var dialog = new SaveFileDialog()
            {
                AddExtension = true,
                Filter = "Portable Network Graphics Files (*.png)|*.png",
                FilterIndex = 0,
                Title = "Select Filename for Export",
                OverwritePrompt = true,
                AutoUpgradeEnabled = true,
                CheckPathExists = true,
                CreatePrompt = false,
                DefaultExt = "png",
                RestoreDirectory = true,
            };

            using (dialog)
                if (DialogResult.OK == dialog.ShowDialog())
                    viewModel.Export(dialog.FileName, ImageFormat.Png);
        }
        private void closeToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Close();
        }

        #endregion


        #region Edit Menu Handlers

        private void undoToolStripMenuItem_Click(object sender, EventArgs e)
        {
            viewModel.Undo();
        }

        private void redoToolStripMenuItem_Click(object sender, EventArgs e)
        {
            viewModel.Redo();
        }

        private void copyRenderedArrangementToolStripMenuItem_Click(object sender, EventArgs e)
        {
            viewModel.CopyRenderedArrangement();
        }

        private void expandRightToolStripMenuItem_Click(object sender, EventArgs e)
        {
            viewModel.ExpandRight();
        }

        private void expandBottomToolStripMenuItem_Click(object sender, EventArgs e)
        {
            viewModel.ExpandBottom();
        }

        private void expandLeftToolStripMenuItem_Click(object sender, EventArgs e)
        {
            viewModel.ExpandLeft();
        }

        private void expandTopToolStripMenuItem_Click(object sender, EventArgs e)
        {
            viewModel.ExpandTop();
        }

        private void contractRightToolStripMenuItem_Click(object sender, EventArgs e)
        {
            viewModel.ContractRight();
        }

        private void contractBottomToolStripMenuItem_Click(object sender, EventArgs e)
        {
            viewModel.ContractBottom();
        }

        private void contractLeftToolStripMenuItem_Click(object sender, EventArgs e)
        {
            viewModel.ContractLeft();
        }

        private void contractTopToolStripMenuItem_Click(object sender, EventArgs e)
        {
            viewModel.ContractTop();
        }

        #endregion


        #region View Menu Handlers

        private void zoomInToolStripMenuItem_Click(object sender, EventArgs e)
        {
            viewModel.ZoomIn();
        }

        private void zoomOutToolStripMenuItem_Click(object sender, EventArgs e)
        {
            viewModel.ZoomOut();
        }

        private void toggleGridDisplayToolStripMenuItem_Click(object sender, EventArgs e)
        {
            viewModel.ToggleGridDisplay();
        }

        private void addTilesetToolStripMenuItem_Click(object sender, EventArgs e)
        {
            var dialog = new OpenFileDialog()
            {
                AutoUpgradeEnabled = true,
                CheckFileExists = true,
                Filter = "Image Files " +
"(*.gif;*.jpg;*.jpe*;*.png;*.bmp;*.dib;*.tif;*.wmf;*.ras;*.eps;*.pcx;*.pcd;*.tga)" +
"|*.gif;*.jpg;*.jpe*;*.png;*.bmp;*.dib;*.tif;*.wmf;*.ras;*.eps;*.pcx;*.pcd;*.tga",
                FilterIndex = 0,
                Multiselect = false,
                RestoreDirectory = true,
                Title = "Select Tileset Image"
            };

            using (dialog)
                if (DialogResult.OK == dialog.ShowDialog())
                {
                    var tileSize = viewModel.TileSize.HasValue
                        ? viewModel.TileSize.Value
                        : TileSizeSelectionForm.QueryForTileSize(dialog.FileName);

                    if (tileSize.HasValue)
                        viewModel.AddTileset(dialog.FileName, tileSize.Value);
                }
        }

        private void removeTilesetToolStripMenuItem_Click(object sender, EventArgs e)
        {
            // TODO: Fix NPE on removing -1th item
            viewModel.RemoveTileset();
        }

        private void reloadTilesetsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            viewModel.ReloadTilesets();
        }

        #endregion


        #region Miscellaneous Event Handlers

        private void tilesetPanel_SizeChanged(object sender, EventArgs e)
        {
            tilesetPanelBorder.Size = new Size(
                tilesetPanel.Width +
                    (tilesetPanelBorder.Width - tilesetPanelBorder.ClientSize.Width),
                tilesetPanel.Height +
                    (tilesetPanelBorder.Height - tilesetPanelBorder.ClientSize.Height)
                );
        }

        private void View_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (false == viewModel.CanClose())
                e.Cancel = true;
        }

        #endregion

        private void viewToolStripMenuItem_Click(object sender, EventArgs e)
        {
            removeTilesetToolStripMenuItem.Enabled = viewModel.CanRemoveTileset();
        }

        private void stampToolStripMenuItem_DropDownOpening(object sender, EventArgs e)
        {
            stampToolStripMenuItem.DropDownItems.Clear();
            foreach (var stamp in Stamp.Stamps)
            {
                var item = new ToolStripMenuItem();
                item.Text = stamp.Title;
                item.Checked = (viewModel.ActiveStamp.Title == stamp.Title);
                item.Click += (handlerSender, handlerE) => { viewModel.ActiveStamp = stamp; };
                stampToolStripMenuItem.DropDownItems.Add(item);
            }
        }
    }
}

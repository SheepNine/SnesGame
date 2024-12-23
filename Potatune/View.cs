using System;
using System.IO;
using System.Text;
using System.Windows.Forms;

namespace Potatune
{
    public partial class View : Form
    {
        IViewModel viewModel;

        public View(IViewModel viewModel)
        {
            InitializeComponent();

            this.viewModel = viewModel;
            viewModel.HookupToView(editorPanel, editorGestures, editorScrollBar);
            viewModel.CurrentFileNameChanged += viewModel_CurrentFilenameChanged;
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
                builder.Append('*');

            if (null != viewModel.CurrentFileName)
            {
                builder.Append(Path.GetFileNameWithoutExtension(viewModel.CurrentFileName));
                builder.Append(" - ");
            }

            builder.Append("Potatune");

            Text = builder.ToString();
        }


        #region File Menu Handlers

        private void newToolStripMenuItem_Click(object sender, EventArgs e)
        {
            viewModel.NewTune();
        }

        private void loadToolStripMenuItem_Click(object sender, EventArgs e)
        {
            viewModel.OpenTune();
        }

        private void saveToolStripMenuItem_Click(object sender, EventArgs e)
        {
            viewModel.SaveTune();
        }

        private void saveAsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            viewModel.SaveTuneAs();
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

        private void editRecordingLengthToolStripMenuItem_Click(object sender, EventArgs e)
        {
            viewModel.EditLength();
        }

        #endregion


        #region Tools Menu Handlers

        private void playSelectedRangeToolStripMenuItem_Click(object sender, EventArgs e)
        {
            viewModel.PlaySelectedRange();
        }

        private void stopPlaybackToolStripMenuItem_Click(object sender, EventArgs e)
        {
            viewModel.StopPlayback();
        }

        #endregion


        #region Miscellaneous Event Handlers

        private void View_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (false == viewModel.CanClose())
                e.Cancel = true;
        }

        #endregion

        private void increaseNoteSkipToolStripMenuItem_Click(object sender, EventArgs e)
        {
            viewModel.IncreaseSkip();
        }

        private void decreaseNoteSkipToolStripMenuItem_Click(object sender, EventArgs e)
        {
            viewModel.DecreaseSkip();
        }

        private void addTrackToolStripMenuItem_Click(object sender, EventArgs e)
        {
            viewModel.AddTrack();
        }
    }
}

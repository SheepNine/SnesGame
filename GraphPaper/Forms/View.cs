using System;
using System.IO;
using System.Text;
using System.Windows.Forms;
using UnaryHeap.GUI;

namespace GraphPaper
{
    partial class View : Form, IView
    {
        IViewModel viewModel;

        public View(IViewModel viewModel)
        {
            this.viewModel = viewModel;
            InitializeComponent();
            viewModel.CurrentFilenameChanged += viewModel_CurrentFilenameChanged;
            viewModel.IsModifiedChanged += viewModel_IsModifiedChanged;
            viewModel.ContentChanged += ViewModel_ContentChanged;
            viewModel.FeedbackChanged += ViewModel_FeedbackChanged;
            viewModel.SetViewExtents(editorPanel.ClientRectangle);
            UpdateDialogText();
        }

        private void ViewModel_FeedbackChanged(object sender, EventArgs e)
        {
            editorPanel.InvalidateFeedback();
        }

        private void ViewModel_ContentChanged(object sender, EventArgs e)
        {
            editorPanel.InvalidateContent();
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

            builder.Append("GraphPaper");

            Text = builder.ToString();
        }

        private void newToolStripMenuItem_Click(object sender, System.EventArgs e)
        {
            viewModel.New();
        }

        private void loadToolStripMenuItem_Click(object sender, System.EventArgs e)
        {
            viewModel.Load();
        }

        private void closeToolStripMenuItem_Click(object sender, System.EventArgs e)
        {
            Close();
        }

        private void wholeModelToolStripMenuItem_Click(object sender, System.EventArgs e)
        {
            viewModel.ViewWholeModel();
        }

        private void zoomInToolStripMenuItem_Click(object sender, EventArgs e)
        {
            viewModel.ZoomIn();
        }

        private void zoomOutToolStripMenuItem_Click(object sender, EventArgs e)
        {
            viewModel.ZoomOut();
        }

        private void undoToolStripMenuItem_Click(object sender, EventArgs e)
        {
            viewModel.Undo();
        }

        private void redoToolStripMenuItem_Click(object sender, EventArgs e)
        {
            viewModel.Redo();
        }

        private void View_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (false == viewModel.CanClose())
                e.Cancel = true;
        }

        private void saveToolStripMenuItem_Click(object sender, EventArgs e)
        {
            viewModel.Save();
        }

        private void saveAsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            viewModel.SaveAs();
        }

        private void selectAllToolStripMenuItem_Click(object sender, EventArgs e)
        {
            viewModel.SelectAll();
        }

        private void deleteToolStripMenuItem_Click(object sender, EventArgs e)
        {
            viewModel.DeleteSelected();
        }

        private void editorPanel_SizeChanged(object sender, EventArgs e)
        {
            if (0 != editorPanel.ClientRectangle.Width &&
                0 != editorPanel.ClientRectangle.Height)
                viewModel.SetViewExtents(editorPanel.ClientRectangle);
        }

        void EditorGestures_ClickGestured(object sender, ClickGestureEventArgs e)
        {
            GraphPaperToolbox.Instance
                .GetClickTool(e.ModifierKeys, e.Button)
                .Gestured(viewModel, e.ClickPoint);
        }

        void EditorGestures_DragGestured(object sender, DragGestureEventArgs e)
        {
            GraphPaperToolbox.Instance
                .GetDragTool(e.ModifierKeys, e.Button)
                .Gestured(viewModel, e.StartPoint, e.EndPoint);
        }

        private void editorPanel_PaintContent(object sender, PaintEventArgs e)
        {
            viewModel.PaintContent(e.Graphics);
        }

        private void editorPanel_PaintFeedback(object sender, PaintEventArgs e)
        {
            viewModel.PaintFeedback(e.Graphics, e.ClipRectangle);
        }

        private void editorGestures_StateChanged(object sender, EventArgs e)
        {
            switch (editorGestures.CurrentState)
            {
                case GestureState.Idle:
                    viewModel.RemoveFeedback();
                    break;
                case GestureState.Hover:
                    viewModel.PreviewHover(editorGestures.CurrentPosition);
                    break;
                case GestureState.Clicking:
                    GraphPaperToolbox.Instance
                        .GetClickTool(
                            editorGestures.ModifierKeys,
                            editorGestures.ClickButton)
                        .Gesturing(
                            viewModel,
                            editorGestures.CurrentPosition);
                    break;
                case GestureState.Dragging:
                    GraphPaperToolbox.Instance
                        .GetDragTool(
                            editorGestures.ModifierKeys,
                            editorGestures.ClickButton)
                        .Gesturing(
                            viewModel,
                            editorGestures.DragStartPosition,
                            editorGestures.CurrentPosition);
                    break;
            }
        }

        private void increaseGridResolutionToolStripMenuItem_Click(object sender, EventArgs e)
        {
            viewModel.IncreaseGridResolution();
        }

        private void decreaseGridResolutionToolStripMenuItem_Click(object sender, EventArgs e)
        {
            viewModel.DecreaseGridResolution();
        }

        private void selectNoneToolStripMenuItem_Click(object sender, EventArgs e)
        {
            viewModel.SelectNone();
        }

        private void editGraphMetadataToolStripMenuItem_Click(object sender, EventArgs e)
        {
            viewModel.EditGraphMetadata();
        }

        private void editVertexMetadataToolStripMenuItem_Click(object sender, EventArgs e)
        {
            viewModel.EditVertexMetadata();
        }

        private void editEdgeMetadataToolStripMenuItem_Click(object sender, EventArgs e)
        {
            viewModel.EditEdgeMetadata();
        }

        private void helpToolStripMenuItem_Click(object sender, EventArgs e)
        {
            MessageBox.Show(this, GraphPaperToolbox.Instance.HelpText,
                "GraphPaper Mouse Gestures",
                MessageBoxButtons.OK,
                MessageBoxIcon.Information,
                MessageBoxDefaultButton.Button1);
        }
    }

    interface IView
    {

    }
}

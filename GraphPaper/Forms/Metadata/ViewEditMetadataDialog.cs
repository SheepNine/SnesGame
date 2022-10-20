using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Windows.Forms;

namespace GraphPaper
{
    partial class ViewEditMetadataDialog : Form
    {
        List<MetadatumControl> controls = new List<MetadatumControl>();

        public ViewEditMetadataDialog(MetadataSet source)
        {
            InitializeComponent();
            FormBorderStyle = FormBorderStyle.FixedToolWindow;
            ResizeMetadatumControlsToFlowLayout();

            foreach (var entry in source.Data)
            {
                var k = entry.Key;
                var v = entry.Value;
                AddRow(k, v);
            }
        }

        private void AddRow(string key, string value)
        {
            var control = new MetadatumControl()
            {
                Key = key,
                Value = value,
                Width = flowLayoutPanel1.Width
            };

            control.RemoveRequested += Control_RemoveRequested;
            flowLayoutPanel1.Controls.Add(control);
            controls.Add(control);
            Realign();
        }

        private void RemoveRow(MetadatumControl c)
        {
            flowLayoutPanel1.Controls.Remove(c);
            controls.Remove(c);
            Realign();
        }

        private void Control_RemoveRequested(object sender, EventArgs e)
        {
            var c = sender as MetadatumControl;
            RemoveRow(c);
        }

        private void flowLayoutPanel1_SizeChanged(object sender, EventArgs e)
        {
            ResizeMetadatumControlsToFlowLayout();
        }

        private void ResizeMetadatumControlsToFlowLayout()
        {
            foreach (var control in controls)
                control.Width = flowLayoutPanel1.Width;
        }

        private void addKeyButton_Click(object sender, EventArgs e)
        {
            if (string.IsNullOrEmpty(addKeyTextBox.Text))
                return;

            foreach (var control in controls)
                if (control.Key.Equals(addKeyTextBox.Text))
                    return;

            AddRow(addKeyTextBox.Text, string.Empty);
            addKeyTextBox.Text = string.Empty;
        }

        private void Realign()
        {
            var longestRow = header.KeyLabelWidth;

            foreach (var control in controls)
                longestRow = Math.Max(longestRow, control.KeyLabelWidth);

            header.AlignTextBox(longestRow + 6);

            foreach (var control in controls)
                control.AlignTextBox(longestRow + 3);
        }

        public MetadataSet GetOutputMetadataSet()
        {
            var sources = new SortedDictionary<string, string>();

            foreach (var control in controls)
                sources.Add(control.Key, string.IsNullOrEmpty(control.Value) ?
                    null : control.Value);

            return new MetadataSet(new ReadOnlyDictionary<string, string>(sources));
        }
    }
}

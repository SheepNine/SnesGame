using System;
using System.Globalization;
using System.Windows.Forms;
using UnaryHeap.Graph;

namespace GraphPaper
{
    public partial class MetadatumControl : UserControl
    {
        public event EventHandler RemoveRequested;
        protected void OnRemoveRequested()
        {
            if (null != RemoveRequested)
                RemoveRequested(this, EventArgs.Empty);
        }

        string key = "<unset>";

        public MetadatumControl()
        {
            InitializeComponent();
        }

        public string Key
        {
            get
            {
                return key;
            }
            set
            {
                key = value;
                keyLabel.Text = string.Format(CultureInfo.InvariantCulture, "{0}:", value);
                valueTextBox.ReadOnly = Graph2D.IsReservedMetadataKey(key);
            }
        }

        public string Value
        {
            get
            {
                return valueTextBox.Text;
            }
            set
            {
                valueTextBox.Text = value ?? string.Empty;
            }
        }

        private void removeButton_Click(object sender, EventArgs e)
        {
            OnRemoveRequested();
        }

        public void AlignTextBox(int offset)
        {
            valueTextBox.Left = keyLabel.Left + offset + 3;
            valueTextBox.Width = removeButton.Left - 6 - valueTextBox.Left;
        }

        public int KeyLabelWidth
        {
            get { return keyLabel.Width; }
        }
    }
}

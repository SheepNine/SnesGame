using System;
using System.Windows.Forms;

namespace GraphPaper
{
    public partial class NewModelArgumentsDialog : Form
    {
        public bool Directed { get; private set; }

        public NewModelArgumentsDialog()
        {
            InitializeComponent();
            FormBorderStyle = FormBorderStyle.FixedDialog;
        }

        private void undirectedButton_Click(object sender, EventArgs e)
        {
            Directed = false;
            DialogResult = DialogResult.OK;
        }

        private void directedButton_Click(object sender, EventArgs e)
        {
            Directed = true;
            DialogResult = DialogResult.OK;
        }

        private void cancelButton_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.Cancel;
        }
    }
}

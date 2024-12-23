using System.Windows.Forms;

namespace GraphPaper
{
    public partial class MetadatumHeaderControl : UserControl
    {
        public MetadatumHeaderControl()
        {
            InitializeComponent();
        }

        public void AlignTextBox(int offset)
        {
            valueLabel.Left = keyLabel.Left + offset;
        }

        public int KeyLabelWidth
        {
            get { return keyLabel.Width; }
        }
    }
}

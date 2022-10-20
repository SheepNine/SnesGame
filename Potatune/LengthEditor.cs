using System;
using System.Windows.Forms;

namespace Potatune
{
    public partial class LengthEditor : Form
    {
        public LengthEditor()
        {
            InitializeComponent();
        }

        public int? Length
        {
            get
            {
                int result;
                if (int.TryParse(lengthTextBox.Text, out result) && result > 0)
                    return result;
                return null;
            }
            set
            {
                if (!value.HasValue)
                    throw new ArgumentNullException("value");
                if (value.Value <= 0)
                    throw new ArgumentException("value");

                lengthTextBox.Text = value.Value.ToString();
            }
        }

        private void lengthTextBox_TextChanged(object sender, EventArgs e)
        {
            var length = Length;
            if (length.HasValue)
                timeLabel.Text = NotePositionToString(length.Value);
            else
                timeLabel.Text = string.Empty;
        }

        public static string NotePositionToString(int position)
        {
            var minutes = position / 3000;
            var secondTics = position % 3000;

            var mString = minutes.ToString();
            if (mString.Length == 1)
                mString = "0" + mString;

            var sString = (secondTics / 50.0).ToString("F2");
            if (sString.Length == 4)
                sString = "0" + sString;

            return string.Format("{0}:{1}", mString, sString);
        }
    }
}

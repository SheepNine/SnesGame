using System.Windows.Forms;

namespace Scribe
{
    class DoubleBufferedControl : Control
    {
        public DoubleBufferedControl()
        {
            SetStyle(ControlStyles.AllPaintingInWmPaint | ControlStyles.OptimizedDoubleBuffer | ControlStyles.UserPaint, true);
        }
    }
}

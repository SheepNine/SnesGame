using System.Drawing;

namespace GraphPaper
{
    class GraphPaperColors
    {
        public static Color HotTrackingPen { get { return Color.FromArgb(128, 128, 128); } }
        public static Color Paper { get { return Color.FromArgb(0, 64, 192); } }
        public static Color MinorGridLines { get { return Color.FromArgb(0, 48, 144); } }
        public static Color MajorGridLines { get { return Color.FromArgb(0, 32, 96); } }
        public static Color Edge { get { return Color.FromArgb(223, 223, 223); } }
        public static Color Vertex { get { return Color.FromArgb(255, 255, 255); } }
        public static Color SelectingPen { get { return Color.FromArgb(128, 255, 0); } }
        public static Color SelectedEdge { get { return Color.FromArgb(224, 160, 0); } }
        public static Color SelectedVertex { get { return Color.FromArgb(255, 192, 0); } }
        public static Color ErrorStatusText { get { return Color.FromArgb(255, 128, 128); } }
        public static Color StatusText { get { return Color.FromArgb(255, 255, 255); } }
        public static Color CreateObject { get { return Color.FromArgb(128, 128, 128); } }
        public static Color SelectionArea { get { return Color.FromArgb(224, 224, 255); } }
    }
}

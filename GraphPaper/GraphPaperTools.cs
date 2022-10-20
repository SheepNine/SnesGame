using System.Drawing;
using System.Media;
using System.Windows.Forms;

namespace GraphPaper
{
    class GraphPaperToolbox : Toolbox<IViewModel>
    {
        public static readonly IToolbox<IViewModel> Instance = new GraphPaperToolbox();

        private GraphPaperToolbox()
        {
            SetMissingClickTool(UnsupportedTool.Instance);
            SetMissingDragTool(UnsupportedTool.Instance);

            SetClickTool(Keys.None, MouseButtons.Left, new SelectSingleObjectTool());
            SetClickTool(Keys.Shift, MouseButtons.Left, new AppendSingleObjectToSelectionTool());
            SetClickTool(Keys.Control, MouseButtons.Left, new ToggleSingleObjectSelectionTool());
            SetDragTool(Keys.None, MouseButtons.Left, new SelectObjectsInAreaTool());
            SetDragTool(Keys.Shift, MouseButtons.Left, new AppendObjectsInAreaToSelectionTool());
            SetClickTool(Keys.Alt, MouseButtons.Left, new CenterViewTool());
            SetDragTool(Keys.Alt, MouseButtons.Left, new AdjustViewTool());
            SetDragTool(Keys.None, MouseButtons.Right, new MoveSelectedTool());
            SetClickTool(Keys.Control, MouseButtons.Right, new AddVertexTool());
            SetDragTool(Keys.Control, MouseButtons.Right, new AddEdgeTool());
            SetClickTool(Keys.Control | Keys.Shift, MouseButtons.Right, new SplitEdgeTool());
            SetDragTool(Keys.None, MouseButtons.Middle, new PanTool());
        }
    }

    class UnsupportedTool : IClickTool<IViewModel>, IDragTool<IViewModel>
    {
        public static readonly UnsupportedTool Instance = new UnsupportedTool();

        private UnsupportedTool() { }

        public void Gestured(IViewModel context, Point p)
        {
            SystemSounds.Beep.Play();
        }

        public void Gestured(IViewModel context, Point start, Point end)
        {
            SystemSounds.Beep.Play();
        }

        public void Gesturing(IViewModel context, Point p)
        {
            context.ShowNoOperationFeedback();
        }

        public void Gesturing(IViewModel context, Point start, Point current)
        {
            context.ShowNoOperationFeedback();
        }

        public string HelpText
        {
            get { return "gesture not supported"; }
        }
    }

    class SelectSingleObjectTool : IClickTool<IViewModel>
    {
        public void Gestured(IViewModel context, Point p)
        {
            context.SelectSingleObject(p);
        }

        public void Gesturing(IViewModel context, Point p)
        {
            context.PreviewSelectSingleObject(p);
        }

        public string HelpText
        {
            get { return "select nearest object"; }
        }
    }

    class AppendSingleObjectToSelectionTool : IClickTool<IViewModel>
    {
        public void Gestured(IViewModel context, Point p)
        {
            context.AppendSingleObjectToSelection(p);
        }

        public void Gesturing(IViewModel context, Point p)
        {
            context.PreviewAppendSingleObjectToSelection(p);
        }

        public string HelpText
        {
            get { return "add nearest object to selection"; }
        }
    }

    class ToggleSingleObjectSelectionTool : IClickTool<IViewModel>
    {
        public void Gestured(IViewModel context, Point p)
        {
            context.ToggleSingleObjectSelection(p);
        }

        public void Gesturing(IViewModel context, Point p)
        {
            context.PreviewToggleSingleObjectSelection(p);
        }

        public string HelpText
        {
            get { return "toggle selection of nearest object"; }
        }
    }

    class CenterViewTool : IClickTool<IViewModel>
    {
        public void Gestured(IViewModel context, Point p)
        {
            context.CenterView(p);
        }

        public void Gesturing(IViewModel context, Point p)
        {
            context.PreviewCenterView(p);
        }

        public string HelpText
        {
            get { return "center view at selected point"; }
        }
    }

    class AddVertexTool : IClickTool<IViewModel>
    {
        public void Gestured(IViewModel context, Point p)
        {
            context.AddVertex(p);
        }

        public void Gesturing(IViewModel context, Point p)
        {
            context.PreviewAddVertex(p);
        }

        public string HelpText
        {
            get { return "create vertex"; }
        }
    }

    class SplitEdgeTool : IClickTool<IViewModel>
    {
        public void Gestured(IViewModel context, Point p)
        {
            context.SplitEdge(p);
        }

        public void Gesturing(IViewModel context, Point p)
        {
            context.PreviewSplitEdge(p);
        }

        public string HelpText
        {
            get { return "split edge"; }
        }
    }

    class AdjustViewTool : IDragTool<IViewModel>
    {
        public void Gestured(IViewModel context, Point start, Point end)
        {
            context.AdjustViewExtents(start.RectangleTo(end));
        }

        public void Gesturing(IViewModel context, Point start, Point current)
        {
            context.PreviewAdjustViewExtents(start.RectangleTo(current));
        }

        public string HelpText
        {
            get { return "zoom to selected area"; }
        }
    }

    class AddEdgeTool : IDragTool<IViewModel>
    {
        public void Gestured(IViewModel context, Point start, Point end)
        {
            context.AddEdge(start, end);
        }

        public void Gesturing(IViewModel context, Point start, Point current)
        {
            context.PreviewAddEdge(start, current);
        }

        public string HelpText
        {
            get { return "create edge"; }
        }
    }

    class SelectObjectsInAreaTool : IDragTool<IViewModel>
    {
        public void Gestured(IViewModel context, Point start, Point end)
        {
            context.SelectObjectsInArea(start.RectangleTo(end));
        }

        public void Gesturing(IViewModel context, Point start, Point current)
        {
            context.PreviewSelectObjectsInArea(start.RectangleTo(current));
        }

        public string HelpText
        {
            get { return "select objects in area"; }
        }
    }

    class AppendObjectsInAreaToSelectionTool : IDragTool<IViewModel>
    {
        public void Gestured(IViewModel context, Point start, Point end)
        {
            context.AppendObjectsInAreaToSelection(start.RectangleTo(end));
        }

        public void Gesturing(IViewModel context, Point start, Point current)
        {
            context.PreviewAppendObjectsInAreaToSelection(start.RectangleTo(current));
        }

        public string HelpText
        {
            get { return "append objects in area to selection"; }
        }
    }

    class MoveSelectedTool : IDragTool<IViewModel>
    {
        public void Gestured(IViewModel context, Point start, Point end)
        {
            context.MoveSelected(start, end);
        }

        public void Gesturing(IViewModel context, Point start, Point current)
        {
            context.PreviewMoveSelected(start, current);
        }

        public string HelpText
        {
            get { return "move selected objects"; }
        }
    }

    class PanTool : IDragTool<IViewModel>
    {
        public void Gestured(IViewModel context, Point start, Point end)
        {
            context.Pan(start, end);
        }

        public void Gesturing(IViewModel context, Point start, Point current)
        {
            context.PreviewPan(start, current);
        }

        public string HelpText
        {
            get { return "pan camera"; }
        }
    }
}

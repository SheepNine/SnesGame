using System;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Globalization;
using UnaryHeap.DataType;

namespace GraphPaper
{
    interface IFeedback : IEquatable<IFeedback>
    {
        void Render(Screen screen);
    }

    class NullFeedback : IFeedback
    {
        public bool Equals(IFeedback other)
        {
            var castOther = other as NullFeedback;
            return castOther != null;
        }

        public void Render(Screen screen)
        {
        }
    }

    class UnsupportedFeedback : IFeedback
    {
        public bool Equals(IFeedback other)
        {
            var castOther = other as UnsupportedFeedback;
            return castOther != null;
        }

        public void Render(Screen screen)
        {
            using (var font = new Font(FontFamily.GenericSansSerif, 16.0f))
                screen.DrawStatusText("Unsupported Operation", font,
                    GraphPaperColors.ErrorStatusText);
        }
    }

    class HoverFeedback : IFeedback
    {
        Point2D feedbackPoint;

        public HoverFeedback(Point2D feedbackPoint)
        {
            this.feedbackPoint = feedbackPoint;
        }

        public bool Equals(IFeedback other)
        {
            var castOther = other as HoverFeedback;

            if (null == castOther)
                return false;

            return this.feedbackPoint.Equals(castOther.feedbackPoint);
        }

        public void Render(Screen screen)
        {
            using (var pen = new Pen(GraphPaperColors.HotTrackingPen, 2.0f))
                screen.DrawCircle(pen, feedbackPoint, 4.0f);

            var display = string.Format(CultureInfo.InvariantCulture,
                "\r\nX: {0}\r\nY: {1}",
                (double)feedbackPoint.X, (double)feedbackPoint.Y);

            using (var font = new Font(FontFamily.GenericSansSerif, 16.0f))
                screen.DrawStatusText(display, font, GraphPaperColors.StatusText);
        }
    }

    class AddVertexFeedback : IFeedback
    {
        Point2D vertexPoint;

        public AddVertexFeedback(Point2D vertexPoint)
        {
            this.vertexPoint = vertexPoint;
        }

        public bool Equals(IFeedback other)
        {
            var castOther = other as AddVertexFeedback;

            if (null == castOther)
                return false;

            return this.vertexPoint.Equals(castOther.vertexPoint);
        }

        public void Render(Screen screen)
        {
            using (var brush = new SolidBrush(GraphPaperColors.CreateObject))
                screen.FillCircle(brush, vertexPoint, 5.0f);

            var display = string.Format(CultureInfo.InvariantCulture, 
                "Add Vertex\r\nX: {0}\r\nY: {1}\r\n",
                (double)vertexPoint.X, (double)vertexPoint.Y);

            using (var font = new Font(FontFamily.GenericSansSerif, 16.0f))
                screen.DrawStatusText(display, font, GraphPaperColors.StatusText);
        }
    }

    class AddEdgeFeedback : IFeedback
    {
        Point2D startPoint;
        Point2D endPoint;

        public AddEdgeFeedback(Point2D startPoint, Point2D endPoint)
        {
            this.startPoint = startPoint;
            this.endPoint = endPoint;
        }

        public bool Equals(IFeedback other)
        {
            var castObj = other as AddEdgeFeedback;

            if (null == castObj)
                return false;

            return this.startPoint.Equals(castObj.startPoint) &&
                this.endPoint.Equals(castObj.endPoint);
        }

        public void Render(Screen screen)
        {
            using (var brush = new SolidBrush(GraphPaperColors.CreateObject))
            {
                screen.FillCircle(brush, startPoint, 5.0f);
                screen.FillCircle(brush, endPoint, 5.0f);
            }

            using (var pen = new Pen(GraphPaperColors.CreateObject, 4.0f))
            {
                screen.DrawLine(pen, startPoint, endPoint);
                screen.DrawTick(pen, startPoint, endPoint);
            }

            var display = string.Format(CultureInfo.InvariantCulture,
                "Add Edge\r\nX1: {0}\r\nY1: {1}\r\nX2: {2}\r\nY2: {3}",
                (double)startPoint.X, (double)startPoint.Y,
                (double)endPoint.X, (double)endPoint.Y);

            using (var font = new Font(FontFamily.GenericSansSerif, 16.0f))
                screen.DrawStatusText(display, font, GraphPaperColors.StatusText);
        }
    }

    class SplitEdgeFeedback : IFeedback
    {
        Point2D splitPoint;

        public SplitEdgeFeedback(Point2D startPoint, Point2D endPoint)
        {
            splitPoint = new Point2D(
                (startPoint.X + endPoint.X) / 2,
                (startPoint.Y + endPoint.Y) / 2);
        }

        public bool Equals(IFeedback other)
        {
            var castObj = other as SplitEdgeFeedback;

            if (null == castObj)
                return false;

            return this.splitPoint.Equals(castObj.splitPoint);
        }

        public void Render(Screen screen)
        {
            using (var brush = new SolidBrush(GraphPaperColors.CreateObject))
                screen.FillCircle(brush, splitPoint, 5.0f);

            var display = string.Format(CultureInfo.InvariantCulture,
                "Split Edge\r\nX: {0}\r\nY: {1}",
                (double)splitPoint.X, (double)splitPoint.Y);

            using (var font = new Font(FontFamily.GenericSansSerif, 16.0f))
                screen.DrawStatusText(display, font, GraphPaperColors.StatusText);
        }
    }

    class ErrorFeedback : IFeedback
    {
        string message;

        public ErrorFeedback(string message)
        {
            this.message = message;
        }

        public bool Equals(IFeedback other)
        {
            var castObj = other as ErrorFeedback;

            if (null == castObj)
                return false;

            return this.message.Equals(castObj.message, StringComparison.Ordinal);
        }

        public void Render(Screen screen)
        {
            using (var font = new Font(FontFamily.GenericSansSerif, 16.0f))
                screen.DrawStatusText(message, font, GraphPaperColors.StatusText);
        }
    }

    class MessageFeedback : IFeedback
    {
        string message;

        public MessageFeedback(string message)
        {
            this.message = message;
        }

        public bool Equals(IFeedback other)
        {
            var castObj = other as MessageFeedback;

            if (null == castObj)
                return false;

            return this.message.Equals(castObj.message, StringComparison.Ordinal);
        }

        public void Render(Screen screen)
        {
            using (var font = new Font(FontFamily.GenericSansSerif, 16.0f))
                screen.DrawStatusText(message, font, GraphPaperColors.StatusText);
        }
    }

    class AdjustViewExtentsFeedback : IFeedback
    {
        Orthotope2D bounds;

        public AdjustViewExtentsFeedback(Orthotope2D bounds)
        {
            this.bounds = bounds;
        }

        public bool Equals(IFeedback other)
        {
            var castObj = other as AdjustViewExtentsFeedback;

            if (null == castObj)
                return false;

            return
                this.bounds.X.Min.Equals(castObj.bounds.X.Min) &&
                this.bounds.Y.Min.Equals(castObj.bounds.Y.Min) &&
                this.bounds.X.Max.Equals(castObj.bounds.X.Max) &&
                this.bounds.Y.Max.Equals(castObj.bounds.Y.Max);
        }

        public void Render(Screen screen)
        {
            using (var brush = new SolidBrush(Color.FromArgb(32, GraphPaperColors.SelectionArea)))
                screen.FillRectangle(brush, bounds);
            using (var pen = new Pen(GraphPaperColors.SelectionArea, 2.0f))
            {
                pen.DashStyle = DashStyle.Dash;
                screen.DrawRectangle(pen, bounds);
            }

            var display = string.Format(CultureInfo.InvariantCulture,
                "Adjust View Extents\r\nX: {0:F2} to {1:F2}\r\nY: {2:F2} to {3:F2}",
                (double)bounds.X.Min, (double)bounds.X.Max,
                (double)bounds.Y.Min, (double)bounds.Y.Max);

            using (var font = new Font(FontFamily.GenericSansSerif, 16.0f))
                screen.DrawStatusText(display, font, GraphPaperColors.StatusText);
        }
    }

    class SelectObjectsFeedback : IFeedback
    {
        Orthotope2D bounds;

        public SelectObjectsFeedback(Orthotope2D bounds)
        {
            this.bounds = bounds;
        }

        public bool Equals(IFeedback other)
        {
            var castObj = other as SelectObjectsFeedback;

            if (null == castObj)
                return false;

            return
                this.bounds.X.Min.Equals(castObj.bounds.X.Min) &&
                this.bounds.Y.Min.Equals(castObj.bounds.Y.Min) &&
                this.bounds.X.Max.Equals(castObj.bounds.X.Max) &&
                this.bounds.Y.Max.Equals(castObj.bounds.Y.Max);
        }

        public void Render(Screen screen)
        {
            using (var brush = new SolidBrush(Color.FromArgb(32, GraphPaperColors.SelectionArea)))
                screen.FillRectangle(brush, bounds);

            using (var pen = new Pen(GraphPaperColors.SelectionArea, 2.0f))
                screen.DrawRectangle(pen, bounds);

            var display = string.Format(CultureInfo.InvariantCulture,
                "Select Objects\r\nX: {0:F2} to {1:F2}\r\nY: {2:F2} to {3:F2}",
                (double)bounds.X.Min, (double)bounds.X.Max,
                (double)bounds.Y.Min, (double)bounds.Y.Max);

            using (var font = new Font(FontFamily.GenericSansSerif, 16.0f))
                screen.DrawStatusText(display, font, GraphPaperColors.StatusText);
        }
    }

    class AppendSelectionFeedback : IFeedback
    {
        Orthotope2D bounds;

        public AppendSelectionFeedback(Orthotope2D bounds)
        {
            this.bounds = bounds;
        }

        public bool Equals(IFeedback other)
        {
            var castObj = other as AppendSelectionFeedback;

            if (null == castObj)
                return false;

            return
                this.bounds.X.Min.Equals(castObj.bounds.X.Min) &&
                this.bounds.Y.Min.Equals(castObj.bounds.Y.Min) &&
                this.bounds.X.Max.Equals(castObj.bounds.X.Max) &&
                this.bounds.Y.Max.Equals(castObj.bounds.Y.Max);
        }

        public void Render(Screen screen)
        {
            using (var brush = new SolidBrush(Color.FromArgb(32, GraphPaperColors.SelectionArea)))
                screen.FillRectangle(brush, bounds);

            using (var pen = new Pen(GraphPaperColors.SelectionArea, 2.0f))
                screen.DrawRectangle(pen, bounds);

            var display = string.Format(CultureInfo.InvariantCulture,
                "Append Objects to Selection\r\nX: {0:F2} to {1:F2}\r\nY: {2:F2} to {3:F2}",
                (double)bounds.X.Min, (double)bounds.X.Max,
                (double)bounds.Y.Min, (double)bounds.Y.Max);

            using (var font = new Font(FontFamily.GenericSansSerif, 16.0f))
                screen.DrawStatusText(display, font, GraphPaperColors.StatusText);
        }
    }

    class CenterViewFeedback : IFeedback
    {
        Point2D newCenterPoint;

        public CenterViewFeedback(Point2D newCenterPoint)
        {
            this.newCenterPoint = newCenterPoint;
        }

        public bool Equals(IFeedback other)
        {
            var castObj = other as CenterViewFeedback;

            if (null == castObj)
                return false;

            return this.newCenterPoint.Equals(castObj.newCenterPoint);
        }

        public void Render(Screen screen)
        {
            var extents = screen.ModelExtents.CenteredAt(newCenterPoint);

            using (var brush = new SolidBrush(Color.FromArgb(32, GraphPaperColors.SelectionArea)))
                screen.FillRectangle(brush, extents);
            using (var pen = new Pen(GraphPaperColors.SelectionArea, 2.0f))
            {
                pen.DashStyle = DashStyle.Dash;
                screen.DrawRectangle(pen, extents);
            }

            var display = string.Format(CultureInfo.InvariantCulture, 
                "Center View\r\nX: {0:F2}\r\nY: {1:F2}\r\n",
                (double)newCenterPoint.X, (double)newCenterPoint.Y);

            using (var font = new Font(FontFamily.GenericSansSerif, 16.0f))
                screen.DrawStatusText(display, font, GraphPaperColors.StatusText);
        }
    }

    class SelectPointFeedback : IFeedback
    {
        Point2D candidatePoint;

        public SelectPointFeedback(Point2D candidatePoint)
        {
            this.candidatePoint = candidatePoint;
        }

        public bool Equals(IFeedback other)
        {
            var castObj = other as SelectPointFeedback;

            if (null == castObj)
                return false;

            return this.candidatePoint.Equals(castObj.candidatePoint);
        }

        public void Render(Screen screen)
        {
            using (var pen = new Pen(GraphPaperColors.SelectingPen, 2.0f))
                screen.DrawCircle(pen, candidatePoint, 6.0f);
        }
    }

    class SelectEdgeFeedback : IFeedback
    {
        Point2D startPoint;
        Point2D endPoint;

        public SelectEdgeFeedback(Point2D startPoint, Point2D endPoint)
        {
            this.startPoint = startPoint;
            this.endPoint = endPoint;
        }

        public bool Equals(IFeedback other)
        {
            var castObj = other as SelectEdgeFeedback;

            if (null == castObj)
                return false;

            return this.startPoint.Equals(castObj.startPoint) &&
                this.endPoint.Equals(castObj.endPoint);
        }

        public void Render(Screen screen)
        {
            using (var pen = new Pen(GraphPaperColors.SelectingPen, 2.0f))
            {
                screen.DrawLine(pen, startPoint, endPoint);
            }
        }
    }
}

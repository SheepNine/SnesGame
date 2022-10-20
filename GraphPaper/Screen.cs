using System;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Drawing.Text;
using UnaryHeap.DataType;

namespace GraphPaper
{
    class Screen : IDisposable
    {
        Graphics g;
        GraphicsState gState;
        ModelViewTransform mvTransform;

        public Screen(Graphics g, ModelViewTransform mvTransform, int panX, int panY)
        {
            this.g = g;
            this.mvTransform = mvTransform.Clone() as ModelViewTransform;
            this.mvTransform.PanView(panX, panY);
            PushGraphicsState(g);
        }

        private void PushGraphicsState(Graphics g)
        {
            gState = g.Save();
            g.SmoothingMode = SmoothingMode.HighQuality;
            g.TextRenderingHint = TextRenderingHint.ClearTypeGridFit;
        }

        public void Dispose()
        {
            g.Restore(gState);
        }

        public Orthotope2D ModelExtents
        {
            get { return mvTransform.ModelExtents; }
        }

        public void RenderGrid(Rational minorGridSize)
        {
            g.SmoothingMode = SmoothingMode.None;
            const int majorGridMultiple = 4;
            var extents = mvTransform.ModelExtents;

            using (var pen = new Pen(GraphPaperColors.MinorGridLines))
            {
                for (var x = (extents.X.Min / minorGridSize).Floor;
                    x <= (extents.X.Max / minorGridSize).Ceiling; x += 1)
                {
                    if (0 != x.Numerator % majorGridMultiple)
                        DrawLine(pen, new Point2D(minorGridSize * x, extents.Y.Min),
                            new Point2D(minorGridSize * x, extents.Y.Max));
                }

                for (var y = (extents.Y.Min / minorGridSize).Floor;
                    y <= (extents.Y.Max / minorGridSize).Ceiling; y += 1)
                {
                    if (0 != y.Numerator % majorGridMultiple)
                        DrawLine(pen, new Point2D(extents.X.Min, minorGridSize * y),
                            new Point2D(extents.X.Max, minorGridSize * y));
                }
            }

            using (var pen = new Pen(GraphPaperColors.MajorGridLines))
            {
                var majorGridSize = minorGridSize * majorGridMultiple;

                for (var x = (extents.X.Min / majorGridSize).Floor;
                    x <= (extents.X.Max / majorGridSize).Ceiling; x += 1)
                {
                    DrawLine(pen, new Point2D(majorGridSize * x, extents.Y.Min),
                        new Point2D(majorGridSize * x, extents.Y.Max));
                }

                for (var y = (extents.Y.Min / majorGridSize).Floor;
                    y <= (extents.Y.Max / majorGridSize).Ceiling; y += 1)
                {
                    DrawLine(pen, new Point2D(extents.X.Min, majorGridSize * y),
                        new Point2D(extents.X.Max, majorGridSize * y));
                }
            }

            g.SmoothingMode = SmoothingMode.HighQuality;
        }

        public void Render(ReadOnlyGraph2D graph, GraphObjectSelection selection,
            IVertexOffset offset)
        {
            using (var pen = new Pen(GraphPaperColors.Edge, 3.0f))
            using (var highlightPen = new Pen(GraphPaperColors.SelectedEdge, 4.0f))
                foreach (var edge in graph.Edges)
                {
                    var selected = selection.IsEdgeSelected(edge.Item1, edge.Item2);
                    var activePen = selected ? highlightPen : pen;

                    DrawLine(activePen,
                        offset.GetOffsetVertex(edge.Item1),
                        offset.GetOffsetVertex(edge.Item2));

                    if (graph.IsDirected)
                        DrawTick(activePen,
                            offset.GetOffsetVertex(edge.Item1),
                            offset.GetOffsetVertex(edge.Item2));
                }

            using (var brush = new SolidBrush(GraphPaperColors.Vertex))
            using (var highlightBrush = new SolidBrush(GraphPaperColors.SelectedVertex))
                foreach (var vertex in graph.Vertices)
                {
                    var selected = selection.IsVertexSelected(vertex);

                    FillCircle(selected ? highlightBrush : brush,
                        offset.GetOffsetVertex(vertex), selected ? 5.0f : 4.0f);
                }
        }

        public void DrawTick(Pen pen, Point2D modelStart, Point2D modelEnd)
        {
            if (modelStart.Equals(modelEnd))
                return;

            var tickQuadrance = mvTransform.Quadrance(new Point(0, 0), new Point(10, 0));

            var center = new Point2D(
                (modelStart.X + modelEnd.X) / 2,
                (modelStart.Y + modelEnd.Y) / 2);
            var delta = new Point2D(
                modelStart.Y - modelEnd.Y,
                modelEnd.X - modelStart.X);
            var deltaQuadrance = delta.X.Squared + delta.Y.Squared;

            var scale = Math.Sqrt((double)(tickQuadrance / deltaQuadrance));
            var rationalScale = new Rational((int)(1000 * scale), 1000);

            if (0 != rationalScale)
            {
                var offset = new Point2D(
                    center.X + rationalScale * delta.X,
                    center.Y + rationalScale * delta.Y);

                DrawLine(pen, center, offset);
            }
        }

        public void DrawCircle(Pen pen, Point2D modelCoords, float radius)
        {
            g.DrawEllipse(pen, CircleBounds(
                mvTransform.ViewFromModel(modelCoords), radius));
        }

        public void FillCircle(Brush b, Point2D modelCoords, float radius)
        {
            var viewCoords = mvTransform.ViewFromModel(modelCoords);

            g.FillEllipse(b,
                (float)viewCoords.X - radius, (float)viewCoords.Y - radius,
                radius * 2.0f, radius * 2.0f);
        }

        public void DrawLine(Pen p, Point2D modelStart, Point2D modelEnd)
        {
            if (modelStart.Equals(modelEnd))
                return;

            var viewStart = mvTransform.ViewFromModel(modelStart);
            var viewEnd = mvTransform.ViewFromModel(modelEnd);

            g.DrawLine(p,
                (float)viewStart.X, (float)viewStart.Y,
                (float)viewEnd.X, (float)viewEnd.Y);
        }

        public void DrawRectangle(Pen p, Orthotope2D modelArea)
        {
            var viewArea = mvTransform.ViewFromModel(modelArea);
            g.DrawRectangle(p, viewArea.X, viewArea.Y, viewArea.Width, viewArea.Height);
        }

        public void FillRectangle(Brush b, Orthotope2D modelArea)
        {
            var viewArea = mvTransform.ViewFromModel(modelArea);
            g.FillRectangle(b, viewArea.X, viewArea.Y, viewArea.Width, viewArea.Height);
        }

        static RectangleF CircleBounds(Point2D p, float radius)
        {
            var pX = (float)p.X;
            var pY = (float)p.Y;

            return new RectangleF(
                pX - radius, pY - radius, radius * 2.0f, radius * 2.0f);
        }

        public void DrawStatusText(string s, Font font, Color color)
        {
            using (var brush = new SolidBrush(color))
                g.DrawString(s, font, brush, new Point(0, 0));
        }

        public void DrawString(string s, Font font, Brush brush,
            Point2D modelCoords, TextOffset offset)
        {
            var p = mvTransform.ViewFromModel(modelCoords);

            g.DrawString(s, font, brush, (float)p.X, (float)p.Y,
                StringFormatFromTextOffset(offset));
        }

        static StringFormat StringFormatFromTextOffset(TextOffset offset)
        {
            var format = new StringFormat();

            switch (offset)
            {
                case TextOffset.NorthEast:
                    format.LineAlignment = StringAlignment.Far;
                    format.Alignment = StringAlignment.Near;
                    break;
                case TextOffset.NorthWest:
                    format.LineAlignment = StringAlignment.Far;
                    format.Alignment = StringAlignment.Far;
                    break;
                case TextOffset.SouthEast:
                    format.LineAlignment = StringAlignment.Near;
                    format.Alignment = StringAlignment.Near;
                    break;
                case TextOffset.SouthWest:
                    format.LineAlignment = StringAlignment.Near;
                    format.Alignment = StringAlignment.Far;
                    break;
            }

            return format;
        }
    }

    enum TextOffset
    {
        NorthEast,
        NorthWest,
        SouthEast,
        SouthWest,
    }
}

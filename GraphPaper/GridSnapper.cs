using UnaryHeap.DataType;

namespace GraphPaper
{
    class GridSnapper
    {
        public Rational GridSize { get; set; }

        public GridSnapper()
        {
            GridSize = 1;
        }

        public Point2D Snap(Point2D p)
        {
            return new Point2D(
                GridSize * (p.X / GridSize).Rounded,
                GridSize * (p.Y / GridSize).Rounded);
        }
    }
}

using System;
using System.Collections.Generic;
using UnaryHeap.DataType;

namespace GraphPaper
{
    interface IVertexOffset : IEquatable<IVertexOffset>
    {
        Point2D GetOffsetVertex(Point2D source);
    }

    class VertexOffset : IVertexOffset
    {
        Rational dX;
        Rational dY;
        SortedSet<Point2D> vertices = new SortedSet<Point2D>(new Point2DComparer());

        public VertexOffset(Rational dX, Rational dY, IEnumerable<Point2D> vertices)
        {
            this.dX = dX;
            this.dY = dY;
            this.vertices.UnionWith(vertices);
        }

        public bool Equals(IVertexOffset other)
        {
            var castOther = other as VertexOffset;

            if (null == castOther)
                return false;

            return this.dX.Equals(castOther.dX) && this.dY.Equals(castOther.dY);
        }

        public Point2D GetOffsetVertex(Point2D source)
        {
            if (vertices.Contains(source))
                return new Point2D(source.X + dX, source.Y + dY);
            else
                return source;
        }
    }

    class IdentityOffset : IVertexOffset
    {
        public static readonly IdentityOffset Instance = new IdentityOffset();

        private IdentityOffset() { }

        public bool Equals(IVertexOffset other)
        {
            var castOther = other as IdentityOffset;
            return castOther != null;
        }

        public Point2D GetOffsetVertex(Point2D source)
        {
            return source;
        }
    }
}

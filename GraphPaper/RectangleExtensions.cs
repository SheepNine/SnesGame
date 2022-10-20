using System;
using System.Drawing;

namespace GraphPaper
{
    static class RectangleExtensions
    {
        public static Rectangle RectangleTo(this Point a, Point b)
        {
            return Rectangle.FromLTRB(
                Math.Min(a.X, b.X),
                Math.Min(a.Y, b.Y),
                Math.Max(a.X, b.X),
                Math.Max(a.Y, b.Y));
        }
    }
}

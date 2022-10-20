using System;
using System.Collections.Generic;
using System.Linq;
using UnaryHeap.DataType;

namespace GraphPaper
{
    class GraphObjectSelection
    {
        public event EventHandler SelectionChanged;

        protected void OnSelectionChanged()
        {
            if (null != SelectionChanged)
                SelectionChanged(this, EventArgs.Empty);
        }

        IComparer<Point2D> comparer;
        SortedSet<Point2D> selectedVertices;
        SortedDictionary<Point2D, SortedSet<Point2D>> selectedEdges;

        public GraphObjectSelection()
        {
            comparer = new Point2DComparer();
            selectedVertices = new SortedSet<Point2D>(comparer);
            selectedEdges = new SortedDictionary<Point2D, SortedSet<Point2D>>(comparer);
        }

        public IEnumerable<Point2D> Vertices
        {
            get { return selectedVertices; }
        }

        public int NumVertices
        {
            get { return selectedVertices.Count; }
        }

        public IEnumerable<Tuple<Point2D, Point2D>> Edges
        {
            get
            {
                return selectedEdges.SelectMany(
                    start => start.Value.Select(
                        end => Tuple.Create(start.Key, end)));
            }
        }

        public int NumEdges
        {
            get { return selectedEdges.Count; }
        }

        void SelectVertex(Point2D vertex)
        {
            selectedVertices.Add(vertex);
            OnSelectionChanged();
        }

        void DeselectVertex(Point2D vertex)
        {
            selectedVertices.Remove(vertex);
            OnSelectionChanged();
        }

        void ToggleVertexSelection(Point2D vertex)
        {
            if (IsVertexSelected(vertex))
                DeselectVertex(vertex);
            else
                SelectVertex(vertex);
        }

        void SelectEdge(Point2D start, Point2D end)
        {
            if (false == selectedEdges.ContainsKey(start))
                selectedEdges.Add(start, new SortedSet<Point2D>(new Point2DComparer()));

            selectedEdges[start].Add(end);
            OnSelectionChanged();
        }

        void DeselectEdge(Point2D start, Point2D end)
        {
            selectedEdges[start].Remove(end);
            if (0 == selectedEdges[start].Count)
                selectedEdges.Remove(start);

            OnSelectionChanged();
        }

        void ToggleEdgeSelection(Point2D start, Point2D end)
        {
            if (IsEdgeSelected(start, end))
                DeselectEdge(start, end);
            else
                SelectEdge(start, end);
        }

        public bool IsEdgeSelected(Point2D start, Point2D end)
        {
            return selectedEdges.ContainsKey(start)
                && selectedEdges[start].Contains(end);
        }

        public bool IsVertexSelected(Point2D vertex)
        {
            return selectedVertices.Contains(vertex);
        }

        public void ClearSelection()
        {
            selectedVertices.Clear();
            selectedEdges.Clear();
            OnSelectionChanged();
        }

        public void SelectAll(ReadOnlyGraph2D currentModelState)
        {
            foreach (var vertex in currentModelState.Vertices)
                SelectVertex(vertex);
            foreach (var edge in currentModelState.Edges)
                SelectEdge(edge.Item1, edge.Item2);

            OnSelectionChanged();
        }

        public void SelectNearestObject(
            ReadOnlyGraph2D g, Point2D p, Rational quadranceCutoff)
        {
            ClearSelection();
            g.DoWithNearest(p, quadranceCutoff, SelectVertex, SelectEdge);
        }

        public void AppendNearestObjectToSelection(
            ReadOnlyGraph2D g, Point2D p, Rational quadranceCutoff)
        {
            g.DoWithNearest(p, quadranceCutoff, SelectVertex, SelectEdge);
        }

        public void ToggleSelectionOfNearestObject(
            ReadOnlyGraph2D g, Point2D p, Rational quadranceCutoff)
        {
            g.DoWithNearest(p, quadranceCutoff, ToggleVertexSelection, ToggleEdgeSelection);
        }

        public void SelectObjectsInArea(
            ReadOnlyGraph2D currentModelState, Orthotope2D area)
        {
            ClearSelection();
            SelectObjects(currentModelState, area);
        }

        public void AppendObjectsInAreaToSelection(
            ReadOnlyGraph2D currentModelState, Orthotope2D area)
        {
            SelectObjects(currentModelState, area);
        }

        void SelectObjects(ReadOnlyGraph2D currentModelState, Orthotope2D area)
        {
            foreach (var vertex in currentModelState.Vertices)
                if (area.Contains(vertex))
                    SelectVertex(vertex);

            foreach (var edge in currentModelState.Edges)
                if (area.Contains(edge.Item1) && area.Contains(edge.Item2))
                    SelectEdge(edge.Item1, edge.Item2);
        }

        public void TranslateValues(Rational dX, Rational dY)
        {
            var verts = Vertices.ToList();
            var edges = Edges.ToList();

            ClearSelection();

            foreach (var vert in verts)
                SelectVertex(new Point2D(vert.X + dX, vert.Y + dY));

            foreach (var edge in edges)
                SelectEdge(new Point2D(edge.Item1.X + dX, edge.Item1.Y + dY),
                    new Point2D(edge.Item2.X + dX, edge.Item2.Y + dY));
        }

        public SortedSet<Point2D> VerticesForMoveOperation()
        {
            var result = new SortedSet<Point2D>(new Point2DComparer());

            foreach (var vertex in Vertices)
                result.Add(vertex);

            foreach (var edge in Edges)
            {
                result.Add(edge.Item1);
                result.Add(edge.Item2);
            }

            return result;
        }
    }
}

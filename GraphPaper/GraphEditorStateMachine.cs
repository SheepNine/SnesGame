using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using UnaryHeap.DataType;
using UnaryHeap.Graph;
using UnaryHeap.GUI;

namespace GraphPaper
{
    class Graph2DCreateArgs
    {
        public bool Directed { get; private set; }

        public Graph2DCreateArgs(bool directed)
        {
            Directed = directed;
        }
    }

    class GraphEditorStateMachine : ModelEditorStateMachine<
        Graph2DCreateArgs, Graph2D, ReadOnlyGraph2D>
    {
        public GraphEditorStateMachine() : base(new Prompts())
        {
        }

        protected override Graph2D Clone(Graph2D instance)
        {
            return instance.Clone();
        }

        protected override Graph2D CreateEmptyModel(Graph2DCreateArgs args)
        {
            return new Graph2D(args.Directed);
        }

        protected override Graph2D ReadModelFromDisk(string fileName)
        {
            using (var stream = File.OpenText(fileName))
                return Graph2D.FromJson(stream);
        }

        protected override ReadOnlyGraph2D Wrap(Graph2D instance)
        {
            return new ReadOnlyGraph2D(instance);
        }

        protected override void WriteModelToDisk(Graph2D instance, string fileName)
        {
            using (var stream = File.CreateText(fileName))
                instance.ToJson(stream);
        }
    }

    class ReadOnlyGraph2D
    {
        Graph2D graph;

        public ReadOnlyGraph2D(Graph2D graph)
        {
            this.graph = graph;
        }

        public IEnumerable<Tuple<Point2D, Point2D>> Edges
        {
            get { return graph.Edges; }
        }

        public IEnumerable<Point2D> Vertices
        {
            get { return graph.Vertices; }
        }

        public bool IsDirected
        {
            get { return graph.IsDirected; }
        }

        public Orthotope2D Extents
        {
            get
            {
                if (0 == graph.NumVertices)
                    return new Orthotope2D(-5, -5, 5, 5);

                var result = Orthotope2D.FromPoints(graph.Vertices);

                if (0 == result.X.Size || 0 == result.Y.Size)
                    result = result.GetPadded(1);

                return result;
            }
        }

        public bool HasVertex(Point2D coordinates)
        {
            return graph.HasVertex(coordinates);
        }

        public bool HasEdge(Point2D start, Point2D end)
        {
            return graph.HasEdge(start, end);
        }

        public void DoWithNearest(Point2D p, Rational quadranceCutoff,
            Action<Point2D> vertexIsClosest, Action<Point2D, Point2D> edgeIsClosest)
        {
            var nearestVertex = FindNearestVertex(p, quadranceCutoff);
            var nearestEdge = FindNearestEdge(p, quadranceCutoff);

            if (null != nearestVertex)
                vertexIsClosest(nearestVertex);
            else if (null != nearestEdge)
                edgeIsClosest(nearestEdge.Item1, nearestEdge.Item2);
        }

        public Point2D FindNearestVertex(
            Point2D p, Rational quadranceCutoff)
        {
            Point2D result = null;
            Rational resultQuadrance = null;

            foreach (var vertex in Vertices)
            {
                var vertexQuadrance = Point2D.Quadrance(vertex, p);

                if (vertexQuadrance <= quadranceCutoff &&
                    (resultQuadrance == null || vertexQuadrance < resultQuadrance))
                {
                    result = vertex;
                    resultQuadrance = vertexQuadrance;
                }
            }

            return result;
        }

        public Tuple<Point2D, Point2D> FindNearestEdge(
            Point2D p, Rational quadranceCutoff)
        {
            Tuple<Point2D, Point2D> result = null;
            Rational resultQuadrance = null;

            foreach (var edge in Edges)
            {
                if (false == CanSelectEdge(edge, p))
                    continue;

                var edgeQuadrance = EdgeQuadrance(edge, p);

                if (edgeQuadrance <= quadranceCutoff &&
                    (resultQuadrance == null || edgeQuadrance < resultQuadrance))
                {
                    result = edge;
                    resultQuadrance = edgeQuadrance;
                }
            }

            return result;
        }

        static bool CanSelectEdge(Tuple<Point2D, Point2D> edge, Point2D point)
        {
            var vX = edge.Item2.X - edge.Item1.X;
            var vY = edge.Item2.Y - edge.Item1.Y;

            var s1 = vX * edge.Item1.X + vY * edge.Item1.Y;
            var sPoint = vX * point.X + vY * point.Y;
            var s2 = vX * edge.Item2.X + vY * edge.Item2.Y;

            var result = s2 >= sPoint && sPoint >= s1;

            return result;
        }

        static Rational EdgeQuadrance(Tuple<Point2D, Point2D> e, Point2D p)
        {
            return new Hyperplane2D(e.Item1, e.Item2).Quadrance(p);
        }

        public MetadataSet GetGraphMetadata()
        {
            return new MetadataSet(graph.GraphMetadata);
        }

        public MetadataSet GetVertexMetadata(GraphObjectSelection selection)
        {
            return new MetadataSet(selection.Vertices.Select(
                v => graph.GetVertexMetadata(v)));
        }

        public MetadataSet GetEdgeMetadata(GraphObjectSelection selection)
        {
            return new MetadataSet(selection.Edges.Select(
                e => graph.GetEdgeMetadata(e.Item1, e.Item2)));
        }
    }
}

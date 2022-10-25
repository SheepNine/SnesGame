using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using UnaryHeap.Graph;

namespace GraphPaper
{
    class Metadata
    {
        public static MetadataChange ViewAndEdit(MetadataSet input, string title)
        {
            var output = ShowUIForEdit(input, title);

            if (null == output)
                return null;

            return input.GetChangeTo(output);
        }

        static MetadataSet ShowUIForEdit(MetadataSet input, string title)
        {
            using (var dialog = new ViewEditMetadataDialog(input)
            {
                Text = title
            })
            {
                dialog.ShowDialog();

                if (DialogResult.OK != dialog.DialogResult)
                    return null;

                return dialog.GetOutputMetadataSet();
            }
        }
    }

    class MetadataSet
    {
        SortedDictionary<string, string> data;

        public MetadataSet(IReadOnlyDictionary<string, string> source)
            : this(new[] { source })
        {
        }

        public MetadataSet(IEnumerable<IReadOnlyDictionary<string, string>> sources)
        {
            foreach (var source in sources)
            {
                if (null == data)
                    InitializeData(source);
                else
                    AppendData(source);
            }
        }

        void InitializeData(IReadOnlyDictionary<string, string> source)
        {
            data = new SortedDictionary<string, string>();

            foreach (var entry in source)
                data.Add(entry.Key, entry.Value);
        }

        void AppendData(IReadOnlyDictionary<string, string> source)
        {
            foreach (var key in data.Keys.ToArray())
            {
                if (false == source.ContainsKey(key))
                    data[key] = null;
                else if (data[key] != source[key])
                    data[key] = null;
            }

            foreach (var key in source.Keys)
            {
                if (false == data.ContainsKey(key))
                    data.Add(key, null);
            }
        }

        public MetadataChange GetChangeTo(MetadataSet output)
        {
            var result = new MetadataChange();

            foreach (var key in this.data.Keys)
                if (false == output.data.ContainsKey(key))
                    result.RemoveKey(key);

            foreach (var keyValue in output.data)
                if (null != keyValue.Value)
                    result.SetKey(keyValue.Key, keyValue.Value);

            return result;
        }

        public override string ToString()
        {
            var result = new StringBuilder();

            foreach (var datum in data)
            {
                if (null == datum.Value)
                    result.AppendLine(string.Format(CultureInfo.InvariantCulture,
                        "{0}: <varies>", datum.Key));
                else
                    result.AppendLine(string.Format(CultureInfo.InvariantCulture,
                        "{0}: {1}", datum.Key, datum.Value));
            }

            return result.ToString();
        }

        public IEnumerable<KeyValuePair<string, string>> Data
        {
            get { return data; }
        }
    }

    class MetadataChange
    {
        List<string> keysToRemove = new List<string>();
        SortedDictionary<string, string> keysToSet = new SortedDictionary<string, string>();

        public void UpdateGraphMetadata(Graph2D graph)
        {
            foreach (var key in keysToRemove)
                graph.UnsetGraphMetadatum(key);

            foreach (var keyValue in keysToSet)
                graph.SetGraphMetadatum(keyValue.Key, keyValue.Value);
        }

        public void UpdateVertexMetadata(Graph2D graph, GraphObjectSelection selection)
        {
            foreach (var vertex in selection.Vertices)
            {
                foreach (var key in keysToRemove)
                    graph.UnsetVertexMetadatum(vertex, key);

                foreach (var keyValue in keysToSet)
                    graph.SetVertexMetadatum(vertex, keyValue.Key, keyValue.Value);
            }
        }

        public void UpdateEdgeMetadata(Graph2D graph, GraphObjectSelection selection)
        {
            foreach (var edge in selection.Edges)
            {
                foreach (var key in keysToRemove)
                    graph.UnsetEdgeMetadatum(edge.Item1, edge.Item2, key);

                foreach (var keyValue in keysToSet)
                    graph.SetEdgeMetadatum(edge.Item1, edge.Item2,
                        keyValue.Key, keyValue.Value);
            }
        }

        public void SetKey(string key, string value)
        {
            if (Graph2D.IsReservedMetadataKey(key))
                return;

            keysToSet.Add(key, value);
        }

        public void RemoveKey(string key)
        {
            if (Graph2D.IsReservedMetadataKey(key))
                return;

            keysToRemove.Add(key);
        }
    }
}

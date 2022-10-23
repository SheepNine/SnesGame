using System;
using System.IO;
using UnaryHeap.Mosaic;

namespace SnesGame.ResourceCompiler.Pipelines.WideBackground
{
    public class WideBackgroundEntry : ManifestEntry
    {
        public const string Tag = "wide background";

        public string SourcePath { get; private set; }
        public int LayerCount { get; private set; }

        public WideBackgroundEntry(string id, string sourcePath, int layerCount) : base(Tag, id)
        {
            SourcePath = sourcePath;
            LayerCount = layerCount;

            if (!File.Exists(sourcePath))
                throw new FileNotFoundException(
                    string.Format("Source for wide background '{0}' not found", id), sourcePath);
        }

        public TileArrangement LoadSourceTileArrangement()
        {
            try
            {
                using (var source = File.OpenRead(SourcePath))
                    return TileArrangement.Deserialize(source);
            }
            catch (Exception ex)
            {
                throw new InvalidDataException(
                    string.Format("Error loading tile arrangement of wide background '{0}'", ID),
                    ex);
            }
        }
    }
}

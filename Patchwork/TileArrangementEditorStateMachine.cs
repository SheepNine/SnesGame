using System.Drawing;
using System.IO;
using UnaryHeap.GUI;
using UnaryHeap.Mosaic;

namespace Patchwork
{
    public class TileArrangementCreateArgs
    {
        public int TileCountX { get; private set; }
        public int TileCountY { get; private set; }

        public TileArrangementCreateArgs(int tileCountX, int tileCountY)
        {
            TileCountX = tileCountX;
            TileCountY = tileCountY;
        }
    }

    public class TileArrangementEditorStateMachine : ModelEditorStateMachine<
        TileArrangementCreateArgs, TileArrangement, ReadOnlyTileArrangement>
    {
        public TileArrangementEditorStateMachine()
            : base(new Prompts())
        {

        }

        protected override ReadOnlyTileArrangement Wrap(TileArrangement instance)
        {
            return new ReadOnlyTileArrangement(instance);
        }

        protected override TileArrangement Clone(TileArrangement instance)
        {
            return instance.Clone();
        }

        protected override TileArrangement CreateEmptyModel(TileArrangementCreateArgs args)
        {
            return new TileArrangement(args.TileCountX, args.TileCountY);
        }

        protected override TileArrangement ReadModelFromDisk(string filename)
        {
            using (var stream = File.OpenRead(filename))
                return TileArrangement.Deserialize(stream);
        }

        protected override void WriteModelToDisk(TileArrangement instance, string filename)
        {
            using (var stream = File.Create(filename))
                instance.Serialize(stream);
        }
    }
    public class ReadOnlyTileArrangement
    {
        TileArrangement arrangement;

        public ReadOnlyTileArrangement(TileArrangement instance)
        {
            this.arrangement = instance;
        }

        public int this[int x, int y]
        {
            get { return arrangement[x, y]; }
        }

        public int TileCountX
        {
            get { return arrangement.TileCountX; }
        }

        public int TileCountY
        {
            get { return arrangement.TileCountY; }
        }

        public void Render(Graphics g, ITileset tileset, int scale)
        {
            arrangement.Render(g, tileset, scale);
        }

        internal void RenderSubset(Graphics g, ITileset tileset, int scale, Rectangle visibleRect)
        {
            arrangement.RenderSubset(g, tileset, scale, visibleRect);
        }
    }
}

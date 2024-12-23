using UnaryHeap.GUI;

namespace Patchwork
{
    public class Prompts : BoilerplatePromptStrategy<TileArrangementCreateArgs>
    {
        protected override string Extension
        {
            get { return "arr"; }
        }

        protected override string Filter
        {
            get { return "Tile Arrangement Files (*.arr)|*.arr"; }
        }

        public override TileArrangementCreateArgs RequestNewModelParameters()
        {
            // TODO: Prompt user for arguments instead of using hardcoded values
            return new TileArrangementCreateArgs(45, 30);
        }
    }
}

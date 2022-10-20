using System.Windows.Forms;
using UnaryHeap.GUI;

namespace GraphPaper
{
    class Prompts : BoilerplatePromptStrategy<Graph2DCreateArgs>
    {
        protected override string Extension
        {
            get { return "jg"; }
        }

        protected override string Filter
        {
            get { return "JSON Graph Files (*.jg)|*.jg"; }
        }

        public override Graph2DCreateArgs RequestNewModelParameters()
        {
            using (var dialog = new NewModelArgumentsDialog())
            {
                if (DialogResult.Cancel == dialog.ShowDialog())
                    return null;
                else
                    return new Graph2DCreateArgs(dialog.Directed);
            }
        }
    }
}

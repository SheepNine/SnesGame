using SnesGame.CLR;
using System.IO;
using UnaryHeap.GUI;

namespace Potatune
{
    class RecordingCreateArgs
    {
        public static RecordingCreateArgs Instance = new RecordingCreateArgs();

        private RecordingCreateArgs() { }
    }

    class Prompts : BoilerplatePromptStrategy<RecordingCreateArgs>
    {
        protected override string Extension
        {
            get { return "rec"; }
        }

        protected override string Filter
        {
            get { return "Recording Files (*.rec)|*.rec"; }
        }

        public override RecordingCreateArgs RequestNewModelParameters()
        {
            return RecordingCreateArgs.Instance;
        }
    }

    class RecordingEditorStateMachine : ModelEditorStateMachine<RecordingCreateArgs, Recording, ReadOnlyRecording>
    {
        public RecordingEditorStateMachine()
            : base(new Prompts())
        {
        }

        protected override Recording Clone(Recording instance)
        {
            return instance.Clone();
        }

        protected override Recording CreateEmptyModel(RecordingCreateArgs args)
        {
            return new Recording();
        }

        protected override Recording ReadModelFromDisk(string fileName)
        {
            using (var stream = File.OpenRead(fileName))
                return Recording.Deserialize(stream);
        }

        protected override ReadOnlyRecording Wrap(Recording instance)
        {
            return instance;
        }

        protected override void WriteModelToDisk(Recording instance, string fileName)
        {
            using (var stream = File.Create(fileName))
                instance.Serialize(stream);
        }
    }
}

using SnesGame.CLR;
using System.Collections.Generic;
using System.IO;
using System.Linq;

namespace SnesGame.ResourceCompiler.Pipelines.Recording
{
    public static class RecordingCodeFile
    {
        public static void WriteRecordingCodeFile(this Manifest manifest)
        {
            PipelineUtils.WriteFileIfStale(
                manifest.GetRecordings().Select(wb => wb.SourcePath),
                Path.Combine(manifest.OutputDirectory, "recordings.c"),
                writer =>
                {
                    writer.WriteLine("#include \"recordings.h\"");
                    writer.WriteLine();

                    foreach (var recording in manifest.GetRecordings())
                    {
                        RecordingOpcodeStream data;
                        using (var stream = File.OpenRead(recording.SourcePath))
                            data = RecordingOpcodeStream.FromRecording(SnesGame.CLR.Recording.Deserialize(stream));

                        writer.WriteLine("#define count_{0} {1}", recording.ID, data.TrackCount);

                        writer.Write("Uint16 offsets_{0}[count_{0}] = {{ ", recording.ID);
                        foreach (var offset in data.Offsets)
                            writer.Write("{0}, ", offset);
                        writer.WriteLine("}};", recording.ID);

                        writer.Write("Uint16 lengths_{0}[count_{0}] = {{ ", recording.ID);
                        foreach (var length in data.Lengths)
                            writer.Write("{0}, ", length);
                        writer.WriteLine("}};", recording.ID);

                        writer.Write("Uint8 stream_{0}[{1}] = {{", recording.ID, data.OpcodeCount);
                        var i = 31;
                        foreach (var opcode in data.Opcodes)
                        {
                            i += 1;
                            if (i == 32)
                            {
                                writer.WriteLine();
                                writer.Write("\t");
                                i = 0;
                            }
                            writer.Write("0x{0:X2},", opcode);
                        }
                        writer.WriteLine();
                        writer.WriteLine("}};", recording.ID);
                        writer.WriteLine();
                    }

                    writer.WriteLine("hRecord loadRecording(Uint16 recId) {");
                    writer.WriteLine("\tswitch (recId) {");

                    foreach (var recording in manifest.GetRecordings())
                    {
                        writer.WriteLine("\t\tcase REC_{0}:", recording.ID);
                        writer.WriteLine("\t\t\treturn creat_Record(count_{0}, stream_{0}, offsets_{0}, lengths_{0});", recording.ID);
                    }

                    writer.WriteLine("\t\tdefault:");
                    writer.WriteLine("\t\t\treturn NULL;");
                    writer.WriteLine("\t}");
                    writer.WriteLine("}");
                });
        }

        class RecordingOpcodeStream
        {
            const byte OPC_PLAY = 0;
            const byte OPC_SILENCE = 1;

            const byte OPC_DEFAULT_LENGTH = 2;
            const byte OPC_DEFAULT_VSHIFT_DIR = 3;
            const byte OPC_DEFAULT_VSHIFT_EB = 4;
            const byte OPC_DEFAULT_VSHIFT_SPEED = 5;
            const byte OPC_DEFAULT_VOLUME_BOTH = 6;
            const byte OPC_DEFAULT_VOLUME_RANGE = 7;
            const byte OPC_DEFAULT_PSHIFT_DIR = 8;
            const byte OPC_DEFAULT_PSHIFT_EB = 9;
            const byte OPC_DEFAULT_PSHIFT_SPEED = 10;
            const byte OPC_DEFAULT_PERIOD_BOTH = 11;
            const byte OPC_DEFAULT_PERIOD_RANGE = 12;

            const byte OPC_SINGLE_LENGTH = 13;
            const byte OPC_SINGLE_VSHIFT_DIR = 14;
            const byte OPC_SINGLE_VSHIFT_EB = 15;
            const byte OPC_SINGLE_VSHIFT_SPEED = 16;
            const byte OPC_SINGLE_VOLUME_BOTH = 17;
            const byte OPC_SINGLE_VOLUME_RANGE = 18;
            const byte OPC_SINGLE_PSHIFT_DIR = 19;
            const byte OPC_SINGLE_PSHIFT_EB = 20;
            const byte OPC_SINGLE_PSHIFT_SPEED = 21;
            const byte OPC_SINGLE_PERIOD_BOTH = 22;
            const byte OPC_SINGLE_PERIOD_RANGE = 23;

            const byte OPC_PLAY_NOISE = 24;

            const byte OPC_DEFAULT_NOISE_REGISTER = 25;
            const byte OPC_DEFAULT_NOISE_REPEAT = 26;
            const byte OPC_DEFAULT_NOISE_TAP = 27;
            const byte OPC_DEFAULT_NOISE_LENGTH = 28;

            const byte OPC_SINGLE_NOISE_REGISTER = 29;
            const byte OPC_SINGLE_NOISE_REPEAT = 30;
            const byte OPC_SINGLE_NOISE_TAP = 31;
            const byte OPC_SINGLE_NOISE_LENGTH = 32;

            private List<byte> opcodes;
            private List<ushort> starts;
            private List<ushort> lengths;

            public RecordingOpcodeStream()
            {
                opcodes = new List<byte>();
                starts = new List<ushort>();
                lengths = new List<ushort>();
            }

            public void StartTrack()
            {
                starts.Add((ushort)opcodes.Count);
            }

            public void EndTrack()
            {
                lengths.Add((ushort)(opcodes.Count - starts[starts.Count - 1]));
            }

            public void Silence(byte length)
            {
                opcodes.Add(OPC_SILENCE);
                opcodes.Add(length);
            }

            public byte TrackCount
            {
                get { return (byte)starts.Count; }
            }

            public ushort OpcodeCount
            {
                get { return (ushort)opcodes.Count; }
            }

            public IEnumerable<byte> Opcodes
            {
                get { return opcodes; }
            }

            public IEnumerable<ushort> Offsets
            {
                get { return starts; }
            }

            public IEnumerable<ushort> Lengths
            {
                get { return lengths; }
            }

            public void Play(Noise note, byte length)
            {
                EmitNoiseTapMask(note.TapMask);
                EmitNoiseRegister(note.InitialRegister);
                EmitNoiseMaxLength(note.MaxLength);
                EmitNoiseRepeat(note.Speed);

                opcodes.Add(OPC_PLAY_NOISE);
                opcodes.Add(length);
            }

            private void EmitNoiseTapMask(ushort value)
            {
                if (noiseTapMask == value)
                    return;
                noiseTapMask = value;
                opcodes.Add(OPC_DEFAULT_NOISE_TAP);
                AddToStream(value);
            }

            private void EmitNoiseRegister(ushort value)
            {
                if (noiseRegister == value)
                    return;
                noiseRegister = value;
                opcodes.Add(OPC_DEFAULT_NOISE_REGISTER);
                AddToStream(value);
            }

            private void EmitNoiseMaxLength(ushort value)
            {
                if (noiseLength == value)
                    return;
                noiseLength = value;
                opcodes.Add(OPC_DEFAULT_NOISE_LENGTH);
                AddToStream(value);
            }

            private void EmitNoiseRepeat(byte value)
            {
                if (noiseRepeat == value)
                    return;
                noiseRepeat = value;
                opcodes.Add(OPC_DEFAULT_NOISE_REPEAT);
                opcodes.Add(value);
            }

            public void Play(SquareNote note, byte length)
            {
                // TODO: optimize this significantly
                EmitSquareNotePeriod(note.PeriodMin, note.PeriodMax);
                EmitSquareNotePShiftDir(note.PeriodShiftParameters.Direction);
                EmitSquareNotePShiftEB(note.PeriodShiftParameters.EdgeBehaviour);
                EmitSquareNotePShiftSpeed(note.PeriodShiftParameters.Speed);

                EmitSquareNoteVolume(note.VolumeMin, note.VolumeMax);
                EmitSquareNoteVShiftDir(note.VolumeShiftParameters.Direction);
                EmitSquareNoteVShiftEB(note.VolumeShiftParameters.EdgeBehaviour);
                EmitSquareNoteVShiftSpeed(note.VolumeShiftParameters.Speed);

                EmitSquareNoteLength(note.MaxLength);

                opcodes.Add(OPC_PLAY);
                opcodes.Add(length);
            }

            private void EmitSquareNotePeriod(ushort min, ushort max)
            {
                if (squarePeriodMin == min && squarePeriodMax == max)
                    return;
                squarePeriodMin = min;
                squarePeriodMax = max;
                opcodes.Add(OPC_DEFAULT_PERIOD_RANGE);
                AddToStream(min);
                AddToStream(max);
            }

            private void EmitSquareNotePShiftDir(ShiftDirection value)
            {
                if (squarePeriodDir == value)
                    return;
                squarePeriodDir = value;
                opcodes.Add(OPC_DEFAULT_PSHIFT_DIR);
                AddToStream(value);
            }

            private void EmitSquareNotePShiftEB(EdgeBehaviour value)
            {
                if (squarePeriodEB == value)
                    return;
                squarePeriodEB = value;
                opcodes.Add(OPC_DEFAULT_PSHIFT_EB);
                AddToStream(value);
            }

            private void EmitSquareNotePShiftSpeed(byte value)
            {
                if (squarePeriodSpeed == value)
                    return;
                squarePeriodSpeed = value;
                opcodes.Add(OPC_DEFAULT_PSHIFT_SPEED);
                opcodes.Add(value);
            }

            private void EmitSquareNoteVolume(byte min, byte max)
            {
                if (squareVolumeMin == min && squareVolumeMax == max)
                    return;
                squareVolumeMin = min;
                squareVolumeMax = max;
                opcodes.Add(OPC_DEFAULT_VOLUME_RANGE);
                opcodes.Add(min);
                opcodes.Add(max);
            }

            private void EmitSquareNoteVShiftDir(ShiftDirection value)
            {
                if (squareVolumeDir == value)
                    return;
                squareVolumeDir = value;
                opcodes.Add(OPC_DEFAULT_VSHIFT_DIR);
                AddToStream(value);
            }

            private void EmitSquareNoteVShiftEB(EdgeBehaviour value)
            {
                if (squareVolumeEB == value)
                    return;
                squareVolumeEB = value;
                opcodes.Add(OPC_DEFAULT_VSHIFT_EB);
                AddToStream(value);
            }

            private void EmitSquareNoteVShiftSpeed(byte value)
            {
                if (squareVolumeSpeed == value)
                    return;
                squareVolumeSpeed = value;
                opcodes.Add(OPC_DEFAULT_VSHIFT_SPEED);
                opcodes.Add(value);
            }

            private void EmitSquareNoteLength(ushort value)
            {
                if (squareMaxLength == value)
                    return;
                squareMaxLength = value;
                opcodes.Add(OPC_DEFAULT_LENGTH);
                AddToStream(value);
            }

            ushort? squarePeriodMin;
            ushort? squarePeriodMax;
            ShiftDirection? squarePeriodDir;
            EdgeBehaviour? squarePeriodEB;
            byte? squarePeriodSpeed;
            byte? squareVolumeMin;
            byte? squareVolumeMax;
            ShiftDirection? squareVolumeDir;
            EdgeBehaviour? squareVolumeEB;
            byte? squareVolumeSpeed;
            ushort? squareMaxLength;
            ushort? noiseTapMask;
            ushort? noiseRegister;
            ushort? noiseLength;
            byte? noiseRepeat;

            private void AddToStream(EdgeBehaviour value)
            {
                switch (value)
                {
                    case EdgeBehaviour.ResetValue:
                        opcodes.Add(1);
                        break;
                    case EdgeBehaviour.ReverseShiftDirection:
                        opcodes.Add(2);
                        break;
                    case EdgeBehaviour.CancelShift:
                        opcodes.Add(3);
                        break;
                    default:
                        opcodes.Add(0);
                        break;
                }
            }

            private void AddToStream(ShiftDirection value)
            {
                switch (value)
                {
                    case ShiftDirection.Rising:
                        opcodes.Add(1);
                        break;
                    case ShiftDirection.Falling:
                        opcodes.Add(2);
                        break;
                    default:
                        opcodes.Add(0);
                        break;
                }
            }

            private void AddToStream(ushort value)
            {
                opcodes.Add((byte)(value >> 8));
                opcodes.Add((byte)(value & 0xFF));
            }

            public static RecordingOpcodeStream FromRecording(SnesGame.CLR.Recording source)
            {
                var result = new RecordingOpcodeStream();

                foreach (var trackIndex in Enumerable.Range(0, source.NumTracks))
                {
                    result.StartTrack();

                    var noteIndices = Enumerable.Range(0, source.Length).Where(i => source[trackIndex, i] != null).ToArray();
                    var noteLengths = Enumerable.Range(0, noteIndices.Length).Select(i => (i == noteIndices.Length - 1 ? source.Length : noteIndices[i + 1]) - noteIndices[i]).ToArray();

                    // Range check missing!
                    if (noteIndices[0] > 0)
                        result.Silence((byte)noteIndices[0]);

                    foreach (var i in Enumerable.Range(0, noteIndices.Length))
                    {
                        var note = source[trackIndex, noteIndices[i]];
                        // Range check missing!
                        if (note is SquareNote)
                            result.Play(note as SquareNote, (byte)noteLengths[i]);
                        if (note is Noise)
                            result.Play(note as Noise, (byte)noteLengths[i]);
                    }

                    result.EndTrack();
                }

                // Range check: opcode stream < 64KB

                return result;
            }
        }
    }
}

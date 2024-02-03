using SnesGame.CLR;
using System;
using System.Globalization;
using System.IO;
using System.Media;
using System.Windows.Forms;

namespace Potatune
{
    partial class SquareNoteEditor : Form
    {
        public SquareNoteEditor()
        {
            InitializeComponent();
            lengthComboBox.SelectedIndex = 0;
            volumeDirComboBox.SelectedIndex = 0;
            periodDirComboBox.SelectedIndex = 0;

            Note = new Noise(0x1, 0x2, 1200, 0);
            Note = new SquareNote(38,
                8, 8, new ShiftParameters(
                    ShiftDirection.None, 0, EdgeBehaviour.SilenceChannel),
                183, 183, new ShiftParameters(
                    ShiftDirection.None, 0, EdgeBehaviour.SilenceChannel));
        }

        private void lengthComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            var isLimited = lengthComboBox.SelectedItem.Equals("Limited");
            lengthLabel1.Visible = isLimited;
            lengthTextBox.Visible = isLimited;
            lengthLabel2.Visible = isLimited;
        }

        private void volumeDirComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            var isVariable = !volumeDirComboBox.SelectedItem.Equals("Steady");
            volumeLabel1.Text = isVariable ? "from" : "at";
            volumeLabel2.Visible = isVariable;
            volumeMaxTextBox.Visible = isVariable;
            volumeLabel3.Visible = isVariable;
            volumeSpeedTextBox.Visible = isVariable;
            volumeLabel4.Visible = isVariable;
            volumeEdgeComboBox.Visible = isVariable;
        }

        private void pitchDirComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            var isVariable = !periodDirComboBox.SelectedItem.Equals("Steady");
            pitchLabel1.Text = isVariable ? "from" : "at";
            pitchLabel2.Visible = isVariable;
            periodMaxTextBox.Visible = isVariable;
            pitchLabel3.Visible = isVariable;
            periodSpeedTextBox.Visible = isVariable;
            pitchLabel4.Visible = isVariable;
            periodEdgeComboBox.Visible = isVariable;
        }

        public INote Note
        {
            get
            {
                if (noteType.SelectedItem.Equals("Square"))
                    return SquareNote;
                if (noteType.SelectedItem.Equals("Noise"))
                    return Noise;
                throw new InvalidOperationException();
            }
            set
            {
                if (value == null)
                    return;

                if (value is SquareNote)
                {
                    SquareNote = value as SquareNote;
                    noteType.SelectedItem = "Square";
                }
                else if (value is Noise)
                {
                    Noise = value as Noise;
                    noteType.SelectedItem = "Noise";
                }
                else
                {
                    throw new InvalidOperationException();
                }
            }
        }

        public SquareNote SquareNote
        {
            get
            {
                ushort maxLength;
                if (!MaxLength.HasValue)
                    throw new InvalidOperationException("Length invalid");
                maxLength = MaxLength.Value;

                byte volumeMin, volumeMax;
                if (!VolumeMin.HasValue)
                    throw new InvalidOperationException("Volume invalid");
                volumeMin = VolumeMin.Value;

                ushort periodMin, periodMax;
                if (!PeriodMin.HasValue)
                    throw new InvalidOperationException("Period invalid");
                periodMin = PeriodMin.Value;

                ShiftParameters volumeShiftParameters;
                if (VolumeShiftDirection == ShiftDirection.None)
                {
                    volumeMax = volumeMin;
                    volumeShiftParameters = new ShiftParameters(
                        VolumeShiftDirection, 0, EdgeBehaviour.SilenceChannel);
                }
                else
                {
                    if (!VolumeMax.HasValue)
                        throw new InvalidOperationException("Volume2 invalid");
                    if (!VolumeSpeed.HasValue)
                        throw new InvalidOperationException("Volume speed invalid");
                    volumeMax = VolumeMax.Value;
                    if (volumeMin == volumeMax)
                        throw new InvalidOperationException("Volume range flat");
                    volumeShiftParameters = new ShiftParameters(
                        VolumeShiftDirection, VolumeSpeed.Value, VolumeEdgeBehaviour);
                }

                ShiftParameters periodShiftParameters;
                if (PeriodShiftDirection == ShiftDirection.None)
                {
                    periodMax = periodMin;
                    periodShiftParameters = new ShiftParameters(
                        PeriodShiftDirection, 0, EdgeBehaviour.SilenceChannel);
                }
                else
                {
                    if (!PeriodMax.HasValue)
                        throw new InvalidOperationException("Period2 invalid");
                    if (!PeriodSpeed.HasValue)
                        throw new InvalidOperationException("Period speed invalid");
                    periodMax = PeriodMax.Value;
                    if (periodMin == periodMax)
                        throw new InvalidOperationException("Period range flat");
                    periodShiftParameters = new ShiftParameters(
                        PeriodShiftDirection, PeriodSpeed.Value, PeriodEdgeBehaviour);
                }

                return new SquareNote(maxLength,
                    Math.Min(volumeMin, volumeMax),
                    Math.Max(volumeMin, volumeMax),
                    volumeShiftParameters,
                    Math.Min(periodMin, periodMax),
                    Math.Max(periodMin, periodMax),
                    periodShiftParameters);
            }
            set
            {
                MaxLength = value.MaxLength;

                if (value.VolumeShiftParameters.Direction == ShiftDirection.Falling)
                {
                    VolumeMin = value.VolumeMax;
                    VolumeMax = value.VolumeMin;
                }
                else
                {
                    VolumeMin = value.VolumeMin;
                    VolumeMax = value.VolumeMax;
                }

                VolumeEdgeBehaviour = value.VolumeShiftParameters.EdgeBehaviour;
                VolumeShiftDirection = value.VolumeShiftParameters.Direction;
                VolumeSpeed = value.VolumeShiftParameters.Speed;

                if (value.PeriodShiftParameters.Direction == ShiftDirection.Falling)
                {
                    PeriodMin = value.PeriodMin;
                    PeriodMax = value.PeriodMax;
                }
                else
                {
                    PeriodMin = value.PeriodMax;
                    PeriodMax = value.PeriodMin;
                }

                PeriodEdgeBehaviour = value.PeriodShiftParameters.EdgeBehaviour;
                PeriodShiftDirection = value.PeriodShiftParameters.Direction;
                PeriodSpeed = value.PeriodShiftParameters.Speed;
            }
        }

        public Noise Noise
        {
            get
            {
                if (!NoiseInitialRegister.HasValue)
                    throw new InvalidOperationException("NoiseInitialRegister invalid");
                if (!NoiseMaxLength.HasValue)
                    throw new InvalidOperationException("NoiseMaxLength invalid");
                if (!NoiseRepeat.HasValue)
                    throw new InvalidOperationException("NoiseSpeed invalid");
                if (!NoiseTapMask.HasValue)
                    throw new InvalidOperationException("NoiseTapMask invalid");

                return new Noise(NoiseInitialRegister.Value, NoiseTapMask.Value,
                    NoiseMaxLength.Value, NoiseRepeat.Value);
            }
            set
            {
                NoiseInitialRegister = value.InitialRegister;
                NoiseMaxLength = value.MaxLength;
                NoiseRepeat = value.Speed;
                NoiseTapMask = value.TapMask;
            }
        }

        private ushort? NoiseInitialRegister
        {
            get
            {
                ushort entered;
                if (!ushort.TryParse(noiseRegisterValue.Text, NumberStyles.HexNumber,
                        CultureInfo.InvariantCulture, out entered))
                    return null;
                return entered;
            }
            set
            {
                noiseRegisterValue.Text = value.Value.ToString("X", CultureInfo.InvariantCulture);
            }
        }

        private ushort? NoiseMaxLength
        {
            get
            {
                ushort entered;
                if (!ushort.TryParse(noiseLength.Text, out entered))
                    return null;
                return entered;
            }
            set
            {
                noiseLength.Text = (value??0).ToString(CultureInfo.InvariantCulture);
            }
        }

        private byte? NoiseRepeat
        {
            get { return ReadSpeedValue(noiseRepeat.Text); }
            set { WriteSpeedValue(noiseRepeat, value); }
        }

        private ushort? NoiseTapMask
        {
            get
            {
                return noiseTapBits.SelectedItem.Equals("Long") ? (ushort)0x02 : (ushort)0x40;
            }
            set
            {
                noiseTapBits.SelectedItem = value == 0x02 ? "Long" : "Short";
            }
        }

        private ushort? MaxLength
        {
            get
            {
                if (lengthComboBox.SelectedItem.Equals("Unlimited"))
                    return 0;

                uint entered;
                if (!uint.TryParse(lengthTextBox.Text, out entered))
                    return null;
                if (entered % 100 != 0)
                    return null;
                entered /= 100;
                if (entered > ushort.MaxValue)
                    return null;
                return (ushort)entered;
            }
            set
            {
                if (!value.HasValue) throw new ArgumentNullException(nameof(value));
                if (value.Value == 0)
                {
                    lengthComboBox.SelectedItem = "Unlimited";
                }
                else
                {
                    lengthComboBox.SelectedItem = "Limited";
                    lengthTextBox.Text = ((int)value.Value * 100)
                        .ToString(CultureInfo.InvariantCulture);
                }
            }
        }

        private byte? VolumeMin
        {
            get { return ReadVolumeValue(volumeMinTextBox.Text); }
            set { WriteVolumeValue(volumeMinTextBox, value); }
        }

        private byte? VolumeMax
        {
            get { return ReadVolumeValue(volumeMaxTextBox.Text); }
            set { WriteVolumeValue(volumeMaxTextBox, value); }
        }

        private static byte? ReadVolumeValue(string value)
        {
            byte entered;
            if (!byte.TryParse(value, out entered))
                return null;
            if (entered < 1 || entered > 8)
                return null;
            return entered;
        }

        private static void WriteVolumeValue(TextBox control, byte? value)
        {
            if (!value.HasValue) throw new ArgumentNullException(nameof(value));
            control.Text = value.Value.ToString(CultureInfo.InvariantCulture);
        }

        private ushort? PeriodMin
        {
            get { return ReadPeriodValue(periodMinTextBox.Text); }
            set { WritePeriodValue(periodMinTextBox, value); }
        }

        private ushort? PeriodMax
        {
            get { return ReadPeriodValue(periodMaxTextBox.Text); }
            set { WritePeriodValue(periodMaxTextBox, value); }
        }

        private static ushort? ReadPeriodValue(string text)
        {
            switch (text)
            {
                case "A0": return 873;
                case "A#0": return 824;
                case "B0": return 778;
                case "C1": return 734;
                case "C#1": return 693;
                case "D1": return 654;
                case "D#1": return 617;
                case "E1": return 582;
                case "F1": return 550;
                case "F#1": return 519;
                case "G1": return 490;
                case "G#1": return 462;
                case "A1": return 436;
                case "A#1": return 412;
                case "B1": return 389;
                case "C2": return 367;
                case "C#2": return 346;
                case "D2": return 327;
                case "D#2": return 309;
                case "E2": return 291;
                case "F2": return 275;
                case "F#2": return 259;
                case "G2": return 245;
                case "G#2": return 231;
                case "A2": return 218;
                case "A#2": return 206;
                case "B2": return 194;
                case "C3": return 183;
                case "C#3": return 173;
                case "D3": return 163;
                case "D#3": return 154;
                case "E3": return 146;
                case "F3": return 137;
                case "F#3": return 130;
                case "G3": return 122;
                case "G#3": return 116;
                case "A3": return 109;
                case "A#3": return 103;
                case "B3": return 97;
                case "C4": return 92;
                case "C#4": return 87;
                case "D4": return 82;
                case "D#4": return 77;
                case "E4": return 73;
                case "F4": return 69;
                case "F#4": return 65;
                case "G4": return 61;
                case "G#4": return 58;
                case "A4": return 55;
                case "A#4": return 51;
                case "B4": return 49;
                case "C5": return 46;
                case "C#5": return 43;
                case "D5": return 41;
                case "D#5": return 39;
                case "E5": return 36;
                case "F5": return 34;
                case "F#5": return 32;
                case "G5": return 31;
                case "G#5": return 29;
                case "A5": return 27;
                case "A#5": return 26;
                case "B5": return 24;
                case "C6": return 23;
                case "C#6": return 22;
                case "D6": return 20;
                case "D#6": return 19;
                case "E6": return 18;
                case "F6": return 17;
                case "F#6": return 16;
                default:
                    {
                        ushort entered;
                        if (!ushort.TryParse(text, out entered))
                            return null;
                        if (entered < 16 || entered > 1200)
                            return null;
                        return entered;
                    }

            }
        }
        public static string PeriodString(ushort? value)
        {
            if (!value.HasValue) throw new ArgumentNullException(nameof(value));
            switch (value.Value)
            {
                case 873: return "A0";
                case 824: return "A#0";
                case 778: return "B0";
                case 734: return "C1";
                case 693: return "C#1";
                case 654: return "D1";
                case 617: return "D#1";
                case 582: return "E1";
                case 550: return "F1";
                case 519: return "F#1";
                case 490: return "G1";
                case 462: return "G#1";
                case 436: return "A1";
                case 412: return "A#1";
                case 389: return "B1";
                case 367: return "C2";
                case 346: return "C#2";
                case 327: return "D2";
                case 309: return "D#2";
                case 291: return "E2";
                case 275: return "F2";
                case 259: return "F#2";
                case 245: return "G2";
                case 231: return "G#2";
                case 218: return "A2";
                case 206: return "A#2";
                case 194: return "B2";
                case 183: return "C3";
                case 173: return "C#3";
                case 163: return "D3";
                case 154: return "D#3";
                case 146: return "E3";
                case 137: return "F3";
                case 130: return "F#3";
                case 122: return "G3";
                case 116: return "G#3";
                case 109: return "A3";
                case 103: return "A#3";
                case 97: return "B3";
                case 92: return "C4";
                case 87: return "C#4";
                case 82: return "D4";
                case 77: return "D#4";
                case 73: return "E4";
                case 69: return "F4";
                case 65: return "F#4";
                case 61: return "G4";
                case 58: return "G#4";
                case 55: return "A4";
                case 51: return "A#4";
                case 49: return "B4";
                case 46: return "C#5";
                case 43: return "C5";
                case 41: return "D5";
                case 39: return "D#5";
                case 36: return "E5";
                case 34: return "F5";
                case 32: return "F#5";
                case 31: return "G5";
                case 29: return "G#5";
                case 27: return "A5";
                case 26: return "A#5";
                case 24: return "B5";
                case 23: return "C6";
                case 22: return "C#6";
                case 20: return "D6";
                case 19: return "D#6";
                case 18: return "E6";
                case 17: return "F6";
                case 16: return "F#6";
                default: return value.Value.ToString(CultureInfo.InvariantCulture);
            }
        }

        private static void WritePeriodValue(TextBox control, ushort? value)
        {
            control.Text = PeriodString(value);
        }

        private byte? VolumeSpeed
        {
            get { return ReadSpeedValue(volumeSpeedTextBox.Text); }
            set { WriteSpeedValue(volumeSpeedTextBox, value); }
        }

        private byte? PeriodSpeed
        {
            get { return ReadSpeedValue(periodSpeedTextBox.Text); }
            set { WriteSpeedValue(periodSpeedTextBox, value); }
        }

        private static byte? ReadSpeedValue(string text)
        {
            ushort entered;
            if (!ushort.TryParse(text, out entered))
                return null;
            entered -= 1;
            if (entered > byte.MaxValue)
                return null;
            return (byte)entered;
        }

        private static void WriteSpeedValue(TextBox control, byte? value)
        {
            if (!value.HasValue) throw new ArgumentNullException(nameof(value));
            control.Text = ((int)value.Value + 1).ToString(CultureInfo.InvariantCulture);
        }

        private EdgeBehaviour VolumeEdgeBehaviour
        {
            get { return ReadEdgeBehaviourValue(volumeEdgeComboBox.Text); }
            set { WriteEdgeBehaviourValue(volumeEdgeComboBox, value); }
        }

        private EdgeBehaviour PeriodEdgeBehaviour
        {
            get { return ReadEdgeBehaviourValue(periodEdgeComboBox.Text); }
            set { WriteEdgeBehaviourValue(periodEdgeComboBox, value); }
        }

        private static EdgeBehaviour ReadEdgeBehaviourValue(string text)
        {
            switch (text)
            {
                case "Reverse direction":
                    return EdgeBehaviour.ReverseShiftDirection;
                case "Wrap value":
                    return EdgeBehaviour.ResetValue;
                case "End note":
                    return EdgeBehaviour.SilenceChannel;
                default:
                    return EdgeBehaviour.CancelShift;
            }
        }

        private static void WriteEdgeBehaviourValue(ComboBox control, EdgeBehaviour value)
        {
            switch (value)
            {
                case EdgeBehaviour.ReverseShiftDirection:
                    control.SelectedItem = "Reverse direction";
                    break;
                case EdgeBehaviour.ResetValue:
                    control.SelectedItem = "Wrap value";
                    break;
                case EdgeBehaviour.SilenceChannel:
                    control.SelectedItem = "End note";
                    break;
                default:
                    control.SelectedItem = "Cancel shift";
                    break;
            }
        }

        private ShiftDirection VolumeShiftDirection
        {
            get { return ReadShiftDirectionValue(volumeDirComboBox.Text); }
            set { WriteShiftDirectionValue(volumeDirComboBox, value); }
        }

        private ShiftDirection PeriodShiftDirection
        {
            get { return ReadShiftDirectionValue(periodDirComboBox.Text); }
            set { WriteShiftDirectionValue(periodDirComboBox, value); }
        }

        private static ShiftDirection ReadShiftDirectionValue(string text)
        {
            switch (text)
            {
                case "Rising":
                    return ShiftDirection.Rising;
                case "Falling":
                    return ShiftDirection.Falling;
                default:
                    return ShiftDirection.None;
            }
        }

        private static void WriteShiftDirectionValue(ComboBox control, ShiftDirection value)
        {
            switch (value)
            {
                case ShiftDirection.Rising:
                    control.SelectedItem = "Rising";
                    break;
                case ShiftDirection.Falling:
                    control.SelectedItem = "Falling";
                    break;
                default:
                    control.SelectedItem = "Steady";
                    break;
            }
        }

        private void SquareNoteEditor_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (DialogResult != DialogResult.OK) return;

            try
            {
                var a = Note;
            }
            catch (InvalidOperationException ex)
            {
                MessageBox.Show(ex.Message);
                e.Cancel = true;
            }
        }

        private void playButton_Click(object sender, EventArgs e)
        {
            INote note;
            try
            {
                note = Note;
            }
            catch (InvalidOperationException ex)
            {
                MessageBox.Show(ex.Message);
                return;
            }

            var buffer = Mixer.SampleAudio(note, 48000);

            var stream = new MemoryStream();
            stream.WriteWaveFile(48000, buffer);
            stream.Seek(0, SeekOrigin.Begin);

            new SoundPlayer() { Stream = stream }.Play();
        }

        private void noteType_SelectedIndexChanged(object sender, EventArgs e)
        {
            squareWavePanel.Visible = noteType.SelectedItem.Equals("Square");
            noisePanel.Visible = noteType.SelectedItem.Equals("Noise");
        }
    }
}

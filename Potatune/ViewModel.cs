using SnesGame.CLR;
using System;
using System.Drawing;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Media;
using System.Windows.Forms;
using UnaryHeap.GUI;

namespace Potatune
{
    public interface IViewModel : IDisposable
    {
        event EventHandler CurrentFileNameChanged;
        event EventHandler IsModifiedChanged;
        string CurrentFileName { get; }
        bool IsModified { get; }

        void HookupToView(WysiwygPanel editorPanel,
            GestureInterpreter editorGestures, HScrollBar editorScrollBar);

        void NewTune();
        void OpenTune();
        void SaveTune();
        void SaveTuneAs();

        void Undo();
        void Redo();

        void PlaySelectedRange();
        void StopPlayback();

        bool CanClose();
        void EditLength();
        void IncreaseSkip();
        void DecreaseSkip();
        void AddTrack();
    }

    public class ViewModel : IViewModel
    {
        const int NOTE_WIDTH = 100;
        RecordingEditorStateMachine stateMachine;
        WysiwygPanel editorPanel;
        GestureInterpreter editorGestures;
        HScrollBar editorScrollBar;
        WysiwygFeedbackStrategyContext editorFeedback;
        SoundPlayer player;
        int playbackStart, playbackEnd;
        int skip = 1;

        public ViewModel()
        {
            stateMachine = new RecordingEditorStateMachine();
            player = new SoundPlayer();
        }

        public void Dispose()
        {
            player.Dispose();
            GC.SuppressFinalize(this);
        }

        public void Run()
        {
            var view = new View(this);
            stateMachine.NewModel(RecordingCreateArgs.Instance);
            UpdateScrollBarRange();
            playbackStart = 0;
            playbackEnd = stateMachine.CurrentModelState.Length - 1;
            Application.Run(view);
        }

        public string CurrentFileName
        {
            get { return stateMachine.CurrentFileName; }
        }

        public bool IsModified
        {
            get { return stateMachine.IsModelModified; }
        }

        public event EventHandler CurrentFileNameChanged
        {
            add { stateMachine.CurrentFileNameChanged += value; }
            remove { stateMachine.CurrentFileNameChanged -= value; }
        }

        public event EventHandler IsModifiedChanged
        {
            add { stateMachine.IsModifiedChanged += value; }
            remove { stateMachine.IsModifiedChanged -= value; }
        }

        public void HookupToView(WysiwygPanel editorPanel,
            GestureInterpreter editorGestures, HScrollBar editorScrollBar)
        {
            this.editorPanel = editorPanel;
            this.editorGestures = editorGestures;
            this.editorScrollBar = editorScrollBar;

            editorPanel.SizeChanged += editorPanel_SizeChanged;
            editorPanel.PaintContent += editorPanel_PaintContent;
            editorGestures.StateChanged += editorGestures_StateChanged;
            editorGestures.ClickGestured += editorGestures_ClickGestured;
            editorGestures.DragGestured += editorGestures_DragGestured;
            editorScrollBar.ValueChanged += editorScrollBar_ValueChanged;

            editorFeedback = new WysiwygFeedbackStrategyContext(editorPanel);

            stateMachine.ModelChanged += stateMachine_ModelChanged;
            stateMachine.ModelReplaced += stateMachine_ModelReplaced;
        }

        private void editorPanel_SizeChanged(object sender, EventArgs e)
        {
            UpdateScrollBarRange();
        }

        private void editorPanel_PaintContent(object sender, PaintEventArgs e)
        {
            e.Graphics.Clear(Color.Black);

            using (var font = new Font(FontFamily.GenericMonospace, 10))
            {
                var notesVisible = editorScrollBar.Width / NOTE_WIDTH;
                var model = stateMachine.CurrentModelState;
                var g = e.Graphics;
                g.Clear(Color.Black);

                for (int track = 0; track < model.NumTracks; track++)
                {
                    var rowState = g.Save();
                    g.TranslateTransform(0, NOTE_WIDTH * track);
                    editorPanel_PaintRow(font, notesVisible, model, g, track);

                    g.Restore(rowState);
                }

                int[] increments = new int[] { 18, 72 };

                for (int i = 0; i < notesVisible; i++)
                {
                    var position = editorScrollBar.Value + skip * i;
                    if (position >= model.Length)
                        break;

                    var state = g.Save();
                    g.TranslateTransform(NOTE_WIDTH * i, 0);

                    foreach (var inc in Enumerable.Range(0, increments.Length).Reverse())
                    {
                        if ((position) % increments[inc] == 0)
                        {
                            g.FillRectangle(Brushes.AntiqueWhite,
                                -1, 0, 2, NOTE_WIDTH * model.NumTracks + 16 * (inc + 1));
                            g.DrawString(LengthEditor.NotePositionToString(position), font,
                                Brushes.AntiqueWhite, 1, NOTE_WIDTH * model.NumTracks + 12 * inc);
                            g.DrawString(
                                (position / increments[inc]).ToString(
                                    CultureInfo.InvariantCulture),
                                font,
                                Brushes.AntiqueWhite, 0, NOTE_WIDTH * model.NumTracks + 12 * inc,
                                new StringFormat() { Alignment = StringAlignment.Far });
                            break;
                        }
                    }

                    if (position >= playbackStart && position <= playbackEnd)
                    {
                        using (var brush = new SolidBrush(Color.FromArgb(16, Color.Orange)))
                            g.FillRectangle(brush,
                                0, 0, NOTE_WIDTH, NOTE_WIDTH * model.NumTracks);
                        g.FillRectangle(Brushes.Orange,
                            0, 0, NOTE_WIDTH, 2);
                        g.FillRectangle(Brushes.Orange,
                            0, model.NumTracks * NOTE_WIDTH - 2, NOTE_WIDTH, 2);
                    }
                    if (position == playbackStart)
                        g.FillRectangle(Brushes.Orange,
                            0, 0, 2, NOTE_WIDTH * model.NumTracks);
                    if (position == playbackEnd)
                        g.FillRectangle(Brushes.Orange,
                            NOTE_WIDTH - 2, 0, 2, NOTE_WIDTH * model.NumTracks);

                    g.Restore(state);
                }
            }
        }

        private void editorPanel_PaintRow(Font font, int notesVisible,
            IReadOnlyRecording model, Graphics g, int track)
        {
            for (int i = 0; i < notesVisible; i++)
            {
                var position = editorScrollBar.Value + skip * i;
                if (position >= model.Length)
                    break;

                var state = g.Save();
                g.TranslateTransform(NOTE_WIDTH * i, 0);

                var note = model[track, position];

                var brush = Brushes.Gray;
                if (note is SquareNote)
                    brush = Brushes.Blue;
                if (note is Noise)
                    brush = Brushes.Green;
                g.FillRectangle(brush, 0, 0, NOTE_WIDTH, NOTE_WIDTH);
                g.DrawRectangle(Pens.CornflowerBlue, 0, 0, NOTE_WIDTH - 1, NOTE_WIDTH - 1);

                if (note != null)
                {
                    if (note is SquareNote)
                    {
                        var snote = note as SquareNote;
                        // Period
                        g.DrawString(SquarePeriodString(snote), font, Brushes.Silver, 0, 0);
                        // Volume
                        g.DrawString(SquareVolumeString(snote), font, Brushes.Silver, 0, 40);
                        // Length
                        g.DrawString(SquareLengthString(snote), font, Brushes.Silver, 0, 80);
                    }
                    if (note is Noise)
                    {
                        var snote = note as Noise;
                        var str1 = string.Format(CultureInfo.InvariantCulture, "{0}:{1}",
                            snote.TapMask == 0x02 ? "Long" : "Short", snote.Speed + 1);
                        g.DrawString(str1,
                            font, Brushes.Silver, 0, 0);
                        g.DrawString(snote.MaxLength.ToString(CultureInfo.InvariantCulture),
                            font, Brushes.Silver, 0, 40);
                        g.DrawString(string.Format(CultureInfo.InvariantCulture,
                            "0x{0:X4}", snote.InitialRegister),
                            font, Brushes.Silver, 0, 80);
                    }
                }

                g.Restore(state);
            }
        }

        private static string SquarePeriodString(SquareNote snote)
        {
            if (snote.PeriodShiftParameters.Direction == ShiftDirection.None)
                return SquareNoteEditor.PeriodString(snote.PeriodMin);

            return string.Format(CultureInfo.InvariantCulture, "{0}-{1}@{2}\n{3}",
                SquareNoteEditor.PeriodString(
                    snote.PeriodShiftParameters.Direction == ShiftDirection.Rising
                        ? snote.PeriodMax : snote.PeriodMin),
                SquareNoteEditor.PeriodString(
                    snote.PeriodShiftParameters.Direction == ShiftDirection.Rising
                        ? snote.PeriodMin : snote.PeriodMax),
                snote.PeriodShiftParameters.Speed + 1,
                EdgeBehaviourString(snote.PeriodShiftParameters.EdgeBehaviour));
        }

        private static string SquareVolumeString(SquareNote snote)
        {
            if (snote.VolumeShiftParameters.Direction == ShiftDirection.None)
                return snote.VolumeMin.ToString(CultureInfo.InvariantCulture);

            return string.Format(CultureInfo.InvariantCulture, "{0}-{1}@{2}\n{3}",
                snote.VolumeShiftParameters.Direction == ShiftDirection.Rising
                    ? snote.VolumeMin : snote.VolumeMax,
                snote.VolumeShiftParameters.Direction == ShiftDirection.Rising
                    ? snote.VolumeMax : snote.VolumeMin,
                snote.VolumeShiftParameters.Speed + 1,
                EdgeBehaviourString(snote.VolumeShiftParameters.EdgeBehaviour));
        }

        private static string EdgeBehaviourString(EdgeBehaviour value)
        {
            switch (value)
            {
                case EdgeBehaviour.CancelShift:
                    return "cancel";
                case EdgeBehaviour.ResetValue:
                    return "reset";
                case EdgeBehaviour.ReverseShiftDirection:
                    return "reverse";
                case EdgeBehaviour.SilenceChannel:
                    return "silence";
                default:
                    return "";
            }
        }

        private static string SquareLengthString(SquareNote snote)
        {
            if (snote.MaxLength == 0)
                return "";

            return string.Format(CultureInfo.InvariantCulture,
                "{0:F2}s", ((snote.MaxLength * 100) / 48000.0));
        }

        private static string Timestamp(int position)
        {
            return (position / 50f).ToString(CultureInfo.InvariantCulture);
        }

        private void editorGestures_StateChanged(object sender, EventArgs e)
        {
            if (editorGestures.CurrentState == GestureState.Idle)
            {
                editorFeedback.ClearFeedback();
            }
            else
            {
                var model = stateMachine.CurrentModelState;
                var screenPosition = editorGestures.CurrentPosition.X / NOTE_WIDTH;
                var trackPosition = screenPosition * skip + editorScrollBar.Value;
                var track = editorGestures.CurrentPosition.Y / NOTE_WIDTH;

                if (trackPosition >= model.Length || track >= model.NumTracks)
                    editorFeedback.ClearFeedback();
                else
                    editorFeedback.SetFeedback(new NoteBoxHighlightFeedback(
                        track, screenPosition, trackPosition, NOTE_WIDTH));
            }
        }

        private void editorGestures_ClickGestured(object sender, ClickGestureEventArgs e)
        {
            var model = stateMachine.CurrentModelState;
            var track = e.ClickPoint.Y / NOTE_WIDTH;
            if (track >= model.NumTracks)
                return;
            var position = skip * (e.ClickPoint.X / NOTE_WIDTH) + editorScrollBar.Value;
            var note = model[track, position];


            if (e.ModifierKeys == Keys.None)
            {
                if (e.Button == MouseButtons.Right)
                {
                    if (note == null)
                        return;
                    stateMachine.Do(mdl => mdl.RemoveNote(track, position));
                }
                if (e.Button == MouseButtons.Left)
                {
                    var dialog = new SquareNoteEditor();
                    dialog.Note = note;
                    if (dialog.ShowDialog() == DialogResult.OK)
                        stateMachine.Do(mdl =>
                        {
                            if (note == null)
                                mdl.AddNote(track, position, dialog.Note);
                            else
                                mdl.EditNote(track, position, dialog.Note);
                        });
                }
            }
            if (e.ModifierKeys == Keys.Alt)
            {
                if (e.Button == MouseButtons.Left)
                {
                    playbackStart = position;
                    playbackEnd = Math.Max(playbackEnd, playbackStart);
                }
                if (e.Button == MouseButtons.Right)
                {
                    playbackEnd = position;
                    playbackStart = Math.Min(playbackStart, playbackEnd);
                }
                editorPanel.InvalidateContent(); // TODO: Feedback? Does it matter?
            }
        }

        private void editorGestures_DragGestured(object sender, DragGestureEventArgs e)
        {
        }

        private void editorScrollBar_ValueChanged(object sender, EventArgs e)
        {
            editorPanel.InvalidateContent();
        }

        private void stateMachine_ModelChanged(object sender, EventArgs e)
        {
            playbackEnd = Math.Min(playbackEnd, stateMachine.CurrentModelState.Length - 1);
            UpdateScrollBarRange();
            editorPanel.InvalidateContent();
        }

        private void stateMachine_ModelReplaced(object sender, EventArgs e)
        {
            editorScrollBar.Value = 0;
            playbackStart = 0;
            playbackEnd = stateMachine.CurrentModelState.Length - 1;
            UpdateScrollBarRange();
            editorPanel.InvalidateContent();
        }

        private void UpdateScrollBarRange()
        {
            var notesVisible = editorScrollBar.Width / NOTE_WIDTH;
            var slack = stateMachine.CurrentModelState.Length - notesVisible;

            editorScrollBar.SmallChange = skip;
            editorScrollBar.LargeChange = skip;
            editorScrollBar.Minimum = 0;
            editorScrollBar.Maximum = Math.Max(slack, 0);
            editorScrollBar.Enabled = editorScrollBar.Maximum > 0;
        }

        public void NewTune()
        {
            stateMachine.NewModel(null);
        }

        public void OpenTune()
        {
            stateMachine.LoadModel();
        }

        public void SaveTune()
        {
            stateMachine.Save();
        }

        public void SaveTuneAs()
        {
            stateMachine.SaveAs();
        }

        public void Undo()
        {
            if (false == stateMachine.CanUndo)
                return;

            stateMachine.Undo();
        }

        public void Redo()
        {
            if (false == stateMachine.CanRedo)
                return;

            stateMachine.Redo();
        }

        public void PlaySelectedRange()
        {
            var buffer = Mixer.SampleAudio(stateMachine.CurrentModelState,
                playbackStart, playbackEnd);

            var stream = new MemoryStream();
            stream.WriteWaveFile(48000, buffer);
            stream.Seek(0, SeekOrigin.Begin);

            player.Stop();
            player.Stream = stream;
            player.Play();
        }

        public void StopPlayback()
        {
            player.Stop();
        }

        public bool CanClose()
        {
            return stateMachine.CanClose();
        }

        public void EditLength()
        {
            var editor = new LengthEditor()
            {
                Length = stateMachine.CurrentModelState.Length
            };
            if (editor.ShowDialog() == DialogResult.OK)
            {
                stateMachine.Do((model) => model.SetLength(editor.Length.Value));
            }
        }

        public void IncreaseSkip()
        {
            if (skip >= 3000) return;
            skip += 1;
            UpdateScrollBarRange();
            editorPanel.InvalidateContent();
        }

        public void DecreaseSkip()
        {
            if (skip <= 1) return;
            skip -= 1;
            UpdateScrollBarRange();
            editorPanel.InvalidateContent();
        }

        public void AddTrack()
        {
            var model = stateMachine.CurrentModelState;
            if (model.NumTracks == Recording.MaxTracks)
                return;
            stateMachine.Do(mdl => mdl.AddTrack());
        }
    }

    class NoteBoxHighlightFeedback : IWysiwygFeedbackStrategy
    {
        int track;
        int screenPosition;
        int trackPosition;
        int boxSize;

        public NoteBoxHighlightFeedback(int track, int screenPosition,
            int trackPosition, int boxSize)
        {
            this.track = track;
            this.screenPosition = screenPosition;
            this.trackPosition = trackPosition;
            this.boxSize = boxSize;
        }

        public bool Equals(IWysiwygFeedbackStrategy other)
        {
            var that = other as NoteBoxHighlightFeedback;
            if (that == null)
                return false;

            return this.track == that.track &&
                this.screenPosition == that.screenPosition &&
                this.trackPosition == that.trackPosition &&
                this.boxSize == that.boxSize;
        }

        public void Render(Graphics g, Rectangle clipRectangle)
        {
            var state = g.Save();
            g.TranslateTransform(screenPosition * boxSize, track * boxSize);

            g.DrawRectangle(Pens.Cyan, 0, 0, boxSize - 1, boxSize - 1);

            using (var font = new Font(FontFamily.GenericMonospace, 10))
            {
                g.DrawString(LengthEditor.NotePositionToString(trackPosition),
                    font, Brushes.Cyan, 0, 0);
                g.DrawString(trackPosition.ToString(CultureInfo.InvariantCulture),
                    font, Brushes.Cyan, 0, 16);
            }

            g.Restore(state);
        }
    }
}

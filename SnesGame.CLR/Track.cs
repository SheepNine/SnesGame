using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;

namespace SnesGame.CLR
{
    class Track
    {
        private List<Note> notes;
        private List<int> notePositions;

        public Track()
        {
            notes = new List<Note>();
            notePositions = new List<int>();
        }

        public Note this[int position]
        {
            get
            {
                var index = notePositions.IndexOf(position);
                return index == -1 ? null : notes[index];
            }
        }

        public Track Clone()
        {
            var result = new Track();
            result.notes.AddRange(notes);
            result.notePositions.AddRange(notePositions);
            return result;
        }
        public void Serialize(JsonWriter writer)
        {
            writer.WriteStartArray();
            foreach (var i in Enumerable.Range(0, notes.Count))
            {
                writer.WriteStartObject();
                writer.WritePropertyName("position");
                writer.WriteValue(notePositions[i]);
                writer.WritePropertyName("note");
                SerializeNote(writer, notes[i]);
                writer.WriteEndObject();
            }
            writer.WriteEndArray();
        }

        public static Track Deserialize(JArray data)
        {
            var result = new Track();
            foreach (JObject note in data)
            {
                result.notePositions.Add((int)note["position"]);
                result.notes.Add(DeserializeNote((JObject)note["note"]));
                if (result.notePositions.Count > 1 &&
                    result.notePositions[result.notePositions.Count - 2] >= result.notePositions[result.notePositions.Count - 1])
                    throw new InvalidDataException("positions not in order");
            }
            return result;
        }

        private void SerializeNote(JsonWriter writer, Note note)
        {
            if (note is SquareNote)
            {
                var cast = note as SquareNote;
                writer.WriteStartObject();
                writer.WritePropertyName("type"); writer.WriteValue("square");
                writer.WritePropertyName("max_length"); writer.WriteValue(cast.MaxLength);
                writer.WritePropertyName("period_min"); writer.WriteValue(cast.PeriodMin);
                writer.WritePropertyName("period_max"); writer.WriteValue(cast.PeriodMax);
                writer.WritePropertyName("period_shift"); SerializeShiftParameters(writer, cast.PeriodShiftParameters);
                writer.WritePropertyName("volume_min"); writer.WriteValue(cast.VolumeMin);
                writer.WritePropertyName("volume_max"); writer.WriteValue(cast.VolumeMax);
                writer.WritePropertyName("volume_shift"); SerializeShiftParameters(writer, cast.VolumeShiftParameters);
                writer.WriteEndObject();
            }
            else if (note is Noise)
            {
                var cast = note as Noise;
                writer.WriteStartObject();
                writer.WritePropertyName("type"); writer.WriteValue("noise");
                writer.WritePropertyName("initial_register"); writer.WriteValue(cast.InitialRegister);
                writer.WritePropertyName("tap_mask"); writer.WriteValue(cast.TapMask);
                writer.WritePropertyName("max_length"); writer.WriteValue(cast.MaxLength);
                writer.WritePropertyName("speed"); writer.WriteValue(cast.Speed);
                writer.WriteEndObject();
            }
            else
            {
                throw new NotImplementedException();
            }
        }

        private static Note DeserializeNote(JObject data)
        {
            switch ((string)data["type"])
            {
                case "square":
                    return new SquareNote((ushort)data["max_length"],
                        (byte)data["volume_min"], (byte)data["volume_max"], DeserializeShiftParameters((JObject)data["volume_shift"]),
                        (ushort)data["period_min"], (ushort)data["period_max"], DeserializeShiftParameters((JObject)data["period_shift"]));
                case "noise":
                    return new Noise(
                        (ushort)data["initial_register"], (ushort)data["tap_mask"],
                        (ushort)data["max_length"], (byte)data["speed"]);
                default:
                    throw new NotImplementedException();
            }
        }

        private static void SerializeShiftParameters(JsonWriter writer, ShiftParameters parameters)
        {
            writer.WriteStartObject();
            writer.WritePropertyName("direction"); writer.WriteValue(parameters.Direction);
            writer.WritePropertyName("edge_behaviour"); writer.WriteValue(parameters.EdgeBehaviour);
            writer.WritePropertyName("speed"); writer.WriteValue(parameters.Speed);
            writer.WriteEndObject();
        }

        private static ShiftParameters DeserializeShiftParameters(JObject data)
        {
            return new ShiftParameters(
                (ShiftDirection)(int)data["direction"],
                (byte)data["speed"],
                (EdgeBehaviour)(int)data["edge_behaviour"]);
        }

        public int LastNotePosition
        {
            get
            {
                if (notePositions.Count == 0) return 0;
                return notePositions[notePositions.Count - 1];
            }
        }

        public void AddNote(int notePosition, Note note)
        {
            if (note == null)
                throw new ArgumentNullException("note");
            if (notePosition < 0)
                throw new ArgumentOutOfRangeException("notePosition");
            if (notePositions.Contains(notePosition))
                throw new InvalidOperationException("A note is already present at the specified position");

            var insertionIndex = notePositions.Count(i => i < notePosition);
            notes.Insert(insertionIndex, note);
            notePositions.Insert(insertionIndex, notePosition);
        }

        public void RemoveNote(int notePosition)
        {
            var removalIndex = notePositions.IndexOf(notePosition);
            if (removalIndex == -1)
                throw new InvalidOperationException("No note is present at the specified position");

            notes.RemoveAt(removalIndex);
            notePositions.RemoveAt(removalIndex);
        }

        public void EditNote(int notePosition, Note note)
        {
            if (note == null)
                throw new ArgumentNullException("note");

            var updateIndex = notePositions.IndexOf(notePosition);
            if (updateIndex == -1)
                throw new InvalidOperationException("No note is present at the specified position");

            notes[updateIndex] = note;
        }

        public void MoveNote(int oldPosition, int newPosition)
        {
            if (!notePositions.Contains(oldPosition))
                throw new InvalidOperationException("No note is present at the specified position");
            if (newPosition < 0)
                throw new ArgumentOutOfRangeException("newPosition");
            if (notePositions.Contains(newPosition))
                throw new InvalidOperationException("A note is already present at the specified position");

            var note = notes[notePositions.IndexOf(oldPosition)];
            RemoveNote(oldPosition);
            AddNote(newPosition, note);
        }

        public void RemoveSection(int position, int size)
        { 
            if (position < 0)
                throw new ArgumentOutOfRangeException("position");
            if (size < 1)
                throw new ArgumentOutOfRangeException("size");

            foreach (var i in Enumerable.Range(0, notes.Count).Reverse())
            {
                var notePosition = notePositions[i];
                if (notePosition >= position && notePosition < position + size)
                    RemoveNote(notePosition);
            }
            notePositions = notePositions.Select(i => i > position ? i - size : i).ToList();
        }
    }
}

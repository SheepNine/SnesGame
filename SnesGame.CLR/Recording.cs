using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;

namespace SnesGame.CLR
{
    public interface ReadOnlyRecording
    {
        int Length { get; }
        int NumTracks { get; }
        Note this[int trackIndex, int noteIndex] { get; }
    }

    public class Recording : ReadOnlyRecording
    {
        public static readonly int MaxTracks = 8;
        private int length;
        private List<Track> tracks;

        public Recording()
        {
            tracks = new List<Track>();
            tracks.Add(new Track());
            length = 50;
        }

        public int Length
        {
            get { return length; }
        }

        public int NumTracks
        {
            get { return tracks.Count; }
        }

        public Note this[int trackIndex, int noteIndex]
        {
            get { return tracks[trackIndex][noteIndex]; }
        }

        public Recording Clone()
        {
            var result = new Recording();
            result.length = length;
            result.tracks = tracks.Select(track => track.Clone()).ToList();
            return result;
        }

        public void Serialize(Stream destination)
        {
            using (var textWriter = new StreamWriter(destination))
            using (var jsonWriter = new JsonTextWriter(textWriter)
            {
                Formatting = Formatting.None
            })
                Serialize(jsonWriter);
        }

        public void Serialize(JsonWriter writer)
        {
            writer.WriteStartObject();
            writer.WritePropertyName("length");
            writer.WriteValue(length);
            writer.WritePropertyName("tracks");
            writer.WriteStartArray();
            foreach (var track in tracks)
                track.Serialize(writer);
            writer.WriteEndArray();
            writer.WriteEndObject();
        }

        public static Recording Deserialize(Stream source)
        {
            using (var textReader = new StreamReader(source))
            using (var jsonReader = new JsonTextReader(textReader))
                return Deserialize(JObject.Load(jsonReader));
        }

        public static Recording Deserialize(JObject data)
        {
            var result = new Recording();

            result.length = (int)data["length"];
            if (result.length < 1)
                throw new InvalidDataException("Length too short");

            result.tracks.Clear();
            foreach (JArray array in data["tracks"])
                result.tracks.Add(Track.Deserialize(array));
            if (result.tracks.Count < 1)
                throw new InvalidDataException("No tracks");
            if (result.tracks.Count > 8)
                throw new InvalidDataException("Too many tracks");
            foreach (var track in result.tracks)
                if (track.LastNotePosition >= result.length)
                    throw new InvalidDataException("Note positioned off track");
            return result;
        }

        public void AddTrack()
        {
            if (tracks.Count == MaxTracks)
                throw new InvalidOperationException("Recording track limit exceeded");

            tracks.Add(new Track());
        }

        public void RemoveTrack(int trackIndex)
        {
            if (trackIndex < 0 || trackIndex >= tracks.Count)
                throw new ArgumentOutOfRangeException("trackIndex");

            tracks.RemoveAt(trackIndex);
        }

        public void SetLength(int newLength)
        {
            if (newLength < 1) throw new ArgumentOutOfRangeException("length");
            if (newLength == length) return;
            if (newLength < length)
            {
                int removed = length - newLength;
                foreach (var track in tracks)
                    track.RemoveSection(newLength, removed);
            }
            length = newLength;
        }

        public void AddNote(int trackIndex, int notePosition, Note note)
        {
            if (trackIndex < 0 || trackIndex >= tracks.Count)
                throw new ArgumentOutOfRangeException("trackIndex");
            if (notePosition >= length)
                throw new ArgumentOutOfRangeException("notePosition");

            tracks[trackIndex].AddNote(notePosition, note);
        }

        public void RemoveNote(int trackIndex, int notePosition)
        {
            if (trackIndex < 0 || trackIndex >= tracks.Count)
                throw new ArgumentOutOfRangeException("trackIndex");
            if (notePosition >= length)
                throw new ArgumentOutOfRangeException("notePosition");

            tracks[trackIndex].RemoveNote(notePosition);
        }

        public void EditNote(int trackIndex, int notePosition, Note note)
        {
            if (trackIndex < 0 || trackIndex >= tracks.Count)
                throw new ArgumentOutOfRangeException("trackIndex");
            if (notePosition >= length)
                throw new ArgumentOutOfRangeException("notePosition");

            tracks[trackIndex].EditNote(notePosition, note);
        }

        public void MoveNote(int trackIndex, int oldPosition, int newPosition)
        {
            if (trackIndex < 0 || trackIndex >= tracks.Count)
                throw new ArgumentOutOfRangeException("trackIndex");
            if (newPosition >= length)
                throw new ArgumentOutOfRangeException("notePosition");

            tracks[trackIndex].MoveNote(oldPosition, newPosition);
        }
    }
}

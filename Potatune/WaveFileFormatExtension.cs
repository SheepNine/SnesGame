using System;
using System.IO;
using System.Text;

namespace Potatune
{
    /// <summary>
    /// Provides a set of static methods for generating WAVE files.
    /// </summary>
    static class WaveFileFormatExtension
    {
        // --- This class currently only supports one format: 16-bit, mono PCM ---
        const int BYTES_PER_SAMPLE = 2;
        const int BITS_PER_SAMPLE = 8 * BYTES_PER_SAMPLE;
        const int NUM_CHANNELS = 1;
        const int PCM_FORMAT = 1;

        /// <summary>
        /// Writes 16-bit mono PCM data as a WAVE file format.
        /// </summary>
        /// <param name="destination">The stream to which data is written.</param>
        /// <param name="samplesPerSecond">The number of data samples per second.</param>
        /// <param name="pcmData">The PCM data to write.</param>
        public static void WriteWaveFile(
            this Stream destination, int samplesPerSecond, short[] pcmData)
        {
            if (null == destination)
                throw new ArgumentNullException(nameof(destination));
            if (48000 < samplesPerSecond || 0 >= samplesPerSecond)
                throw new ArgumentOutOfRangeException(nameof(samplesPerSecond));
            if (null == pcmData)
                throw new ArgumentNullException(nameof(pcmData));

            var writer = new BinaryWriter(destination);

            // --- RIFF chunk ---

            writer.Write(Encoding.ASCII.GetBytes("RIFF"));
            writer.Write((uint)(36 + (pcmData.Length * BYTES_PER_SAMPLE * NUM_CHANNELS)));
            writer.Write(Encoding.ASCII.GetBytes("WAVE"));


            // --- fmt chunk ---

            writer.Write(Encoding.ASCII.GetBytes("fmt "));
            writer.Write((uint)16);

            // wFormatTag
            writer.Write((short)PCM_FORMAT);
            // nChannels
            writer.Write((short)NUM_CHANNELS);
            // nSamplesPerSec
            writer.Write((int)samplesPerSecond);
            // nAvgBytesPerSec
            writer.Write((int)samplesPerSecond * BYTES_PER_SAMPLE * NUM_CHANNELS);
            // nBlockAlign
            writer.Write((short)(BYTES_PER_SAMPLE * NUM_CHANNELS));
            // wBitsPerSample
            writer.Write((short)BITS_PER_SAMPLE);


            // --- data chunk ---

            writer.Write(Encoding.ASCII.GetBytes("data"));
            writer.Write((uint)(pcmData.Length * BYTES_PER_SAMPLE * NUM_CHANNELS));

            foreach (var datum in pcmData)
                writer.Write(datum);
        }
    }
}

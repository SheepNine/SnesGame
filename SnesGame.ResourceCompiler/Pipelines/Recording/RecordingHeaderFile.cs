using System.IO;
using System.Linq;

namespace SnesGame.ResourceCompiler.Pipelines.Recording
{
    public static class RecordingHeaderFile
    {
        public static void WriteRecordingHeaderFile(this Manifest manifest)
        {
            PipelineUtils.WriteFileIfStale(
                manifest.GetRecordings().Select(wb => wb.SourcePath),
                Path.Combine(manifest.OutputDirectory, "recordings.h"),
                writer =>
                {
                    writer.WriteLine("#pragma once");
                    writer.WriteLine("#include \"snes_devkit.h\"");
                    writer.WriteLine();

                    int recordingIndex = 0;
                    foreach (var recording in manifest.GetRecordings())
                    {
                        writer.WriteLine("#define REC_{0} {1}", recording.ID, recordingIndex++);
                    }

                    writer.WriteLine();
                    writer.WriteLine("hRecord loadRecording(Uint16 recId);");
                });
        }
    }
}

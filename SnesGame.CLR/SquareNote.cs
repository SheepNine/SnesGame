using System;

namespace SnesGame.CLR
{
    public class SquareNote : Note
    {
        public ushort MaxLength { get; private set; }
        public byte VolumeMin { get; private set; }
        public byte VolumeMax { get; private set; }
        public ShiftParameters VolumeShiftParameters { get; private set; }
        public ushort PeriodMin { get; private set; }
        public ushort PeriodMax { get; private set; }
        public ShiftParameters PeriodShiftParameters { get; private set; }


        public SquareNote(ushort maxLength,
            byte volumeMin, byte volumeMax, ShiftParameters volumeShiftParameters,
            ushort periodMin, ushort periodMax, ShiftParameters periodShiftParameters)
        {
            if (volumeMin < 1 || volumeMin > 8)
                throw new ArgumentOutOfRangeException("volumeMin");
            if (volumeMax < 1 || volumeMax > 8)
                throw new ArgumentOutOfRangeException("volumeMax");
            if (volumeMin > volumeMax)
                throw new ArgumentException("volume envelope inverted");

            // 16 is the lowest that a channel can emit
            // 1200 is highest that the human ear can hear
            if (periodMin < 16 || periodMin > 1200)
                throw new ArgumentOutOfRangeException("periodMin");
            if (periodMax < 16 || periodMax > 1200)
                throw new ArgumentOutOfRangeException("periodMax");
            if (periodMin > periodMax)
                throw new ArgumentException("period envelope inverted");

            if (volumeShiftParameters == null)
                throw new ArgumentNullException("volumeShiftParameters");
            if ((volumeMin == volumeMax)
                    ^ (volumeShiftParameters.Direction == ShiftDirection.None))
                throw new ArgumentException("volume envelope incompatible with shift direction");

            if (periodShiftParameters == null)
                throw new ArgumentNullException("periodShiftParameters");
            if ((periodMin == periodMax)
                    ^ (periodShiftParameters.Direction == ShiftDirection.None))
                throw new ArgumentException("period envelope incompatible with shift direction");

            MaxLength = maxLength;
            VolumeMin = volumeMin;
            VolumeMax = volumeMax;
            VolumeShiftParameters = volumeShiftParameters;
            PeriodMin = periodMin;
            PeriodMax = periodMax;
            PeriodShiftParameters = periodShiftParameters;
        }
    }
}

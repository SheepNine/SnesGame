﻿using System;

namespace SnesGame.CLR
{
    public class Noise : INote
    {
        public ushort InitialRegister { get; private set; }
        public ushort TapMask { get; private set; }
        public ushort MaxLength { get; private set; }
        public byte Speed { get; private set; }

        public Noise(ushort initialRegister, ushort tapMask, ushort maxLength, byte speed)
        {
            if (initialRegister == 0)
                throw new ArgumentOutOfRangeException(nameof(initialRegister));
            if (tapMask != 0x2 && tapMask != 0x40)
                throw new ArgumentOutOfRangeException(nameof(tapMask));

            InitialRegister = initialRegister;
            TapMask = tapMask;
            MaxLength = maxLength;
            Speed = speed;
        }
    }
}

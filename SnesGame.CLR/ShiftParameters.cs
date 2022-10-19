namespace SnesGame.CLR
{
    public class ShiftParameters
    {
        public ShiftDirection Direction { get; private set; }
        public byte Speed { get; private set; }
        public EdgeBehaviour EdgeBehaviour { get; private set; }

        public ShiftParameters(ShiftDirection direction, byte speed, EdgeBehaviour edgeBehaviour)
        {
            Direction = direction;
            if (direction == ShiftDirection.None)
            {
                Speed = 0;
                EdgeBehaviour = EdgeBehaviour.SilenceChannel;
            }
            else
            {
                Speed = speed;
                EdgeBehaviour = edgeBehaviour;
            }
        }

        public static ShiftParameters None
        {
            get { return new ShiftParameters(ShiftDirection.None, 0, EdgeBehaviour.SilenceChannel); }
        }
    }
}

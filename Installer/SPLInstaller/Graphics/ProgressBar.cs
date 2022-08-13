namespace SPLInstaller.Graphics
{
    internal sealed class ProgressBar
    {
        private readonly int max = 100;
        private const int Blocks = 10;
        private const char Fill = '\u2588';
        private const char Unfilled = ' ';

        private float progress;

        public void UpdateProgress(int progress)
        {
            this.progress = progress;
        }

        public void Draw((int left, int top) pos)
        {
            (int left, int top) = Console.GetCursorPosition();

            Console.SetCursorPosition(pos.left, pos.top);

            float percent = ((progress / max) * 100f);
            int prog = (int)(percent / Blocks);

            Console.Write('[');
            for (int filled = 0; filled < prog; filled++)
            {
                Console.Write(Fill);
            }
            for (int unfilled = prog; unfilled < Blocks; unfilled++)
            {
                Console.Write(Unfilled);
            }

            Console.Write(']');
            Console.SetCursorPosition(left, top);
        }

        public ProgressBar(int max)
        {
            this.max = max;
        }
    }
}

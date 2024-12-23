using System;
using System.Windows.Forms;

namespace Potatune
{
    static class Program
    {
        [STAThread]
        public static void Main(string[] args)
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);

            using (var viewModel = new ViewModel())
                viewModel.Run();
        }
    }
    // Remove existing track
    // Swap tracks
    // Move note
    // Selection/group editing
    // Copy/paste?
    // 'comments' in the track
}

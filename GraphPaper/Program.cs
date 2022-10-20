using System;
using System.Windows.Forms;
using UnaryHeap.GUI;

namespace GraphPaper
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static int Main(string[] arguments)
        {
            return ErrorReporting.ErrorHandlingMain(arguments, (args) =>
            {
                Application.EnableVisualStyles();
                Application.SetCompatibleTextRenderingDefault(false);

                using (var viewModel = new ViewModel())
                    viewModel.Run();

                return 0;
            });
        }
    }
}

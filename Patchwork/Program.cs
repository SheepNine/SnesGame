using System;
using System.Windows.Forms;
using UnaryHeap.GUI;

namespace Patchwork
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

                var settings = new SettingsLocker(Properties.Settings.Default);

                using (var viewModel = new ViewModel())
                    viewModel.Run(settings);

                settings.Persist();
                return 0;
            });
        }
    }
}

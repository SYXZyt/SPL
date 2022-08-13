using Microsoft.Win32;
using SPLInstaller.Graphics;
using System.IO.Compression;

using File = System.IO.File;

namespace SPLInstaller
{
    internal static class Program
    {
        private static bool isAlreadyBeenInstalled = false;
        private static readonly byte[] content = Properties.Resources.content;

        private static readonly ProgressBar progressBar;
        private static DirectoryInfo path;

        private static (int left, int top) consolePosition;

        private static void CheckRegistry()
        {
            string path = "HKEY_LOCAL_MACHINE\\SOFTWARE\\SYXZ\\SPL";
            string key = "path";
            isAlreadyBeenInstalled = Registry.GetValue(path, key, null) is not null;
        }

        private static void WriteRegistry()
        {
            progressBar.Draw((consolePosition.left, consolePosition.top));

            Registry.SetValue("HKEY_LOCAL_MACHINE\\SOFTWARE\\SYXZ\\SPL", "path", $"{path.FullName}");

            progressBar.UpdateProgress(3);
            progressBar.Draw((consolePosition.left, consolePosition.top));
        }

        private static void WriteProgramFiles(bool hideProgress = false)
        {
            //In order to extract the data, we must copy the zip to a temp file
            if (!hideProgress)
            {
                progressBar.Draw((consolePosition.left, consolePosition.top));
            }

            if (File.Exists("temp.zip")) File.Delete("temp.zip");

            FileStream fs = new("temp.zip", FileMode.CreateNew);
            foreach (byte b in content)
            {
                fs.WriteByte(b);
            }
            fs.Close();

            if (!hideProgress)
            {
                progressBar.UpdateProgress(1);
                progressBar.Draw((consolePosition.left, consolePosition.top));
            }

            //Now we have extracted the zip file from memory into a file, we can then extract it before cleaning up our temp file
            string prefix = "C:/Program Files/SYXZ/SPL/";
            string[] files = new string[]
            {
                "readme.md",
                "bytecode.md",
                "SPL.exe",
            };
            foreach (string file in files)
            {
                string fPath = prefix + file;
                if (File.Exists(fPath))
                {
                    File.Delete(fPath);
                }
            }

            ZipFile.ExtractToDirectory("temp.zip", path.FullName);
            
            File.Delete("temp.zip");

            if (!hideProgress)
            {
                progressBar.UpdateProgress(2);
                progressBar.Draw((consolePosition.left, consolePosition.top));
            }
        }

        private static void WritePath()
        {
            progressBar.Draw((consolePosition.left, consolePosition.top));

            string name = "PATH";
            EnvironmentVariableTarget scope = EnvironmentVariableTarget.Machine;
            string oldValue = Environment.GetEnvironmentVariable(name, scope);
            string newValue = $"{oldValue};{path.FullName}";
            Environment.SetEnvironmentVariable(name, newValue, scope);

            progressBar.UpdateProgress(4);
            progressBar.Draw((consolePosition.left, consolePosition.top));
        }

        private static void CreateShortcut()
        {
            progressBar.Draw((consolePosition.left, consolePosition.top));

            string shortcutFolder = Environment.GetFolderPath(Environment.SpecialFolder.StartMenu).Replace("\\", "/");
            Shortcut.CreateShortcut(shortcutFolder + "/SPL.lnk", path.FullName + "/SPL.exe", path.FullName, "Launch SPL terminal");

            progressBar.UpdateProgress(5);
            progressBar.Draw((consolePosition.left, consolePosition.top));
        }

        private static void GetInstalledPath()
        {
            string _path = Registry.GetValue("HKEY_LOCAL_MACHINE\\SOFTWARE\\SYXZ\\SPL", "path", null) as string;

            if (_path is not null)
            {
                path = new(_path);
            }
            else
            {
                path = null;
            }
        }

        private static void Main()
        {
            CheckRegistry();

            if (!ConfirmationAndInput.ConfirmInstall()) return;

            if (!isAlreadyBeenInstalled)
            {
                path = Directory.CreateDirectory(ConfirmationAndInput.GetInstallPath());

                Console.CursorVisible = false;
                Console.WriteLine("Installing");
                consolePosition = Console.GetCursorPosition();
                WriteProgramFiles();
                WriteRegistry();
                WritePath();
                CreateShortcut();

                progressBar.UpdateProgress(5);
                progressBar.Draw((consolePosition.left, consolePosition.top));
                Console.SetCursorPosition(consolePosition.left, consolePosition.top + 2);
                Console.WriteLine("Install complete. It is recommended to run SPL through command prompt, using the SPL command");
                Console.WriteLine(@"Docs available at https:\SYXZyt.github.io\SPL\docs\index.html");
                Console.CursorVisible = false;
                Console.ReadKey();
            }
            else
            {
                Console.WriteLine("SPL has already been installed. Would you like to repair the install?");
                if (!ConfirmationAndInput.ConfirmInstall()) return;
                GetInstalledPath();
                if (path is null)
                {
                    Console.WriteLine("Could not find the installed path");
                    return;
                }

                WriteProgramFiles(true);
            }

            if (File.Exists("temp.zip")) File.Delete("temp.zip");
        }

        static Program()
        {
            progressBar = new(5);
        }
    }
}
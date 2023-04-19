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

        private static DirectoryInfo path;

        private static void CheckRegistry()
        {
            string path = "HKEY_LOCAL_MACHINE\\SOFTWARE\\SYXZ\\SPL";
            string key = "path";
            isAlreadyBeenInstalled = Registry.GetValue(path, key, null) is not null;
        }

        private static void WriteRegistry()
        {
            Registry.SetValue("HKEY_LOCAL_MACHINE\\SOFTWARE\\SYXZ\\SPL", "path", $"{path.FullName}");
        }

        private static void WriteProgramFiles()
        {
            if (File.Exists("temp.zip")) File.Delete("temp.zip");

            FileStream fs = new("temp.zip", FileMode.CreateNew);
            foreach (byte b in content)
            {
                fs.WriteByte(b);
            }
            fs.Close();

            //Now we have extracted the zip file from memory into a file, we can then extract it before cleaning up our temp file
            string prefix = "C:/Program Files/SYXZ/SPL/";
            string[] files = new string[]
            {
                "README.md",
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
        }

        private static void WritePath()
        {
            string name = "PATH";
            EnvironmentVariableTarget scope = EnvironmentVariableTarget.Machine;
            string oldValue = Environment.GetEnvironmentVariable(name, scope);
            string newValue = $"{oldValue};{path.FullName}";
            Environment.SetEnvironmentVariable(name, newValue, scope);
        }

        private static void CreateShortcut()
        {
            string shortcutFolder = Environment.GetFolderPath(Environment.SpecialFolder.StartMenu).Replace("\\", "/");
            Shortcut.CreateShortcut(shortcutFolder + "/SPL.lnk", path.FullName + "/SPL.exe", path.FullName, "Launch SPL terminal");
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
            InstallationProcess process = new();
            CheckRegistry();

            if (!ConfirmationAndInput.ConfirmInstall()) return;

            if (!isAlreadyBeenInstalled)
            {
                //Now we know that we are installing from scratch, we can load up our installer with the required functions
                InstallTask writeProgramFiles = new(WriteProgramFiles);
                InstallTask writeRegistry = new(WriteRegistry);
                InstallTask writePath = new(WritePath);
                InstallTask writeShortcut = new(CreateShortcut);

                process.AddTask(writeProgramFiles);
                process.AddTask(writeRegistry);
                process.AddTask(writePath);
                process.AddTask(writeShortcut);

                path = Directory.CreateDirectory(ConfirmationAndInput.GetInstallPath());
                Console.CursorVisible = false;

                process.ExecuteProcess();
                Console.WriteLine("Docs available at https:\\SYXZyt.github.io\\SPL\\docs\\index.html\r\nPress any key");
                Console.CursorVisible = true;
                Console.ReadKey();
            }
            else
            {
                Console.WriteLine("SPL has already been installed. Would you like to update/repair the install?");
                if (!ConfirmationAndInput.ConfirmInstall()) return;

                GetInstalledPath();
                if (path is null)
                {
                    Console.WriteLine("Could not find the installed path");
                    return;
                }

                InstallTask writeProgramFiles = new(WriteProgramFiles);
                process.AddTask(writeProgramFiles);

                Console.CursorVisible = false;
                process.ExecuteProcess();
                Console.CursorVisible = true;

                if (File.Exists("temp.zip")) File.Delete("temp.zip");
            }
        }
    }
}
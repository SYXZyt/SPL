using Microsoft.Win32;

namespace SPLUninstaller
{
    internal class Program
    {
        private static void LookForSPL()
        {
            string path = "HKEY_LOCAL_MACHINE\\SOFTWARE\\SYXZ\\SPL";
            string key = "path";
            if (Registry.GetValue(path, key, null) is null)
            {
                Console.WriteLine("SPL could not be found");
                Environment.Exit(1);
            }
        }

        static void Main()
        {
            Console.WriteLine("Searching for SPL");
            LookForSPL();

            //If SPL has been found, we can begin deleting all of the things it installed
            RegistryKey key = Registry.LocalMachine.OpenSubKey(@"Software\SYXZ", true);
            key.DeleteSubKeyTree("SPL");

            string path = Environment.GetFolderPath(Environment.SpecialFolder.StartMenu);
            if (File.Exists(path + "\\SPL.lnk")) File.Delete(path + "\\SPL.lnk");
            if (File.Exists(path + "\\UninstallSPL.lnk") )File.Delete(path + "\\UninstallSPL.lnk");

            path = @"c:\Program Files\SYXZ\SPL\";
            if (File.Exists(path + "readme.md")) File.Delete(path + "readme.md");
            if (File.Exists(path + "bytecode.md")) File.Delete(path + "bytecode.md");
            if (File.Exists(path + "spl.exe")) File.Delete(path + "spl.exe");
            if (Directory.Exists(path)) Directory.Delete(path);
        }
    }
}
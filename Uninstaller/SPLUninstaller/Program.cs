using Microsoft.Win32;

namespace SPLUninstaller
{
    internal class Program
    {
        private static bool LookForSPL()
        {
            string path = "HKEY_LOCAL_MACHINE\\SOFTWARE\\SYXZ\\SPL";
            string key = "path";
            if (Registry.GetValue(path, key, null) is null)
            {
                Console.WriteLine("SPL could not be found");
                return false;
            }

            return true;
        }

        static void Main()
        {
            Console.WriteLine("Searching for SPL");
            if (!LookForSPL())
            {
                return;
            }

            Console.WriteLine("SPL Found");

            //If SPL has been found, we can begin deleting all of the things it installed
            RegistryKey key = Registry.LocalMachine.OpenSubKey(@"Software\SYXZ", true);
            key.DeleteSubKeyTree("SPL");

            string path = Environment.GetFolderPath(Environment.SpecialFolder.StartMenu);
            if (File.Exists(path + "\\SPL.lnk"))
            {
                File.Delete(path + "\\SPL.lnk");
            }

            path = @"c:\Program Files\SYXZ\SPL\";
            if (File.Exists(path + "readme.md"))
            {
                Console.WriteLine($"Deleting: {path + "readme.md"}");
                File.Delete(path + "readme.md");
            }

            if (File.Exists(path + "bytecode.md"))
            {
                Console.WriteLine($"Deleting: {path + "bytecode.md"}");
                File.Delete(path + "bytecode.md");
            }

            if (File.Exists(path + "spl.exe"))
            {
                Console.WriteLine($"Deleting: {path + "spl.exe"}");
                File.Delete(path + "spl.exe");
            }

            if (Directory.Exists(path))
            {
                Console.WriteLine($"Deleting: {path}");
                Directory.Delete(path);
            }
        }
    }
}
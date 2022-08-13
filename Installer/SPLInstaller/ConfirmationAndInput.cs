namespace SPLInstaller
{
    internal static class ConfirmationAndInput
    {
        private static bool Confirm()
        {
            string key;
            bool b;
            while (true)
            {
                key = Console.ReadLine();

                if (key.ToLower() == "y")
                {
                    b = true;
                    break;
                }
                else if (key.ToLower() == "n")
                {
                    b = false;
                    break;
                }
            }
            Console.WriteLine();
            return b;
        }

        public static string GetInstallPath()
        {
            Console.WriteLine(@"Where would you like to install SPL. Leave blank for default (C:\Program Files\SYXZ\SPL\)");
            string path = Console.ReadLine();
            return path == string.Empty ? @"C:\Program Files\SYXZ\SPL\" : path;
        }

        public static bool ConfirmInstall()
        {
            Console.WriteLine("Would you like to install SPL? (y/n)");
            return Confirm();
        }
    }
}
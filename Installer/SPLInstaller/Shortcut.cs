using IWshRuntimeLibrary;

using File = System.IO.File;

namespace SPLInstaller
{
    /// <summary>
    /// Class containing methods to handle shortcut files
    /// </summary>
    internal static class Shortcut
    {
        /// <summary>
        /// Create a new shortcut file on disk
        /// </summary>
        /// <param name="shortcutFile">The final name of the shortcut. e.g. myshortcut.lnk</param>
        /// <param name="linkTo">The file that the shortcut points to</param>
        /// <param name="workingDir">The directory where the shortcut will execute. Should be the same directory that <paramref name="linkTo"/> will execute</param>
        /// <param name="desc">The description of the shortcut</param>
        public static void CreateShortcut(string shortcutFile, string linkTo, string workingDir, string desc)
        {
            WshShellClass shellClass = new();
            IWshShortcut shortcut = (IWshShortcut)shellClass.CreateShortcut(shortcutFile);
            shortcut.TargetPath = linkTo;
            shortcut.Description = desc;
            shortcut.WorkingDirectory = workingDir;
            shortcut.Save();
        }
    }
}
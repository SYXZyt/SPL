using SPLInstaller.Graphics;

namespace SPLInstaller
{
    internal sealed class InstallationProcess
    {
        private readonly List<InstallTask> tasks;

        public void AddTask(InstallTask task)
        {
            tasks.Add(task);
        }

        public void ExecuteProcess()
        {
            ProgressBar progressBar = new(tasks.Count);

            int i = 0;
            (int left, int top) consolePos = Console.GetCursorPosition();
            consolePos.top++;

            Console.WriteLine("Installing");

            foreach (InstallTask task in tasks)
            {
                task.ExecuteTask();
                progressBar.UpdateProgress(i++);
                progressBar.Draw(consolePos);
            }

            Console.WriteLine("SPL has been installed");
        }

        public InstallationProcess() => tasks = new();
    }
}
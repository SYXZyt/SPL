namespace SPLInstaller
{
    internal sealed class InstallTask
    {
        public delegate void InstallFunction();

        private readonly InstallFunction func;

        public void ExecuteTask()
        {
            func.Invoke();
        }

        public InstallTask(InstallFunction func)
        {
            this.func = func;
        }
    }
}
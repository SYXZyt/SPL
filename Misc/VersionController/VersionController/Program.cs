global using System.Net;

namespace VersionController
{
    internal struct Version
    {
        public int major;
        public int minor;
        public int patch;

        public static implicit operator Version(string s)
        {
            string[] parts = s.Split('.');
            Version v;
            v.major = int.Parse(parts[0]);
            v.minor = int.Parse(parts[1]);
            v.patch = int.Parse(parts[2]);
            return v;
        }

        public static bool operator ==(Version a, Version b) => a.Equals(b);
        public static bool operator !=(Version a, Version b) => !a.Equals(b);

        public static bool operator >(Version a, Version b)
        {
            return
                a.major > b.major ||
                a.minor > b.minor ||
                a.patch > b.patch;
        }

        public static bool operator <(Version a, Version b)
        {
            return
                a.major < b.major ||
                a.minor < b.minor ||
                a.patch < b.patch;
        }

        public override bool Equals(object obj)
        {
            if (obj is null || obj is not Version) return false;

            Version v = (Version)obj;
            return (major == v.major && minor == v.minor && patch == v.patch);
        }

        public static Version GetLatest(Version a, Version b, out bool areEqual)
        {
            areEqual = false;

            if (a == b)
            {
                areEqual = true;
                return a;
            }
            else if (a > b)
            {
                return a;
            }
            return b;
        }

        public override int GetHashCode() => ToString().GetHashCode();
        public override string ToString() => $"{major}.{minor}.{patch}";
    }

    internal static class Program
    {
        private readonly static Version currentVersion = "1.0.0";
        private const string Address = "https://syxzyt.github.io/spl/ves.txt";

        private static Version GetLatestVersion()
        {
            Version v;
            HttpClient client = new();

            using HttpResponseMessage response = client.GetAsync(Address).Result;
            using HttpContent content = response.Content;
            v = content.ReadAsStringAsync().Result;
            return v;
        }

        private static void Main(string[] args)
        {
            Version latest = GetLatestVersion();
            Version comparison = Version.GetLatest(latest, currentVersion, out bool areEqual);

            if (areEqual)
            {
                Console.WriteLine("SPL up to date");
            }
            else if (latest == comparison)
            {
                Console.WriteLine("New version of SPL available. Run the installer to update");
            }
        }
    }
}
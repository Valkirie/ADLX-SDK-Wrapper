using System.Runtime.InteropServices;
using System.Text;
using Timer = System.Timers.Timer;

namespace ADLX_Probe
{
    class Program
    {
        public const string ADLX_Wrapper = @"ADLX_Wrapper.dll";
        [DllImport(ADLX_Wrapper, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)] private static extern bool IntializeAdlx(StringBuilder dispName, int nameLength);
        [DllImport(ADLX_Wrapper, CallingConvention = CallingConvention.Cdecl)] private static extern bool InitializeAdlxWithIncompatibleDriver();
        [DllImport(ADLX_Wrapper, CallingConvention = CallingConvention.Cdecl)] public static extern bool CloseAdlx();

        private static StringBuilder displayName = new StringBuilder(256);
        private static Version ADLXVersion = new Version(1,0);

        static void Main()
        {
            Timer UpdateTimer = new Timer(2000);
            UpdateTimer.Elapsed += UpdateTimer_Elapsed;
            UpdateTimer.Start();

            // initialize ADLX
            IntializeAdlx(displayName, displayName.Capacity);

            // check version
            ADLXVersion = new Version(displayName.ToString());
            if (ADLXVersion.Major <= 1 && ADLXVersion.Minor <= 0)
                Environment.Exit(0); // Failed, version is too old

            while (true)
                Thread.Sleep(500);
        }

        private static void UpdateTimer_Elapsed(object? sender, System.Timers.ElapsedEventArgs e)
        {
            if (sender is Timer timer)
                timer.Stop();

            // close ADLX
            CloseAdlx();

            Console.WriteLine(ADLXVersion.ToString());
            Environment.Exit(2); // Success
        }
    }
}
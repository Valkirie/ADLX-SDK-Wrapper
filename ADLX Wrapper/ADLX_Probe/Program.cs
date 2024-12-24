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

        static StringBuilder displayName = new StringBuilder(256);
        static void Main()
        {
            Timer UpdateTimer = new Timer(1000);
            UpdateTimer.Elapsed += UpdateTimer_Elapsed;
            UpdateTimer.Start();

            // try
            IntializeAdlx(displayName, displayName.Capacity);

            while(true)
                Thread.Sleep(1000);
        }

        private static void UpdateTimer_Elapsed(object? sender, System.Timers.ElapsedEventArgs e)
        {
            Console.WriteLine(displayName.ToString());
            Environment.Exit(2); // Success
        }
    }
}
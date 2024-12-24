using System.Runtime.InteropServices;

namespace ADLX_Probe
{
    class Program
    {
        public const string ADLX_Wrapper = @"ADLX_Wrapper.dll";
        [DllImport(ADLX_Wrapper, CallingConvention = CallingConvention.Cdecl)] private static extern bool IntializeAdlx();
        [DllImport(ADLX_Wrapper, CallingConvention = CallingConvention.Cdecl)] private static extern bool InitializeAdlxWithIncompatibleDriver();
        [DllImport(ADLX_Wrapper, CallingConvention = CallingConvention.Cdecl)] public static extern bool CloseAdlx();

        static void Main()
        {
            try
            {
                if (IntializeAdlx())
                {
                    CloseAdlx();
                    Environment.Exit(0); // Success
                }
            }
            catch { }

            Environment.Exit(1); // Error
        }
    }
}
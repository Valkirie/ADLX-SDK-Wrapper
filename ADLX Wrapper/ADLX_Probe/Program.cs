using System.Runtime.InteropServices;
using System.Text;

namespace ADLX_Probe
{
    class Program
    {
        public const string ADLX_Wrapper = @"ADLX_Wrapper.dll";
        [DllImport(ADLX_Wrapper, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)] private static extern bool IntializeAdlx(StringBuilder dispName, int nameLength);
        [DllImport(ADLX_Wrapper, CallingConvention = CallingConvention.Cdecl)] private static extern bool InitializeAdlxWithIncompatibleDriver();
        [DllImport(ADLX_Wrapper, CallingConvention = CallingConvention.Cdecl)] public static extern bool CloseAdlx();

        static void Main()
        {
            try
            {
                StringBuilder displayName = new StringBuilder(256); // Assume display name won't exceed 255 characters
                if (IntializeAdlx(displayName, displayName.Capacity))
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
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

namespace test
{
    public partial class KoWebInsp
    {
#if DEBUG
        public const string DllName = "KoWebDLL_D.dll";
#else
        public const string DllName = "KoWebDLL.dll";
#endif

        [DllImport(DllName)]
        extern private static IntPtr InitKoWebDll();

        [DllImport(DllName)]
        extern private static void CloseKoWebDll(IntPtr handle);

        [DllImport(DllName)]
        extern private static bool Initialize(IntPtr handle, int procCnt);

        [DllImport(DllName, CharSet = CharSet.Auto, CallingConvention = CallingConvention.Cdecl)]
        extern private static bool SetParam(IntPtr handle, int procIdx, string pcName, string ModelPath);

        [DllImport(DllName, CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        extern private static bool SetImage(IntPtr handle, IntPtr buf, int width, int height, int pitch);

        [DllImport(DllName)]
        extern private static void RunWebInspect(IntPtr handle, int procNum);
    }
}

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
        [StructLayout(LayoutKind.Sequential, Pack =1)]
        public struct FIND_EDGE_RESULT
        {
            int InspType;       // 검사 데이터 성격 규정
            int IsInspOK;      // 검사 결과 정보 : false(실패), true(성공)
            double StX;         // 시작 에지 정보
            double EdX;			// 끝 에지 정보
        };

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

        [DllImport(DllName, CallingConvention=CallingConvention.Cdecl)]
        extern private static void EdgeFind1(IntPtr buf, int width, int height, int pitch, int depth, double scaleX, double scaleY,
                                            int inspMethod, int prodCnt, double prodSize, double prodGap, double[] refCenterX,
                                            int edgeTH, int edgeCnt, FIND_EDGE_RESULT[] res, int bufSize, ref IntPtr returnPtr);
    }
}

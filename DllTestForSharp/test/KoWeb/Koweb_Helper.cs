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

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        extern private static int RunBasicWebInsp_T(int procNum, IntPtr image, int width, int height, int pitch, double scaleX, double scaleY, double edgeX1, double edgeX2, double edgeOffset,
                            int brightMin, int brightMax, int sizeMethod, int noInsp,
                            bool useLv1, bool useLv2, bool useLv3, bool useLv4, bool useLv5, bool useLv6, bool useLv7, bool useLv8, bool useLv9, bool useLv10,
                            float cycVal1, float cycVal2, float cycVal3, float cycVal4, float cycVal5, float cycVal6, float cycVal7, float cycVal8, float cycVal9, float cycVal10,
                            int upAreaTh, int lvUpTh1, int lvUpTh2, int lvUpTh3, int lvUpTh4, int lvUpTh5, int lvUpTh6, int lvUpTh7, int lvUpTh8, int lvUpTh9, int lvUpTh10,
                            double upSize1, double upSize2, double upSize3, double upSize4, double upSize5, double upSize6, double upSize7, double upSize8, double upSize9, double upSize10,
                            int dnAreaTh, int lvDnTh1, int lvDnTh2, int lvDnTh3, int lvDnTh4, int lvDnTh5, int lvDnTh6, int lvDnTh7, int lvDnTh8, int lvDnTh9, int lvDnTh10,
                            double dnSize1, double dnSize2, double dnSize3, double dnSize4, double dnSize5, double dnSize6, double dnSize7, double dnSize8, double dnSize9, double dnSize10,
                            int cnInspArea, double cnCandiVal, double cnStd_1, double cnStd_2, double cnStd_3, double cnStd_4, double cnStd_5, double cnStd_6, double cnStd_7, double cnStd_8, double cnStd_9, double cnStd_10,
                            int psUpTh, int psDnTh, int psUpAreaTh, int psDnAreaTh, int psVal1, int psVal2, int psVal3, int psVal4, int psVal5, int psVal6, int psVal7, int psVal8, int psVal9, int psVal10,
                            double psSize1, double psSize2, double psSize3, double psSize4, double psSize5, double psSize6, double psSize7, double psSize8, double psSize9, double psSize10,
                            int scVal1, int ScVal2, int ScVal3, int ScVal4, int ScVal5, int ScVal6, int ScVal7, int ScVal8, int ScVal9, int ScVal10);
    }
}

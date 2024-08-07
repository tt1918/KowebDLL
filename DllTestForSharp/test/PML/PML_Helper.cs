using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace test
{
    public partial class PML
    {
        [DllImport("GeoMatchLib.dll")]
        extern private static IntPtr PmlInit();

        [DllImport("GeoMatchLib.dll")]
        extern private static IntPtr PmlClose(IntPtr handle);

        [DllImport("GeoMatchLib.dll")]
        extern private static void SetScore(IntPtr handle, float value);

        [DllImport("GeoMatchLib.dll")]
        extern private static float GetScore(IntPtr handle);

        [DllImport("GeoMatchLib.dll")]
        extern private static void SetGreedness(IntPtr handle, float value);

        [DllImport("GeoMatchLib.dll")]
        extern private static float GetGreedness(IntPtr handle);

        [DllImport("GeoMatchLib.dll")]
        extern private static void SetMaxOverlap(IntPtr handle, float value);

        [DllImport("GeoMatchLib.dll")]
        extern private static float GetMaxOverlap(IntPtr handle);

        [DllImport("GeoMatchLib.dll")]
        extern private static void SetMaxContrast(IntPtr handle, int value);

        [DllImport("GeoMatchLib.dll")]
        extern private static int GetMaxContrast(IntPtr handle);

        [DllImport("GeoMatchLib.dll")]
        extern private static void SetMinContrast(IntPtr handle, int value);

        [DllImport("GeoMatchLib.dll")]
        extern private static int GetMinContrast(IntPtr handle);

        [DllImport("GeoMatchLib.dll")]
        extern private static void SetAngleRange(IntPtr handle, int start, int end);

        [DllImport("GeoMatchLib.dll")]
        extern private static int GetStartAngle(IntPtr handle);

        [DllImport("GeoMatchLib.dll")]
        extern private static int GetEndAngle(IntPtr handle);

        [DllImport("GeoMatchLib.dll")]
        extern private static void SetAngleStep(IntPtr handle, int value);

        [DllImport("GeoMatchLib.dll")]
        extern private static int GetAngleStep(IntPtr handle);

        [DllImport("GeoMatchLib.dll")]
        extern private static void SetSearchStep(IntPtr handle, int value);

        [DllImport("GeoMatchLib.dll")]
        extern private static int GetSearchStep(IntPtr handle);

        [DllImport("GeoMatchLib.dll")]
        extern private static void SetPatternImage(IntPtr handle, IntPtr buf, int width, int height);

        [DllImport("GeoMatchLib.dll")]
        extern private static bool CreateGeoMatchModel(IntPtr handle);

        [DllImport("GeoMatchLib.dll")]
        extern private static void SetProcessBuffer(IntPtr handle, int width, int height);

        [DllImport("GeoMatchLib.dll")]
        extern private static bool FindGeoMatchModel(IntPtr handle, IntPtr buf, int width, int height, ref int ptX, ref int ptY, ref double score, ref double angle);
    }
}

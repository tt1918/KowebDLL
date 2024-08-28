using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace test
{
    public partial class KoWebInsp
    {
        private IntPtr _KowebHandle;

        public bool Init()
        {
            _KowebHandle = InitKoWebDll();

            if (_KowebHandle == null)   return false;

            return true;
        }

        public void Destroy()
        {
            CloseKoWebDll(_KowebHandle);
        }

        public bool SetProcCnt(int procCnt)
        {
            return Initialize(_KowebHandle, procCnt);
        }

        public bool SetParam(int procIdx, string pcName, string modelPath)
        {
            return SetParam(_KowebHandle, procIdx, pcName, modelPath);
        }

        public void SetImage(IntPtr buf, int width, int height, int pitch)
        {
            SetImage(_KowebHandle, buf, width, height, pitch);
        }

        public void RunInspect(int procIdx)
        {
            RunWebInspect(_KowebHandle, procIdx);
        }

        public void FindEdge(IntPtr buf, int width, int height, int pitch, int depth, double scaleX, double scaleY,
                                            int inspMethod, int prodCnt, double prodSize, double prodGap, double[] refCenterX,
                                            int edgeTH, int edgeCnt, ref FIND_EDGE_RESULT[] res)
        {

            //EdgeFind(buf, width, height, pitch, depth, scaleX, scaleY, inspMethod, prodCnt, prodSize, prodGap, refCenterX, edgeTH, edgeCnt, ref res);

            IntPtr returnPtr = IntPtr.Zero;
            EdgeFind1(buf, width, height, pitch, depth, scaleX, scaleY, inspMethod, prodCnt, prodSize, prodGap, refCenterX, edgeTH, edgeCnt, res, 2, ref returnPtr);

            int next = 0;
            
            for(int i=0; i<2; i++)
            {
                FIND_EDGE_RESULT resultData;
                resultData = (FIND_EDGE_RESULT)Marshal.PtrToStructure(returnPtr + next, typeof(FIND_EDGE_RESULT));

                //          FIND_EDGE_RESULT data = (FIND_EDGE_RESULT)Marshal.PtrToStructure(new IntPtr(returnPtr.ToInt64() + next), res.GetType());

                if(i<1)
                    next += Marshal.SizeOf(typeof(FIND_EDGE_RESULT));
                res[i] = resultData;
            }
        }


        public void RunBasicInsp(IntPtr buf, int width, int height, int pitch)
        {
            //for(int i=0; i<1; i++)
            
            RunBasicWebInsp_T(0, buf, width, height, pitch, 0.05, 0.05, 498, 3800, 50, 30, 230, 0, 0,
                        true, true, false, false, false, false, false, false, false, false,
                        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                        23, 35, 30, 0, 0, 0, 0, 0, 0, 0, 0,
                        0.3, 0.25, 0, 0, 0, 0, 0, 0, 0, 0,
                        20, 30, 25, 0, 0, 0, 0, 0, 0, 0, 0,
                        0.3, 0.25, 0, 0, 0, 0, 0, 0, 0, 0,
                        32, 500, 1500, 2000, 0, 0, 0, 0, 0, 0, 0, 0,
                        25, 23, 22, 20, 45, 30, 0, 0, 0, 0, 0, 0, 0, 0,
                        0.5, 0.3, 0, 0, 0, 0, 0, 0, 0, 0,
                        2000, 1500, 0, 0, 0, 0, 0, 0, 0, 0);

            RunBasicWebInsp_T(0, buf, width, height, pitch, 0.05, 0.05, 4298, 7600, 50, 30, 230, 0, 0,
                        true, true, false, false, false, false, false, false, false, false,
                        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                        23, 35, 30, 0, 0, 0, 0, 0, 0, 0, 0,
                        0.3, 0.25, 0, 0, 0, 0, 0, 0, 0, 0,
                        20, 30, 25, 0, 0, 0, 0, 0, 0, 0, 0,
                        0.3, 0.25, 0, 0, 0, 0, 0, 0, 0, 0,
                        32, 500, 1500, 2000, 0, 0, 0, 0, 0, 0, 0, 0,
                        25, 23, 22, 20, 45, 30, 0, 0, 0, 0, 0, 0, 0, 0,
                        0.5, 0.3, 0, 0, 0, 0, 0, 0, 0, 0,
                        2000, 1500, 0, 0, 0, 0, 0, 0, 0, 0);
        }

    }
}

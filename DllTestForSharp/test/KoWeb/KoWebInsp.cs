using System;
using System.Collections.Generic;
using System.Linq;
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
    }
}

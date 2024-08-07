using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using OpenCvSharp;

namespace test
{
    public partial class PML
    {
        private IntPtr pmlHnd;

        public float MinScore
        {
            get {   return GetScore(pmlHnd);    }
            set {   SetScore(pmlHnd, value);    }
        }

        public float Greedness
        {
            get {   return GetGreedness(pmlHnd);    }
            set {   SetGreedness(pmlHnd, value);    }
        }

        public float MaxOverlap
        {
            get {   return GetMaxOverlap(pmlHnd);   }
            set {   SetMaxOverlap(pmlHnd, value);   }
        }

        public int MaxContrast
        {
            get {   return GetMaxContrast(pmlHnd);  }
            set {   SetMaxContrast(pmlHnd, value);  }
        }

        public int MinContrast
        {
            get {   return GetMinContrast(pmlHnd);  }
            set {   SetMinContrast(pmlHnd, value);  }
        }

        public int StartAngle
        {
            get {   return GetStartAngle(pmlHnd);   }
            set {   SetAngleRange(pmlHnd, value, EndAngle); }
        }

        public int EndAngle
        {
            get {   return GetEndAngle(pmlHnd); }
            set {   SetAngleRange(pmlHnd, StartAngle, value); }
        }

        public int AngleStep
        {
            get {   return GetAngleStep(pmlHnd); }
            set {   SetAngleStep(pmlHnd, value); }
        }

        public int SearchStep
        {
            get {   return GetSearchStep(pmlHnd); }
            set {   SetSearchStep(pmlHnd, value); }
        }
        public void Init()
        {
            pmlHnd = PmlInit();
        }

        public void Close()
        {
            patImage?.Dispose();
            patImage = null;

            PmlClose(pmlHnd);
        }
        public int CenterX
        {
            get { return ptX; }
        }
        private int ptX;

        public int CenterY
        {
            get { return ptY; }
        }
        private int ptY;

        public double Score
        {
            get { return score; }
        }
        private double score;

        public double Angle
        {
            get { return angle; }
        }
        private double angle;

        public Mat PatternImage
        {
            get { return patImage;  }
            set
            {
                if(patImage!=null)
                {
                    patImage.Dispose();
                }
                patImage = value;
                SetPatternImage(pmlHnd, patImage.Data, patImage.Cols, patImage.Rows);
            }
        }
        private Mat patImage=null;

        public bool CreateGeoMatchModel()
        {
            return CreateGeoMatchModel(pmlHnd);
        }

        public void SetProcessBuffer(int width, int height)
        {
            SetProcessBuffer(pmlHnd, width, height);
        }

        public bool Find(OpenCvSharp.Mat input)
        {
            return FindGeoMatchModel(pmlHnd, input.Data, input.Cols, input.Rows, ref ptX, ref ptY, ref score, ref angle);
        }
    }
}

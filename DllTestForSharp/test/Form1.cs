using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using OpenCvSharp;
using ImageView;

namespace test
{
    public partial class Form1 : Form
    {
        //  Storage the texture itself.
        Mat mainImage = null;
        Bitmap mainBmp = null;

        Mat patImage = null;
        Bitmap patBmp = null;

        private KoWebInsp _Koweb = null;

        public Form1()
        {
            InitializeComponent();

            _Koweb = new KoWebInsp();
            _Koweb.Init();
            _Koweb.SetProcCnt(1);

            if(_Koweb.SetParam(0, "COS-203", "C:\\COSS\\parameter.ini")==true)
            {

            }

        }

        private void Form1_FormClosed(object sender, FormClosedEventArgs e)
        {
            if (_Koweb != null)
                _Koweb.Destroy();
        }

        private void btnLoadImage_Click(object sender, EventArgs e)
        {
            try
            {
                OpenFileDialog dlg = new OpenFileDialog();
                if (dlg.ShowDialog() == DialogResult.OK)
                {
                    mainImage?.Release();
                    mainBmp?.Dispose();
                    MainView.Image?.Dispose();

                    mainImage = Cv2.ImRead(dlg.FileName, ImreadModes.Color);
                    mainBmp?.Dispose();
                    mainBmp = OpenCvSharp.Extensions.BitmapConverter.ToBitmap(mainImage);

                    MainView.Image = mainBmp;

                    Mat grayImg = new Mat();
                    grayImg.Create(mainImage.Rows, mainImage.Cols, MatType.CV_8UC1);
                    Cv2.CvtColor(mainImage, grayImg, ColorConversionCodes.RGB2GRAY);

                    //Mat ExpendImg = new Mat(mainImage.Rows*5, mainImage.Cols*8, MatType.CV_8UC1);
                    
                    //for(int i=0; i<5; i++)
                    //{
                    //    for(int j=0; j<8; j++)
                    //    {
                    //        Mat ROI = new Mat();
                    //        Rect rect = new Rect(j * mainImage.Cols, i * mainImage.Rows, mainImage.Cols, mainImage.Rows);
                    //        ROI = ExpendImg.SubMat(rect);
                            
                    //        for(int ii=0; ii<grayImg.Rows; ii++)
                    //        {
                    //            for(int jj=0; jj<grayImg.Cols; jj++)
                    //            {
                    //                ROI.Set<byte>(ii, jj, grayImg.At<byte>(ii, jj));
                    //            }
                    //        }

                    //    }
                    //}

                    //Cv2.ImWrite("D:\\32K_10K.bmp", ExpendImg);



                    _Koweb.SetImage(grayImg.Data, grayImg.Cols, grayImg.Rows, grayImg.Cols);
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

        
        private void btnAddTracker_Click(object sender, EventArgs e)
        {
            MainView.RectTracker.Show();
        }

        private void btnTest_Click(object sender, EventArgs e)
        {
            this.MainView.RectTracker.Hide();
        }

        private void btnAddDefect_Click(object sender, EventArgs e)
        {
            Rectangle defect = new Rectangle(1000, 1000, 100, 100);
            this.MainView.AddDefectPt(defect);
        }

        private void btnClearDefect_Click(object sender, EventArgs e)
        {
            this.MainView.ClearDefectPt();

        }

        private void btnLoadModelImage_Click(object sender, EventArgs e)
        {
          
        }

        private void btnMatchImage_Click(object sender, EventArgs e)
        {
            Mat grayImg = new Mat();
            grayImg.Create(mainImage.Rows, mainImage.Cols, MatType.CV_8UC1);
            Cv2.CvtColor(mainImage, grayImg, ColorConversionCodes.RGB2GRAY);


            _Koweb.RunBasicInsp(grayImg.Data, grayImg.Cols, grayImg.Rows, grayImg.Cols);
            
            
            grayImg.Dispose();
            //return;

            //List<double> refCntX = new List<double>();
            //refCntX.Add(107.5);
            //refCntX.Add(190.0);

            //KoWebInsp.FIND_EDGE_RESULT[] res = new KoWebInsp.FIND_EDGE_RESULT[2];
            //res[0] = new KoWebInsp.FIND_EDGE_RESULT();
            //res[1] = new KoWebInsp.FIND_EDGE_RESULT();

            //_Koweb.FindEdge(grayImg.Data, grayImg.Cols, grayImg.Rows, grayImg.Cols, grayImg.Channels(), 0.05, 0.05, 1, 2, 165.0,
            //    25.0, refCntX.ToArray(), 30, 2, ref res);


            grayImg.Dispose();
        }

    }

}

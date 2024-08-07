using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace ImageView
{
    public class ViewDefault
    {
        private object owner;
        private PictureBox pbCtrl;
        private Point startPoint;
        private float zoomRatio = 1.125f;
        private int toolStripH = 25;
        private int toolStatusH = 22;

        public ViewDefault(object owner, PictureBox ctrl, int stripH, int statusH)
        {
            this.owner = owner;
            pbCtrl = ctrl;

            toolStripH = stripH;
            toolStatusH = statusH;
        }

        public void OnResize(object sender, EventArgs e)
        {
            ImgView mainForm = this.owner as ImgView;

            int width = mainForm.Width;
            int height = mainForm.Height - (toolStripH+ toolStatusH);
            if(pbCtrl.Width <= width && pbCtrl.Height <= height)
            {
                ZoomFixed();
            }
            else
            {
                pbCtrl.Location = new System.Drawing.Point((width - pbCtrl.Width) / 2, (height - pbCtrl.Height) / 2 + toolStripH);
            }
        }

        public void OnMouseDown(object sender, MouseEventArgs e)
        {
            startPoint = e.Location;
        }

        public void OnMouseMove(object sender, MouseEventArgs e)
        {
            pbCtrl.Location = new System.Drawing.Point(pbCtrl.Left + e.Location.X - startPoint.X, pbCtrl.Top + e.Location.Y - startPoint.Y);
        }

        public void OnMouseUp(object sender, MouseEventArgs e)
        {
            startPoint = System.Drawing.Point.Empty;
        }

        public void OnMouseWheel(object sender, MouseEventArgs e)
        {
            // 마우스 위치 확인
            // 스크롤 이벤트는 마우스가 picture control 위에서만 동작함
            // 마우스 위치가 이미지 위라면 마우스 위치 기준으로 처리함
            // 그렇지 않을 경우 이미지 센터 기준으로 축소/확대 처리함

            if (e.Delta > 0)
            {
                zoomIn(e.Location);
            }
            else
            {
                zoomOut(e.Location);
            }
        }

        private void zoomIn(Point pt)
        {
            ImgView mainForm = this.owner as ImgView;

            int picPosX = pbCtrl.Left + pt.X;
            int picPosY = pbCtrl.Top + pt.Y;

            // Check if the pictureBox dimensions are in range (5 is the minimum and maximum zoom level)
            if ((pbCtrl.Width < (5 * mainForm.Width)) && (pbCtrl.Height < (5 * mainForm.Height)))
            {
                pbCtrl.SuspendLayout();

                // Change the size of the picturebox, multiply it by the ZOOMFACTOR
                pbCtrl.Width = (int)(pbCtrl.Width * zoomRatio);
                pbCtrl.Height = (int)(pbCtrl.Height * zoomRatio);

                // Formula to move the picturebox, to zoom in the point selected by the mouse cursor
                pbCtrl.Top = picPosY - (int)(zoomRatio * (float)pt.Y);
                pbCtrl.Left = picPosX - (int)(zoomRatio * (float)pt.X);

                pbCtrl.ResumeLayout();
            }
        }

        public void ZoomIn()
        {
            ImgView mainForm = this.owner as ImgView;

            // Change the size of the picturebox, multiply it by the ZOOMFACTOR
            int centerX = pbCtrl.Left + pbCtrl.Width / 2;
            int centerY = pbCtrl.Top + pbCtrl.Height / 2;

            pbCtrl.SuspendLayout();
            if ((pbCtrl.Width < (5 * mainForm.Width)) && (pbCtrl.Height < (5 * mainForm.Height)))
            {
                pbCtrl.Width = (int)(pbCtrl.Width * zoomRatio);
                pbCtrl.Height = (int)(pbCtrl.Height * zoomRatio);

                pbCtrl.Left = centerX - pbCtrl.Width / 2;
                pbCtrl.Top = centerY - pbCtrl.Height / 2;
            }
            pbCtrl.ResumeLayout();
        }

        private void zoomOut(Point pt)
        {
            ImgView mainForm = this.owner as ImgView;
            
            Image image = pbCtrl.Image;
            if (image == null)  return;

            int picPosX = pbCtrl.Left + pt.X;
            int picPosY = pbCtrl.Top + pt.Y;

            int width = mainForm.Width;
            int height = mainForm.Height - (toolStripH + toolStatusH);

            pbCtrl.SuspendLayout();
            // Check if the pictureBox dimensions are in range (15 is the minimum and maximum zoom level)
            if ((pbCtrl.Width > width) || (pbCtrl.Height > height))
            {
                // Change the size of the picturebox, divide it by the ZOOMFACTOR
                pbCtrl.Width = (int)((float)pbCtrl.Width / 1.125f);
                pbCtrl.Height = (int)((float)pbCtrl.Height / 1.125f);

                if(pbCtrl.Height<= height && pbCtrl.Width <= width)
                {
                    ZoomFixed(false);
                }
                else
                {
                    // Formula to move the picturebox, to zoom in the point selected by the mouse cursor
                    pbCtrl.Top = picPosY - (int)((float)pt.Y / 1.125f);
                    pbCtrl.Left = picPosX - (int)((float)pt.X / 1.125f);
                }
            }
            else
            {
                ZoomFixed(false);
            }
            pbCtrl.ResumeLayout();
        }

        public void ZoomOut()
        {
            ImgView mainForm = this.owner as ImgView;

            int width = mainForm.Width;
            int height = mainForm.Height - (toolStripH + toolStatusH);

            int centerX = pbCtrl.Left + pbCtrl.Width / 2;
            int centerY = pbCtrl.Top + pbCtrl.Height / 2;
            int newWidth, newHeight;

            // Change the size of the picturebox, divide it by the ZOOMFACTOR
            pbCtrl.SuspendLayout();

            newWidth = (int)(pbCtrl.Width / zoomRatio);
            newHeight = (int)(pbCtrl.Height / zoomRatio);

            if (pbCtrl.Height <= height && pbCtrl.Width <= width)
            {
                ZoomFixed(false);
            }
            else
            {
                pbCtrl.Left = centerX - pbCtrl.Width / 2;
                pbCtrl.Top = centerY - pbCtrl.Height / 2;
                pbCtrl.Width = newWidth;
                pbCtrl.Height = newHeight;
            }
            pbCtrl.ResumeLayout();
        }

        public void ZoomExpand()
        {
            Image image = pbCtrl.Image;
            if (image == null)  return;

            ImgView mainForm = this.owner as ImgView;

            pbCtrl.SuspendLayout();
            int width = mainForm.Width;
            int height = mainForm.Height - (toolStripH + toolStatusH);
            pbCtrl.Width = image.Width;
            pbCtrl.Height = image.Height;
            pbCtrl.Location = new System.Drawing.Point((width - pbCtrl.Width) / 2, (height - pbCtrl.Height) / 2 + toolStripH);
            pbCtrl.ResumeLayout();
        }

        public void ZoomFixed(bool isHold=true)
        {
            ImgView mainForm = this.owner as ImgView;

            Image image = pbCtrl.Image;
            if (image == null) return;

            int width = mainForm.Width;
            int height = mainForm.Height - (toolStripH + toolStatusH);

            float xyRatio = (float)width / (float)height;
            
            if(isHold==true) pbCtrl.SuspendLayout();

            if (image.Width > image.Height)
            {
                pbCtrl.Width = width;
                pbCtrl.Height = (int)((float)image.Height * ((float)width / (float)image.Width));

                if (pbCtrl.Height > height)
                {
                    float raito = (float)height / (float)pbCtrl.Height;
                    pbCtrl.Height = height;
                    pbCtrl.Width = (int)((float)pbCtrl.Width * raito);
                }
            }
            else
            {
                pbCtrl.Height = height;
                pbCtrl.Width = (int)((float)image.Width * ((float)height / (float)image.Height));

                if (pbCtrl.Width > width)
                {
                    float ratio = (float)width / (float)pbCtrl.Width;
                    pbCtrl.Width = width;
                    pbCtrl.Height = (int)((float)pbCtrl.Height * ratio);
                }
            }

            pbCtrl.Location = new System.Drawing.Point((width - pbCtrl.Width) / 2, (height - pbCtrl.Height) / 2 + toolStripH);

            if (isHold == true) pbCtrl.ResumeLayout();
        }

        public bool ViewPtToRealPt(Point pt, out Point realPt, out Color value)
        {
            realPt = new Point();
            value = new Color();
            
            Image image = pbCtrl.Image;
            if (image == null) return false;

            int boxWidth = pbCtrl.Width;
            int boxHeight = pbCtrl.Height;

            int imageWidth = pbCtrl.Image.Width;
            int imageHeight = pbCtrl.Image.Height;

            float ratioX = (float)imageWidth / (float)boxWidth;
            float ratioY = (float)imageHeight / (float)boxHeight;

            realPt.X = (int)((float)pt.X * ratioX);
            realPt.Y = (int)((float)pt.Y * ratioY);

            if (realPt.X < 0 || realPt.X >= imageWidth || realPt.Y < 0 || realPt.Y >= imageHeight)
                return false;

            Color colorVal = ((Bitmap)image).GetPixel(realPt.X, realPt.Y);
            value = colorVal;

            return true;
        }
    }
}

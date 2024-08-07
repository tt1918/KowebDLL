using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Windows.Forms;

namespace ImageView
{
    public class SelRect : IDisposable
    {
        private bool _disposed = false;

        private PictureBox owner;
        private Rectangle rectDraw;
        
        public int Num
        {
            get { return num; }
        }
        private int num;

        public Rectangle Area
        {
            get { return rectReal; }
            set
            {
                rectReal = value;
                CalcRealToDraw();
                owner?.Invoke(new MethodInvoker(delegate ()
                {
                    owner.Invalidate();
                }));
            }
        }
        private Rectangle rectReal;

        public bool IsHitted
        {
            get { return isHitted; }
        }
        private bool isHitted = false;

        private Color rectColor = Color.Blue;
        private Color rectSelColor = Color.Red;
        

        public SelRect(int num, Rectangle r, PictureBox p)
        {
            this.num = num;
            rectReal = r;
            
            this.owner = p;
            owner.MouseDown += new MouseEventHandler(OnMouseDown);
            owner.MouseUp += new MouseEventHandler(OnMouseUp);
            owner.Paint += new PaintEventHandler(OnPaint);
            owner.Resize += new EventHandler(OnResize);

            Redraw();
        }

        ~SelRect()
        {
            if (_disposed)
                return;
            Dispose(false);
        }
        public void Dispose()
        {
            this.Dispose(true);
            GC.SuppressFinalize(this);
        }
        protected virtual void Dispose(bool disposing)
        {
            if (_disposed)
                return;
            if (disposing)
            {
                owner.MouseDown -= OnMouseDown;
                owner.MouseUp -= OnMouseUp;
                owner.Paint -= OnPaint;
            }
            ///disposing un-managed resource     
            _disposed = true;
        }

        public void OnResize(object e, EventArgs arg)
        {
            CalcRealToDraw();
            Redraw();
        }

        private void Redraw()
        {
            owner?.Invoke(new MethodInvoker(delegate ()
            {
                owner.Invalidate();
            }));
        }

        public void Draw(Graphics g)
        {
            if (owner == null) return;
            if (owner.Image == null) return;

            CalcRealToDraw();

            if (IsHitted == true)
                g.DrawRectangle(new Pen(rectSelColor), rectDraw);
            else
                g.DrawRectangle(new Pen(rectColor), rectDraw);
        }

        private void CalcRealToDraw()
        {
            int boxWidth = owner.Width;
            int boxHeight = owner.Height;

            int imageWidth = owner.Image.Width;
            int imageHeight = owner.Image.Height;

            float ratioX = (float)boxWidth / (float)imageWidth;
            float ratioY = (float)boxHeight / (float)imageHeight;

            rectDraw.X = (int)((float)rectReal.X * ratioX);
            rectDraw.Width = (int)((float)rectReal.Width * ratioX);
            rectDraw.Y = (int)((float)rectReal.Y * ratioY);
            rectDraw.Height = (int)((float)rectReal.Height * ratioY);
        }
        
        private void OnPaint(object sender, PaintEventArgs e)
        {
            try
            {
                Draw(e.Graphics);
            }
            catch (Exception exp)
            {
                System.Console.WriteLine(exp.Message);
            }
        }

        private void OnMouseDown(object sender, MouseEventArgs e)
        {
            if (IsHitted == false)
                return;
        }

        private void OnMouseUp(object sender, MouseEventArgs e)
        {
            if (owner == null)
                return;
            if (owner.Image == null)
                return;
            
            HitTest(e.Location);
            Redraw();
        }
        
        private bool HitTest(Point pt)
        {
            Rectangle rectOutRange = new Rectangle(rectDraw.Location, rectDraw.Size);
            Rectangle rectInRange = new Rectangle(rectDraw.Location, rectDraw.Size);

            rectOutRange.Inflate(3, 3);
            rectInRange.Inflate(-3, -3);

            if (rectOutRange.Contains(pt) == true && rectInRange.Contains(pt) == false)
                isHitted = true;
            else
                isHitted = false;

            return isHitted;
        }
    }
}

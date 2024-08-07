using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Windows.Forms;

namespace ImageView
{
    public class RectTracker : IDisposable
    {
        private bool _disposed = false;

        private PictureBox owner;
        private Rectangle rectDraw;

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
        
        public string Name
        {
            get { return name; }
            set { name = value; }
        }
        private string name;

        public bool ShowName
        {
            get { return showName; }
            set { showName = value; }
        }
        private bool showName = false;

        private Font txtFont = new Font("Malgun Gothic", 14, FontStyle.Bold, GraphicsUnit.Pixel);

        public bool IsHitted
        {
            get { return isHitted; }
        }
        private bool isHitted = false;

        public bool SkipHand
        {
            get { return skipHand; }
            set { skipHand = value; }
        }
        private bool skipHand = false;

        private bool isClicked = false;
        private bool isCanMove = false;
        private bool isMoved = false;
        private int oldX;
        private int oldY;
        private int sizeNodeRect = 5;
        private PosSizableRect nodeSelected = PosSizableRect.None;
        private bool allowDeformingDuringMovement = false;

        private Color rectColor = Color.Blue;
        private Color rectSelColor = Color.Red;

        public bool IsShow
        {
            get { return isShow; }
        }
        private bool isShow = false;

        private enum PosSizableRect
        {
            UpMiddle,
            LeftMiddle,
            LeftBottom,
            LeftUp,
            RightUp,
            RightMiddle,
            RightBottom,
            BottomMiddle,
            None
        };

        public RectTracker(Rectangle r, string name)
        {
            rectReal = r;
            isClicked = false;
            isShow = false;
            this.Name = name;
        }

        ~RectTracker()
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
                txtFont.Dispose();
                owner.MouseDown -= OnMouseDown;
                owner.MouseUp -= OnMouseUp;
                owner.MouseMove -= OnMouseMove;
                owner.MouseWheel -= OnMouseWheel;
                owner.Paint -= OnPaint;
            }
            ///disposing un-managed resource     
            _disposed = true;
        }
        public void Draw(Graphics g)
        {
            if (isShow == false) return;
            if (owner == null) return;
            if (owner.Image == null) return;

            if (IsHitted == true)
                g.DrawRectangle(new Pen(rectSelColor), rectDraw);
            else
                g.DrawRectangle(new Pen(rectColor), rectDraw);

            foreach (PosSizableRect pos in Enum.GetValues(typeof(PosSizableRect)))
            {
                g.DrawRectangle(new Pen(Color.Cyan), GetRect(pos));
            }

            if(showName==true)
            {
                Point textPt = new Point(rectDraw.X, rectDraw.Y - 20);
                g.DrawString(this.name, txtFont, Brushes.Blue, textPt);
            }
        }

        public void SetPictureBox(PictureBox p)
        {
            this.owner = p;
            owner.MouseDown += new MouseEventHandler(OnMouseDown);
            owner.MouseUp += new MouseEventHandler(OnMouseUp);
            owner.MouseMove += new MouseEventHandler(OnMouseMove);
            owner.MouseWheel += new MouseEventHandler(OnMouseWheel);
            owner.Paint += new PaintEventHandler(OnPaint);
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

        private void Redraw()
        {
            owner.Invoke(new MethodInvoker(delegate ()
            {
                owner.Invalidate();
            }));
        }
       

        public void Show()
        {
            isShow = true;
            CalcRealToDraw();
            Redraw();
        }

        public void Hide()
        {
            isShow = false;
            Redraw();
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

            isClicked = true;

            nodeSelected = PosSizableRect.None;
            nodeSelected = GetNodeSelectable(e.Location);

            if (rectDraw.Contains(new Point(e.X, e.Y)))
            {
                isCanMove = true;
                isMoved = false;
            }
            oldX = e.X;
            oldY = e.Y;
        }

        private void OnMouseUp(object sender, MouseEventArgs e)
        {
            bool isOldMoved = isMoved;
            isClicked = false;
            isCanMove = false;
            isMoved = false;

            if (owner == null)
                return;
            if (owner.Image == null)
                return;

            int boxWidth = owner.Width;
            int boxHeight = owner.Height;

            int imageWidth = owner.Image.Width;
            int imageHeight = owner.Image.Height;

            float ratioX = (float)imageWidth / (float)boxWidth;
            float ratioY = (float)imageHeight / (float)boxHeight;

            if (rectDraw.Width != 0 && rectDraw.Height != 0)
            {
                rectReal.X = (int)((float)rectDraw.X * ratioX);
                rectReal.Width = (int)((float)rectDraw.Width * ratioX);
                rectReal.Y = (int)((float)rectDraw.Y * ratioY);
                rectReal.Height = (int)((float)rectDraw.Height * ratioY);
            }

            if(skipHand==false)
                HitTest(e.Location, isOldMoved);

            Redraw();
        }

        private void OnMouseWheel(object sender, MouseEventArgs e)
        {
            if (owner == null)
                return;
            if (owner.Image == null)
                return;

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

            Redraw();
        }

        private void OnMouseMove(object sender, MouseEventArgs e)
        {
            if (skipHand == true)   return;
            if(isHitted==true)
                ChangeCursor(e.Location);

            if (isClicked == false) return;
            
            Rectangle backupRect = rectDraw;

            switch (nodeSelected)
            {
                case PosSizableRect.LeftUp:
                    rectDraw.X += e.X - oldX;
                    rectDraw.Width -= e.X - oldX;
                    rectDraw.Y += e.Y - oldY;
                    rectDraw.Height -= e.Y - oldY;
                    break;
                case PosSizableRect.LeftMiddle:
                    rectDraw.X += e.X - oldX;
                    rectDraw.Width -= e.X - oldX;
                    break;
                case PosSizableRect.LeftBottom:
                    rectDraw.Width -= e.X - oldX;
                    rectDraw.X += e.X - oldX;
                    rectDraw.Height += e.Y - oldY;
                    break;
                case PosSizableRect.BottomMiddle:
                    rectDraw.Height += e.Y - oldY;
                    break;
                case PosSizableRect.RightUp:
                    rectDraw.Width += e.X - oldX;
                    rectDraw.Y += e.Y - oldY;
                    rectDraw.Height -= e.Y - oldY;
                    break;
                case PosSizableRect.RightBottom:
                    rectDraw.Width += e.X - oldX;
                    rectDraw.Height += e.Y - oldY;
                    break;
                case PosSizableRect.RightMiddle:
                    rectDraw.Width += e.X - oldX;
                    break;

                case PosSizableRect.UpMiddle:
                    rectDraw.Y += e.Y - oldY;
                    rectDraw.Height -= e.Y - oldY;
                    break;

                default:
                    if (isCanMove)
                    {
                        rectDraw.X = rectDraw.X + e.X - oldX;
                        rectDraw.Y = rectDraw.Y + e.Y - oldY;
                        isMoved = true;
                    }
                    break;
            }
            oldX = e.X;
            oldY = e.Y;

            if (rectDraw.Width < 5 || rectDraw.Height < 5)
            {
                rectDraw = backupRect;
            }

            TestIfRectInsideArea();

            Redraw();
        }

        private void TestIfRectInsideArea()
        {
            // Test if rectangle still inside the area.
            if (rectDraw.X < 0) rectDraw.X = 0;
            if (rectDraw.Y < 0) rectDraw.Y = 0;
            if (rectDraw.Width <= 0) rectDraw.Width = 1;
            if (rectDraw.Height <= 0) rectDraw.Height = 1;

            if (rectDraw.X + rectDraw.Width > owner.Width)
            {
                rectDraw.Width = owner.Width - rectDraw.X - 1; // -1 to be still show 
                if (allowDeformingDuringMovement == false)
                {
                    isClicked = false;
                }
            }
            if (rectDraw.Y + rectDraw.Height > owner.Height)
            {
                rectDraw.Height = owner.Height - rectDraw.Y - 1;// -1 to be still show 
                if (allowDeformingDuringMovement == false)
                {
                    isClicked = false;
                }
            }
        }

        private Rectangle CreateRectSizableNode(int x, int y)
        {
            return new Rectangle(x - sizeNodeRect / 2, y - sizeNodeRect / 2, sizeNodeRect, sizeNodeRect);
        }

        private Rectangle GetRect(PosSizableRect p)
        {
            switch (p)
            {
                case PosSizableRect.LeftUp:
                    return CreateRectSizableNode(rectDraw.X, rectDraw.Y);

                case PosSizableRect.LeftMiddle:
                    return CreateRectSizableNode(rectDraw.X, rectDraw.Y + +rectDraw.Height / 2);

                case PosSizableRect.LeftBottom:
                    return CreateRectSizableNode(rectDraw.X, rectDraw.Y + rectDraw.Height);

                case PosSizableRect.BottomMiddle:
                    return CreateRectSizableNode(rectDraw.X + rectDraw.Width / 2, rectDraw.Y + rectDraw.Height);

                case PosSizableRect.RightUp:
                    return CreateRectSizableNode(rectDraw.X + rectDraw.Width, rectDraw.Y);

                case PosSizableRect.RightBottom:
                    return CreateRectSizableNode(rectDraw.X + rectDraw.Width, rectDraw.Y + rectDraw.Height);

                case PosSizableRect.RightMiddle:
                    return CreateRectSizableNode(rectDraw.X + rectDraw.Width, rectDraw.Y + rectDraw.Height / 2);

                case PosSizableRect.UpMiddle:
                    return CreateRectSizableNode(rectDraw.X + rectDraw.Width / 2, rectDraw.Y);
                default:
                    return new Rectangle();
            }
        }

        private PosSizableRect GetNodeSelectable(Point p)
        {
            foreach (PosSizableRect r in Enum.GetValues(typeof(PosSizableRect)))
            {
                if (GetRect(r).Contains(p))
                {
                    return r;
                }
            }
            return PosSizableRect.None;
        }

        private void ChangeCursor(Point p)
        {
            owner.Cursor = GetCursor(GetNodeSelectable(p));
        }

        /// <summary>
        /// Get cursor for the handle
        /// </summary>
        /// <param name="p"></param>
        /// <returns></returns>
        private Cursor GetCursor(PosSizableRect p)
        {
            switch (p)
            {
                case PosSizableRect.LeftUp:
                    return Cursors.SizeNWSE;

                case PosSizableRect.LeftMiddle:
                    return Cursors.SizeWE;

                case PosSizableRect.LeftBottom:
                    return Cursors.SizeNESW;

                case PosSizableRect.BottomMiddle:
                    return Cursors.SizeNS;

                case PosSizableRect.RightUp:
                    return Cursors.SizeNESW;

                case PosSizableRect.RightBottom:
                    return Cursors.SizeNWSE;

                case PosSizableRect.RightMiddle:
                    return Cursors.SizeWE;

                case PosSizableRect.UpMiddle:
                    return Cursors.SizeNS;
                default:
                    return Cursors.Default;
            }
        }

        private bool HitTest(Point pt, bool isExcepted)
        {
            Rectangle rectOutRange = new Rectangle(rectDraw.Location, rectDraw.Size);
            Rectangle rectInRange = new Rectangle(rectDraw.Location, rectDraw.Size);

            rectOutRange.Inflate(3, 3);
            rectInRange.Inflate(-3, -3);

            if (rectOutRange.Contains(pt))
            {
                if (!rectInRange.Contains(pt) || isExcepted == true)
                    isHitted = true;
            }
            else
            {
                isHitted = false;
            }

            return isHitted;
        }
    }
}

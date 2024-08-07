using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace ImageView
{
    public partial class ImgView : UserControl
    {
        public Bitmap Image
        {
            get { return image; }
            set
            {
                if (value == null)
                    return;

                image?.Dispose();
                image = new Bitmap(value);
                setDataToViewer();
            }
        }
        private Bitmap image = null;
        private ViewDefault viewer = null;

        public RectTracker RectTracker
        {
            get { return tracker; }
            set { tracker = value; }
        }
        private RectTracker tracker;

        private List<SelRect> defectRect = new List<SelRect>();
        
        public ImgView()
        {
            InitializeComponent();
            viewer = new ViewDefault(this, this.pbViewer, this.toolStrip1.Height, this.statusStrip1.Height);
            this.pbViewer.MouseWheel += Viewer_MouseWheel;
            this.pbViewer.MouseMove += new System.Windows.Forms.MouseEventHandler(this.Viewer_MouseMove);

            RectTracker = new RectTracker(new Rectangle(10, 10, 100, 100), "Test");
            RectTracker.SetPictureBox(this.pbViewer);
        }

        ~ImgView()
        {
            tracker?.Dispose();
            image?.Dispose();
            defectRect.Clear();
        }

        private void setDataToViewer()
        {
            try
            {
                pbViewer.SuspendLayout();
                pbViewer.Image?.Dispose();
                pbViewer.Top = 0;
                pbViewer.Left = 0;
                pbViewer.Image = image;

                viewer.ZoomFixed();
            }
            catch (Exception exc)
            {
                image?.Dispose();
                image = null;
                MessageBox.Show(exc.Message);
            }
        }

        #region 마우스 스크롤 처리
        private void Viewer_MouseDown(object sender, MouseEventArgs e)
        {
            if (Image == null)
                return;
            if (e.Button == MouseButtons.Left)
                viewer.OnMouseDown(sender, e);
        }

        private void Viewer_MouseMove(object sender, MouseEventArgs e)
        {
            if (Image == null)
                return;
            if (e.Button == MouseButtons.Left && tsHandMove.CheckState == CheckState.Checked)
            {
                viewer.OnMouseMove(sender, e);
                this.Cursor = Cursors.Hand;
            }
            else
            {
                OnUpdatePixelData(e.Location);
            }
        }

        private void Viewer_MouseUp(object sender, MouseEventArgs e)
        {
            viewer.OnMouseUp(sender, e);
            this.Cursor = Cursors.Default;
        }

        private void Viewer_MouseWheel(object sender, MouseEventArgs e)
        {
            if (null == image)
                return;

            viewer?.OnMouseWheel(sender, e);
        }
        #endregion

        private void tsHandMove_Click(object sender, EventArgs e)
        {
            if (tsHandMove.CheckState == CheckState.Unchecked)
            {
                this.pbViewer.MouseDown += new System.Windows.Forms.MouseEventHandler(this.Viewer_MouseDown);
                this.pbViewer.MouseUp += new System.Windows.Forms.MouseEventHandler(this.Viewer_MouseUp);
                tsHandMove.CheckState = CheckState.Checked;
                this.RectTracker.SkipHand = true;
            }
            else
            {
                this.pbViewer.MouseDown -= this.Viewer_MouseDown;
                this.pbViewer.MouseUp -= this.Viewer_MouseUp;
                tsHandMove.CheckState = CheckState.Unchecked;
                this.RectTracker.SkipHand = false;
            }
        }

        private void tbZoomIn_Click(object sender, EventArgs e)
        {
            viewer.ZoomIn();
        }

        private void tbZoomOut_Click(object sender, EventArgs e)
        {
            viewer.ZoomOut();
        }

        private void tbExpand_Click(object sender, EventArgs e)
        {
            viewer.ZoomExpand();
        }

        private void tbFixImage_Click(object sender, EventArgs e)
        {
            viewer.ZoomFixed();
        }

        private void ImgView_SizeChanged(object sender, EventArgs e)
        {
            viewer?.OnResize(sender, e);
        }

        public void OnUpdatePixelData(Point pt)
        {
            if(viewer.ViewPtToRealPt(pt, out Point realPt, out Color value)==true)
            {
                this.Invoke(new MethodInvoker(delegate ()
                {
                    lblPosValue.Text = $"(X:{realPt.X}, Y:{realPt.Y})";
                    lblPixelValue.Text = $"R:{value.R}, G:{value.G}, B:{value.B}";
                }));
            }    
        }

        public void AddDefectPt(Rectangle rect)
        {
            SelRect data = new SelRect(0, rect, this.pbViewer);
            defectRect.Add(data);
        }

        public void ClearDefectPt()
        {
            while(defectRect.Count>0)
            {
                defectRect[0].Dispose();
                defectRect.RemoveAt(0);
            }

            pbViewer?.Invoke(new MethodInvoker(delegate ()
            {
                pbViewer.Invalidate();
            }));
        }
    }
}

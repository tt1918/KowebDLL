namespace ImageView
{
    partial class ImgView
    {
        /// <summary> 
        /// 필수 디자이너 변수입니다.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary> 
        /// 사용 중인 모든 리소스를 정리합니다.
        /// </summary>
        /// <param name="disposing">관리되는 리소스를 삭제해야 하면 true이고, 그렇지 않으면 false입니다.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region 구성 요소 디자이너에서 생성한 코드

        /// <summary> 
        /// 디자이너 지원에 필요한 메서드입니다. 
        /// 이 메서드의 내용을 코드 편집기로 수정하지 마세요.
        /// </summary>
        private void InitializeComponent()
        {
            this.statusStrip1 = new System.Windows.Forms.StatusStrip();
            this.lblPos = new System.Windows.Forms.ToolStripStatusLabel();
            this.lblPosValue = new System.Windows.Forms.ToolStripStatusLabel();
            this.lblPixel = new System.Windows.Forms.ToolStripStatusLabel();
            this.lblPixelValue = new System.Windows.Forms.ToolStripStatusLabel();
            this.toolStrip1 = new System.Windows.Forms.ToolStrip();
            this.tsHandMove = new System.Windows.Forms.ToolStripButton();
            this.tbZoomIn = new System.Windows.Forms.ToolStripButton();
            this.tbZoomOut = new System.Windows.Forms.ToolStripButton();
            this.tbExpand = new System.Windows.Forms.ToolStripButton();
            this.tbFixImage = new System.Windows.Forms.ToolStripButton();
            this.pbViewer = new System.Windows.Forms.PictureBox();
            this.statusStrip1.SuspendLayout();
            this.toolStrip1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pbViewer)).BeginInit();
            this.SuspendLayout();
            // 
            // statusStrip1
            // 
            this.statusStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.lblPos,
            this.lblPosValue,
            this.lblPixel,
            this.lblPixelValue});
            this.statusStrip1.Location = new System.Drawing.Point(0, 506);
            this.statusStrip1.Name = "statusStrip1";
            this.statusStrip1.Size = new System.Drawing.Size(668, 22);
            this.statusStrip1.TabIndex = 1;
            this.statusStrip1.Text = "statusStrip1";
            // 
            // lblPos
            // 
            this.lblPos.BackColor = System.Drawing.Color.Transparent;
            this.lblPos.Name = "lblPos";
            this.lblPos.Size = new System.Drawing.Size(29, 17);
            this.lblPos.Text = "Pos:";
            // 
            // lblPosValue
            // 
            this.lblPosValue.BackColor = System.Drawing.Color.Transparent;
            this.lblPosValue.Name = "lblPosValue";
            this.lblPosValue.Size = new System.Drawing.Size(26, 17);
            this.lblPosValue.Text = "( , )";
            // 
            // lblPixel
            // 
            this.lblPixel.BackColor = System.Drawing.Color.Transparent;
            this.lblPixel.Name = "lblPixel";
            this.lblPixel.Size = new System.Drawing.Size(73, 17);
            this.lblPixel.Text = "Pixel Value :";
            // 
            // lblPixelValue
            // 
            this.lblPixelValue.BackColor = System.Drawing.Color.Transparent;
            this.lblPixelValue.Name = "lblPixelValue";
            this.lblPixelValue.Size = new System.Drawing.Size(20, 17);
            this.lblPixelValue.Text = " - ";
            // 
            // toolStrip1
            // 
            this.toolStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.tsHandMove,
            this.tbZoomIn,
            this.tbZoomOut,
            this.tbExpand,
            this.tbFixImage});
            this.toolStrip1.Location = new System.Drawing.Point(0, 0);
            this.toolStrip1.Name = "toolStrip1";
            this.toolStrip1.Size = new System.Drawing.Size(668, 25);
            this.toolStrip1.TabIndex = 2;
            this.toolStrip1.Text = "toolStrip1";
            // 
            // tsHandMove
            // 
            this.tsHandMove.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.tsHandMove.Image = global::test.Properties.Resources.hand_move;
            this.tsHandMove.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.tsHandMove.Name = "tsHandMove";
            this.tsHandMove.Size = new System.Drawing.Size(23, 22);
            this.tsHandMove.Text = "toolStripButton1";
            this.tsHandMove.ToolTipText = "Hand Move";
            this.tsHandMove.Click += new System.EventHandler(this.tsHandMove_Click);
            // 
            // tbZoomIn
            // 
            this.tbZoomIn.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.tbZoomIn.Image = global::test.Properties.Resources.zoom_in_16;
            this.tbZoomIn.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.tbZoomIn.Name = "tbZoomIn";
            this.tbZoomIn.Size = new System.Drawing.Size(23, 22);
            this.tbZoomIn.Text = "zoom in";
            this.tbZoomIn.ToolTipText = "Zoom In";
            this.tbZoomIn.Click += new System.EventHandler(this.tbZoomIn_Click);
            // 
            // tbZoomOut
            // 
            this.tbZoomOut.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.tbZoomOut.Image = global::test.Properties.Resources.zoom_out_16;
            this.tbZoomOut.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.tbZoomOut.Name = "tbZoomOut";
            this.tbZoomOut.Size = new System.Drawing.Size(23, 22);
            this.tbZoomOut.Text = "zoom out";
            this.tbZoomOut.ToolTipText = "Zoom Out";
            this.tbZoomOut.Click += new System.EventHandler(this.tbZoomOut_Click);
            // 
            // tbExpand
            // 
            this.tbExpand.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.tbExpand.Image = global::test.Properties.Resources.expand_16;
            this.tbExpand.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.tbExpand.Name = "tbExpand";
            this.tbExpand.Size = new System.Drawing.Size(23, 22);
            this.tbExpand.Text = "expand";
            this.tbExpand.ToolTipText = "1:1 View";
            this.tbExpand.Click += new System.EventHandler(this.tbExpand_Click);
            // 
            // tbFixImage
            // 
            this.tbFixImage.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.tbFixImage.Image = global::test.Properties.Resources.collapse_16;
            this.tbFixImage.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.tbFixImage.Name = "tbFixImage";
            this.tbFixImage.Size = new System.Drawing.Size(23, 22);
            this.tbFixImage.Text = "FixedView";
            this.tbFixImage.ToolTipText = "Fix View";
            this.tbFixImage.Click += new System.EventHandler(this.tbFixImage_Click);
            // 
            // pbViewer
            // 
            this.pbViewer.BackColor = System.Drawing.Color.Black;
            this.pbViewer.Location = new System.Drawing.Point(3, 81);
            this.pbViewer.Name = "pbViewer";
            this.pbViewer.Size = new System.Drawing.Size(189, 138);
            this.pbViewer.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom;
            this.pbViewer.TabIndex = 0;
            this.pbViewer.TabStop = false;
            // 
            // ImgView
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.Black;
            this.Controls.Add(this.toolStrip1);
            this.Controls.Add(this.statusStrip1);
            this.Controls.Add(this.pbViewer);
            this.Name = "ImgView";
            this.Size = new System.Drawing.Size(668, 528);
            this.SizeChanged += new System.EventHandler(this.ImgView_SizeChanged);
            this.statusStrip1.ResumeLayout(false);
            this.statusStrip1.PerformLayout();
            this.toolStrip1.ResumeLayout(false);
            this.toolStrip1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pbViewer)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.PictureBox pbViewer;
        private System.Windows.Forms.StatusStrip statusStrip1;
        private System.Windows.Forms.ToolStripStatusLabel lblPos;
        private System.Windows.Forms.ToolStripStatusLabel lblPosValue;
        private System.Windows.Forms.ToolStripStatusLabel lblPixel;
        private System.Windows.Forms.ToolStripStatusLabel lblPixelValue;
        private System.Windows.Forms.ToolStrip toolStrip1;
        private System.Windows.Forms.ToolStripButton tsHandMove;
        private System.Windows.Forms.ToolStripButton tbZoomIn;
        private System.Windows.Forms.ToolStripButton tbZoomOut;
        private System.Windows.Forms.ToolStripButton tbExpand;
        private System.Windows.Forms.ToolStripButton tbFixImage;
    }
}

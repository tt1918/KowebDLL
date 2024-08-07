namespace test
{
    partial class Form1
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

        #region Windows Form 디자이너에서 생성한 코드

        /// <summary>
        /// 디자이너 지원에 필요한 메서드입니다. 
        /// 이 메서드의 내용을 코드 편집기로 수정하지 마세요.
        /// </summary>
        private void InitializeComponent()
        {
            this.btnLoadImage = new System.Windows.Forms.Button();
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.splitContainer2 = new System.Windows.Forms.SplitContainer();
            this.MainView = new ImageView.ImgView();
            this.btnClearDefect = new System.Windows.Forms.Button();
            this.btnAddDefect = new System.Windows.Forms.Button();
            this.btnTest = new System.Windows.Forms.Button();
            this.btnAddTracker = new System.Windows.Forms.Button();
            this.btnMatchImage = new System.Windows.Forms.Button();
            this.btnLoadModelImage = new System.Windows.Forms.Button();
            this.ViewPattern = new ImageView.ImgView();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).BeginInit();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer2)).BeginInit();
            this.splitContainer2.Panel1.SuspendLayout();
            this.splitContainer2.Panel2.SuspendLayout();
            this.splitContainer2.SuspendLayout();
            this.SuspendLayout();
            // 
            // btnLoadImage
            // 
            this.btnLoadImage.Font = new System.Drawing.Font("Malgun Gothic", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.btnLoadImage.Location = new System.Drawing.Point(7, 10);
            this.btnLoadImage.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.btnLoadImage.Name = "btnLoadImage";
            this.btnLoadImage.Size = new System.Drawing.Size(104, 34);
            this.btnLoadImage.TabIndex = 1;
            this.btnLoadImage.Text = "LOAD";
            this.btnLoadImage.UseVisualStyleBackColor = true;
            this.btnLoadImage.Click += new System.EventHandler(this.btnLoadImage_Click);
            // 
            // splitContainer1
            // 
            this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer1.Location = new System.Drawing.Point(0, 0);
            this.splitContainer1.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.splitContainer1.Name = "splitContainer1";
            // 
            // splitContainer1.Panel1
            // 
            this.splitContainer1.Panel1.Controls.Add(this.splitContainer2);
            // 
            // splitContainer1.Panel2
            // 
            this.splitContainer1.Panel2.Controls.Add(this.btnClearDefect);
            this.splitContainer1.Panel2.Controls.Add(this.btnAddDefect);
            this.splitContainer1.Panel2.Controls.Add(this.btnTest);
            this.splitContainer1.Panel2.Controls.Add(this.btnAddTracker);
            this.splitContainer1.Panel2.Controls.Add(this.btnMatchImage);
            this.splitContainer1.Panel2.Controls.Add(this.btnLoadModelImage);
            this.splitContainer1.Panel2.Controls.Add(this.btnLoadImage);
            this.splitContainer1.Size = new System.Drawing.Size(1368, 856);
            this.splitContainer1.SplitterDistance = 1238;
            this.splitContainer1.SplitterWidth = 5;
            this.splitContainer1.TabIndex = 2;
            // 
            // splitContainer2
            // 
            this.splitContainer2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer2.Location = new System.Drawing.Point(0, 0);
            this.splitContainer2.Name = "splitContainer2";
            // 
            // splitContainer2.Panel1
            // 
            this.splitContainer2.Panel1.Controls.Add(this.MainView);
            // 
            // splitContainer2.Panel2
            // 
            this.splitContainer2.Panel2.Controls.Add(this.ViewPattern);
            this.splitContainer2.Size = new System.Drawing.Size(1238, 856);
            this.splitContainer2.SplitterDistance = 619;
            this.splitContainer2.TabIndex = 1;
            // 
            // MainView
            // 
            this.MainView.BackColor = System.Drawing.Color.Black;
            this.MainView.Dock = System.Windows.Forms.DockStyle.Fill;
            this.MainView.Image = null;
            this.MainView.Location = new System.Drawing.Point(0, 0);
            this.MainView.Margin = new System.Windows.Forms.Padding(3, 5, 3, 5);
            this.MainView.Name = "MainView";
            this.MainView.Size = new System.Drawing.Size(619, 856);
            this.MainView.TabIndex = 0;
            // 
            // btnClearDefect
            // 
            this.btnClearDefect.Font = new System.Drawing.Font("Malgun Gothic", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.btnClearDefect.Location = new System.Drawing.Point(7, 224);
            this.btnClearDefect.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.btnClearDefect.Name = "btnClearDefect";
            this.btnClearDefect.Size = new System.Drawing.Size(104, 50);
            this.btnClearDefect.TabIndex = 1;
            this.btnClearDefect.Text = "Clear Defect";
            this.btnClearDefect.UseVisualStyleBackColor = true;
            this.btnClearDefect.Click += new System.EventHandler(this.btnClearDefect_Click);
            // 
            // btnAddDefect
            // 
            this.btnAddDefect.Font = new System.Drawing.Font("Malgun Gothic", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.btnAddDefect.Location = new System.Drawing.Point(7, 166);
            this.btnAddDefect.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.btnAddDefect.Name = "btnAddDefect";
            this.btnAddDefect.Size = new System.Drawing.Size(104, 50);
            this.btnAddDefect.TabIndex = 1;
            this.btnAddDefect.Text = "Add Defect";
            this.btnAddDefect.UseVisualStyleBackColor = true;
            this.btnAddDefect.Click += new System.EventHandler(this.btnAddDefect_Click);
            // 
            // btnTest
            // 
            this.btnTest.Font = new System.Drawing.Font("Malgun Gothic", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.btnTest.Location = new System.Drawing.Point(7, 109);
            this.btnTest.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.btnTest.Name = "btnTest";
            this.btnTest.Size = new System.Drawing.Size(104, 50);
            this.btnTest.TabIndex = 1;
            this.btnTest.Text = "CLEAR TRACHKER";
            this.btnTest.UseVisualStyleBackColor = true;
            this.btnTest.Click += new System.EventHandler(this.btnTest_Click);
            // 
            // btnAddTracker
            // 
            this.btnAddTracker.Font = new System.Drawing.Font("Malgun Gothic", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.btnAddTracker.Location = new System.Drawing.Point(7, 51);
            this.btnAddTracker.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.btnAddTracker.Name = "btnAddTracker";
            this.btnAddTracker.Size = new System.Drawing.Size(104, 50);
            this.btnAddTracker.TabIndex = 1;
            this.btnAddTracker.Text = "ADD TRACKER";
            this.btnAddTracker.UseVisualStyleBackColor = true;
            this.btnAddTracker.Click += new System.EventHandler(this.btnAddTracker_Click);
            // 
            // btnMatchImage
            // 
            this.btnMatchImage.Font = new System.Drawing.Font("Malgun Gothic", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.btnMatchImage.Location = new System.Drawing.Point(10, 452);
            this.btnMatchImage.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.btnMatchImage.Name = "btnMatchImage";
            this.btnMatchImage.Size = new System.Drawing.Size(104, 39);
            this.btnMatchImage.TabIndex = 1;
            this.btnMatchImage.Text = "Run";
            this.btnMatchImage.UseVisualStyleBackColor = true;
            this.btnMatchImage.Click += new System.EventHandler(this.btnMatchImage_Click);
            // 
            // btnLoadModelImage
            // 
            this.btnLoadModelImage.Font = new System.Drawing.Font("Malgun Gothic", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.btnLoadModelImage.Location = new System.Drawing.Point(7, 386);
            this.btnLoadModelImage.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.btnLoadModelImage.Name = "btnLoadModelImage";
            this.btnLoadModelImage.Size = new System.Drawing.Size(104, 58);
            this.btnLoadModelImage.TabIndex = 1;
            this.btnLoadModelImage.Text = "Load Model Image";
            this.btnLoadModelImage.UseVisualStyleBackColor = true;
            this.btnLoadModelImage.Click += new System.EventHandler(this.btnLoadModelImage_Click);
            // 
            // ViewPattern
            // 
            this.ViewPattern.BackColor = System.Drawing.Color.Black;
            this.ViewPattern.Dock = System.Windows.Forms.DockStyle.Fill;
            this.ViewPattern.Image = null;
            this.ViewPattern.Location = new System.Drawing.Point(0, 0);
            this.ViewPattern.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.ViewPattern.Name = "ViewPattern";
            this.ViewPattern.Size = new System.Drawing.Size(615, 856);
            this.ViewPattern.TabIndex = 2;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1368, 856);
            this.Controls.Add(this.splitContainer1);
            this.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.Name = "Form1";
            this.Text = "Form1";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.Form1_FormClosed);
            this.splitContainer1.Panel1.ResumeLayout(false);
            this.splitContainer1.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).EndInit();
            this.splitContainer1.ResumeLayout(false);
            this.splitContainer2.Panel1.ResumeLayout(false);
            this.splitContainer2.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer2)).EndInit();
            this.splitContainer2.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion
        private System.Windows.Forms.Button btnLoadImage;
        private System.Windows.Forms.SplitContainer splitContainer1;
        private ImageView.ImgView MainView;
        private System.Windows.Forms.Button btnAddTracker;
        private System.Windows.Forms.Button btnTest;
        private System.Windows.Forms.Button btnAddDefect;
        private System.Windows.Forms.Button btnClearDefect;
        private System.Windows.Forms.Button btnLoadModelImage;
        private System.Windows.Forms.Button btnMatchImage;
        private System.Windows.Forms.SplitContainer splitContainer2;
        private ImageView.ImgView ViewPattern;
    }
}


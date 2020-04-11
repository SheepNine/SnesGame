namespace Scribe
{
    partial class MainForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.glyphPanel = new System.Windows.Forms.Control();
            this.palettePanel = new System.Windows.Forms.Control();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.SuspendLayout();
            // 
            // glyphPanel
            // 
            this.glyphPanel.Location = new System.Drawing.Point(12, 12);
            this.glyphPanel.Name = "glyphPanel";
            this.glyphPanel.Size = new System.Drawing.Size(256, 256);
            this.glyphPanel.TabIndex = 0;
            this.glyphPanel.Paint += new System.Windows.Forms.PaintEventHandler(this.glyphPanel_Paint);
            this.glyphPanel.MouseDown += new System.Windows.Forms.MouseEventHandler(this.glyphPanel_MouseDown);
            this.glyphPanel.MouseMove += new System.Windows.Forms.MouseEventHandler(this.glyphPanel_MouseMove);
            this.glyphPanel.MouseUp += new System.Windows.Forms.MouseEventHandler(this.glyphPanel_MouseUp);
            // 
            // palettePanel
            // 
            this.palettePanel.Location = new System.Drawing.Point(274, 12);
            this.palettePanel.Name = "palettePanel";
            this.palettePanel.Size = new System.Drawing.Size(16, 256);
            this.palettePanel.TabIndex = 1;
            this.palettePanel.Paint += new System.Windows.Forms.PaintEventHandler(this.palettePanel_Paint);
            this.palettePanel.MouseUp += new System.Windows.Forms.MouseEventHandler(this.palettePanel_MouseUp);
            // 
            // textBox1
            // 
            this.textBox1.Font = new System.Drawing.Font("Consolas", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.textBox1.Location = new System.Drawing.Point(12, 274);
            this.textBox1.Multiline = true;
            this.textBox1.Name = "textBox1";
            this.textBox1.ReadOnly = true;
            this.textBox1.Size = new System.Drawing.Size(278, 75);
            this.textBox1.TabIndex = 2;
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(302, 361);
            this.Controls.Add(this.textBox1);
            this.Controls.Add(this.palettePanel);
            this.Controls.Add(this.glyphPanel);
            this.Name = "MainForm";
            this.Text = "Scribe";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Control glyphPanel;
        private System.Windows.Forms.Control palettePanel;
        private System.Windows.Forms.TextBox textBox1;
    }
}


namespace GraphPaper
{
    partial class NewModelArgumentsDialog
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
            this.undirectedButton = new System.Windows.Forms.Button();
            this.directedButton = new System.Windows.Forms.Button();
            this.cancelButton = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // undirectedButton
            // 
            this.undirectedButton.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.undirectedButton.Location = new System.Drawing.Point(14, 14);
            this.undirectedButton.Margin = new System.Windows.Forms.Padding(4, 3, 4, 3);
            this.undirectedButton.Name = "undirectedButton";
            this.undirectedButton.Size = new System.Drawing.Size(190, 27);
            this.undirectedButton.TabIndex = 0;
            this.undirectedButton.Text = "Undirected Graph";
            this.undirectedButton.UseVisualStyleBackColor = true;
            this.undirectedButton.Click += new System.EventHandler(this.undirectedButton_Click);
            // 
            // directedButton
            // 
            this.directedButton.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.directedButton.Location = new System.Drawing.Point(14, 47);
            this.directedButton.Margin = new System.Windows.Forms.Padding(4, 3, 4, 3);
            this.directedButton.Name = "directedButton";
            this.directedButton.Size = new System.Drawing.Size(190, 27);
            this.directedButton.TabIndex = 1;
            this.directedButton.Text = "Directed Graph";
            this.directedButton.UseVisualStyleBackColor = true;
            this.directedButton.Click += new System.EventHandler(this.directedButton_Click);
            // 
            // cancelButton
            // 
            this.cancelButton.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.cancelButton.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.cancelButton.Location = new System.Drawing.Point(14, 81);
            this.cancelButton.Margin = new System.Windows.Forms.Padding(4, 3, 4, 3);
            this.cancelButton.Name = "cancelButton";
            this.cancelButton.Size = new System.Drawing.Size(190, 27);
            this.cancelButton.TabIndex = 2;
            this.cancelButton.Text = "Cancel";
            this.cancelButton.UseVisualStyleBackColor = true;
            this.cancelButton.Click += new System.EventHandler(this.cancelButton_Click);
            // 
            // NewModelArgumentsDialog
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.cancelButton;
            this.ClientSize = new System.Drawing.Size(218, 121);
            this.ControlBox = false;
            this.Controls.Add(this.cancelButton);
            this.Controls.Add(this.directedButton);
            this.Controls.Add(this.undirectedButton);
            this.Margin = new System.Windows.Forms.Padding(4, 3, 4, 3);
            this.Name = "NewModelArgumentsDialog";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Create New...";
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button undirectedButton;
        private System.Windows.Forms.Button directedButton;
        private System.Windows.Forms.Button cancelButton;
    }
}
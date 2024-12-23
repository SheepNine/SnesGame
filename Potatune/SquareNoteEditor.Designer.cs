
namespace Potatune
{
    partial class SquareNoteEditor
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
            this.lengthComboBox = new System.Windows.Forms.ComboBox();
            this.lengthLabel1 = new System.Windows.Forms.Label();
            this.lengthTextBox = new System.Windows.Forms.TextBox();
            this.lengthLabel2 = new System.Windows.Forms.Label();
            this.volumeDirComboBox = new System.Windows.Forms.ComboBox();
            this.volumeLabel1 = new System.Windows.Forms.Label();
            this.volumeMinTextBox = new System.Windows.Forms.TextBox();
            this.volumeLabel2 = new System.Windows.Forms.Label();
            this.volumeMaxTextBox = new System.Windows.Forms.TextBox();
            this.volumeLabel3 = new System.Windows.Forms.Label();
            this.volumeSpeedTextBox = new System.Windows.Forms.TextBox();
            this.volumeLabel4 = new System.Windows.Forms.Label();
            this.volumeEdgeComboBox = new System.Windows.Forms.ComboBox();
            this.periodEdgeComboBox = new System.Windows.Forms.ComboBox();
            this.pitchLabel4 = new System.Windows.Forms.Label();
            this.periodSpeedTextBox = new System.Windows.Forms.TextBox();
            this.pitchLabel3 = new System.Windows.Forms.Label();
            this.periodMaxTextBox = new System.Windows.Forms.TextBox();
            this.pitchLabel2 = new System.Windows.Forms.Label();
            this.periodMinTextBox = new System.Windows.Forms.TextBox();
            this.pitchLabel1 = new System.Windows.Forms.Label();
            this.periodDirComboBox = new System.Windows.Forms.ComboBox();
            this.okButton = new System.Windows.Forms.Button();
            this.cancelButton = new System.Windows.Forms.Button();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.playButton = new System.Windows.Forms.Button();
            this.squareWavePanel = new System.Windows.Forms.Panel();
            this.noisePanel = new System.Windows.Forms.Panel();
            this.label7 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.noiseRepeat = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.noiseLength = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.noiseRegisterValue = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.noiseTapBits = new System.Windows.Forms.ComboBox();
            this.label8 = new System.Windows.Forms.Label();
            this.noteType = new System.Windows.Forms.ComboBox();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.squareWavePanel.SuspendLayout();
            this.noisePanel.SuspendLayout();
            this.SuspendLayout();
            // 
            // lengthComboBox
            // 
            this.lengthComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.lengthComboBox.FormattingEnabled = true;
            this.lengthComboBox.Items.AddRange(new object[] {
            "Unlimited",
            "Limited"});
            this.lengthComboBox.Location = new System.Drawing.Point(8, 23);
            this.lengthComboBox.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.lengthComboBox.Name = "lengthComboBox";
            this.lengthComboBox.Size = new System.Drawing.Size(76, 24);
            this.lengthComboBox.TabIndex = 1;
            this.lengthComboBox.SelectedIndexChanged += new System.EventHandler(this.lengthComboBox_SelectedIndexChanged);
            // 
            // lengthLabel1
            // 
            this.lengthLabel1.AutoSize = true;
            this.lengthLabel1.Location = new System.Drawing.Point(93, 27);
            this.lengthLabel1.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.lengthLabel1.Name = "lengthLabel1";
            this.lengthLabel1.Size = new System.Drawing.Size(20, 17);
            this.lengthLabel1.TabIndex = 2;
            this.lengthLabel1.Text = "to";
            // 
            // lengthTextBox
            // 
            this.lengthTextBox.Location = new System.Drawing.Point(123, 23);
            this.lengthTextBox.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.lengthTextBox.Name = "lengthTextBox";
            this.lengthTextBox.Size = new System.Drawing.Size(64, 22);
            this.lengthTextBox.TabIndex = 3;
            this.lengthTextBox.Text = "6553500";
            // 
            // lengthLabel2
            // 
            this.lengthLabel2.AutoSize = true;
            this.lengthLabel2.Location = new System.Drawing.Point(196, 27);
            this.lengthLabel2.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.lengthLabel2.Name = "lengthLabel2";
            this.lengthLabel2.Size = new System.Drawing.Size(60, 17);
            this.lengthLabel2.TabIndex = 4;
            this.lengthLabel2.Text = "samples";
            // 
            // volumeDirComboBox
            // 
            this.volumeDirComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.volumeDirComboBox.FormattingEnabled = true;
            this.volumeDirComboBox.Items.AddRange(new object[] {
            "Steady",
            "Rising",
            "Falling"});
            this.volumeDirComboBox.Location = new System.Drawing.Point(8, 23);
            this.volumeDirComboBox.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.volumeDirComboBox.Name = "volumeDirComboBox";
            this.volumeDirComboBox.Size = new System.Drawing.Size(76, 24);
            this.volumeDirComboBox.TabIndex = 5;
            this.volumeDirComboBox.SelectedIndexChanged += new System.EventHandler(this.volumeDirComboBox_SelectedIndexChanged);
            // 
            // volumeLabel1
            // 
            this.volumeLabel1.AutoSize = true;
            this.volumeLabel1.Location = new System.Drawing.Point(93, 27);
            this.volumeLabel1.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.volumeLabel1.Name = "volumeLabel1";
            this.volumeLabel1.Size = new System.Drawing.Size(36, 17);
            this.volumeLabel1.TabIndex = 6;
            this.volumeLabel1.Text = "from";
            // 
            // volumeMinTextBox
            // 
            this.volumeMinTextBox.Location = new System.Drawing.Point(137, 23);
            this.volumeMinTextBox.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.volumeMinTextBox.Name = "volumeMinTextBox";
            this.volumeMinTextBox.Size = new System.Drawing.Size(17, 22);
            this.volumeMinTextBox.TabIndex = 7;
            this.volumeMinTextBox.Text = "8";
            // 
            // volumeLabel2
            // 
            this.volumeLabel2.AutoSize = true;
            this.volumeLabel2.Location = new System.Drawing.Point(164, 27);
            this.volumeLabel2.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.volumeLabel2.Name = "volumeLabel2";
            this.volumeLabel2.Size = new System.Drawing.Size(20, 17);
            this.volumeLabel2.TabIndex = 8;
            this.volumeLabel2.Text = "to";
            // 
            // volumeMaxTextBox
            // 
            this.volumeMaxTextBox.Location = new System.Drawing.Point(193, 23);
            this.volumeMaxTextBox.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.volumeMaxTextBox.Name = "volumeMaxTextBox";
            this.volumeMaxTextBox.Size = new System.Drawing.Size(17, 22);
            this.volumeMaxTextBox.TabIndex = 9;
            this.volumeMaxTextBox.Text = "8";
            // 
            // volumeLabel3
            // 
            this.volumeLabel3.AutoSize = true;
            this.volumeLabel3.Location = new System.Drawing.Point(220, 27);
            this.volumeLabel3.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.volumeLabel3.Name = "volumeLabel3";
            this.volumeLabel3.Size = new System.Drawing.Size(43, 17);
            this.volumeLabel3.TabIndex = 11;
            this.volumeLabel3.Text = "every";
            // 
            // volumeSpeedTextBox
            // 
            this.volumeSpeedTextBox.Location = new System.Drawing.Point(272, 23);
            this.volumeSpeedTextBox.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.volumeSpeedTextBox.Name = "volumeSpeedTextBox";
            this.volumeSpeedTextBox.Size = new System.Drawing.Size(33, 22);
            this.volumeSpeedTextBox.TabIndex = 12;
            this.volumeSpeedTextBox.Text = "888";
            // 
            // volumeLabel4
            // 
            this.volumeLabel4.AutoSize = true;
            this.volumeLabel4.Location = new System.Drawing.Point(315, 27);
            this.volumeLabel4.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.volumeLabel4.Name = "volumeLabel4";
            this.volumeLabel4.Size = new System.Drawing.Size(89, 17);
            this.volumeLabel4.TabIndex = 13;
            this.volumeLabel4.Text = "wave(s) then";
            // 
            // volumeEdgeComboBox
            // 
            this.volumeEdgeComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.volumeEdgeComboBox.FormattingEnabled = true;
            this.volumeEdgeComboBox.Items.AddRange(new object[] {
            "End note",
            "Wrap value",
            "Reverse direction",
            "Cancel shift"});
            this.volumeEdgeComboBox.Location = new System.Drawing.Point(413, 23);
            this.volumeEdgeComboBox.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.volumeEdgeComboBox.Name = "volumeEdgeComboBox";
            this.volumeEdgeComboBox.Size = new System.Drawing.Size(141, 24);
            this.volumeEdgeComboBox.TabIndex = 14;
            // 
            // periodEdgeComboBox
            // 
            this.periodEdgeComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.periodEdgeComboBox.FormattingEnabled = true;
            this.periodEdgeComboBox.Items.AddRange(new object[] {
            "End note",
            "Wrap value",
            "Reverse direction",
            "Cancel shift"});
            this.periodEdgeComboBox.Location = new System.Drawing.Point(465, 23);
            this.periodEdgeComboBox.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.periodEdgeComboBox.Name = "periodEdgeComboBox";
            this.periodEdgeComboBox.Size = new System.Drawing.Size(141, 24);
            this.periodEdgeComboBox.TabIndex = 24;
            // 
            // pitchLabel4
            // 
            this.pitchLabel4.AutoSize = true;
            this.pitchLabel4.Location = new System.Drawing.Point(367, 27);
            this.pitchLabel4.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.pitchLabel4.Name = "pitchLabel4";
            this.pitchLabel4.Size = new System.Drawing.Size(89, 17);
            this.pitchLabel4.TabIndex = 23;
            this.pitchLabel4.Text = "wave(s) then";
            // 
            // periodSpeedTextBox
            // 
            this.periodSpeedTextBox.Location = new System.Drawing.Point(323, 23);
            this.periodSpeedTextBox.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.periodSpeedTextBox.Name = "periodSpeedTextBox";
            this.periodSpeedTextBox.Size = new System.Drawing.Size(35, 22);
            this.periodSpeedTextBox.TabIndex = 22;
            this.periodSpeedTextBox.Text = "288";
            // 
            // pitchLabel3
            // 
            this.pitchLabel3.AutoSize = true;
            this.pitchLabel3.Location = new System.Drawing.Point(271, 27);
            this.pitchLabel3.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.pitchLabel3.Name = "pitchLabel3";
            this.pitchLabel3.Size = new System.Drawing.Size(43, 17);
            this.pitchLabel3.TabIndex = 21;
            this.pitchLabel3.Text = "every";
            // 
            // periodMaxTextBox
            // 
            this.periodMaxTextBox.Location = new System.Drawing.Point(219, 23);
            this.periodMaxTextBox.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.periodMaxTextBox.Name = "periodMaxTextBox";
            this.periodMaxTextBox.Size = new System.Drawing.Size(43, 22);
            this.periodMaxTextBox.TabIndex = 19;
            this.periodMaxTextBox.Text = "1888";
            // 
            // pitchLabel2
            // 
            this.pitchLabel2.AutoSize = true;
            this.pitchLabel2.Location = new System.Drawing.Point(189, 27);
            this.pitchLabel2.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.pitchLabel2.Name = "pitchLabel2";
            this.pitchLabel2.Size = new System.Drawing.Size(20, 17);
            this.pitchLabel2.TabIndex = 18;
            this.pitchLabel2.Text = "to";
            // 
            // periodMinTextBox
            // 
            this.periodMinTextBox.Location = new System.Drawing.Point(137, 23);
            this.periodMinTextBox.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.periodMinTextBox.Name = "periodMinTextBox";
            this.periodMinTextBox.Size = new System.Drawing.Size(43, 22);
            this.periodMinTextBox.TabIndex = 17;
            this.periodMinTextBox.Text = "1888";
            // 
            // pitchLabel1
            // 
            this.pitchLabel1.AutoSize = true;
            this.pitchLabel1.Location = new System.Drawing.Point(93, 27);
            this.pitchLabel1.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.pitchLabel1.Name = "pitchLabel1";
            this.pitchLabel1.Size = new System.Drawing.Size(36, 17);
            this.pitchLabel1.TabIndex = 16;
            this.pitchLabel1.Text = "from";
            // 
            // periodDirComboBox
            // 
            this.periodDirComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.periodDirComboBox.FormattingEnabled = true;
            this.periodDirComboBox.Items.AddRange(new object[] {
            "Steady",
            "Rising",
            "Falling"});
            this.periodDirComboBox.Location = new System.Drawing.Point(8, 23);
            this.periodDirComboBox.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.periodDirComboBox.Name = "periodDirComboBox";
            this.periodDirComboBox.Size = new System.Drawing.Size(76, 24);
            this.periodDirComboBox.TabIndex = 15;
            this.periodDirComboBox.SelectedIndexChanged += new System.EventHandler(this.pitchDirComboBox_SelectedIndexChanged);
            // 
            // okButton
            // 
            this.okButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.okButton.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.okButton.Location = new System.Drawing.Point(423, 238);
            this.okButton.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.okButton.Name = "okButton";
            this.okButton.Size = new System.Drawing.Size(100, 28);
            this.okButton.TabIndex = 25;
            this.okButton.Text = "OK";
            this.okButton.UseVisualStyleBackColor = true;
            // 
            // cancelButton
            // 
            this.cancelButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.cancelButton.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.cancelButton.Location = new System.Drawing.Point(531, 238);
            this.cancelButton.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.cancelButton.Name = "cancelButton";
            this.cancelButton.Size = new System.Drawing.Size(100, 28);
            this.cancelButton.TabIndex = 26;
            this.cancelButton.Text = "Cancel";
            this.cancelButton.UseVisualStyleBackColor = true;
            // 
            // groupBox1
            // 
            this.groupBox1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox1.Controls.Add(this.lengthComboBox);
            this.groupBox1.Controls.Add(this.lengthLabel1);
            this.groupBox1.Controls.Add(this.lengthTextBox);
            this.groupBox1.Controls.Add(this.lengthLabel2);
            this.groupBox1.Location = new System.Drawing.Point(0, 0);
            this.groupBox1.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Padding = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.groupBox1.Size = new System.Drawing.Size(621, 57);
            this.groupBox1.TabIndex = 27;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Length";
            // 
            // groupBox2
            // 
            this.groupBox2.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox2.Controls.Add(this.volumeDirComboBox);
            this.groupBox2.Controls.Add(this.volumeLabel1);
            this.groupBox2.Controls.Add(this.volumeMinTextBox);
            this.groupBox2.Controls.Add(this.volumeLabel2);
            this.groupBox2.Controls.Add(this.volumeMaxTextBox);
            this.groupBox2.Controls.Add(this.volumeLabel3);
            this.groupBox2.Controls.Add(this.volumeSpeedTextBox);
            this.groupBox2.Controls.Add(this.volumeLabel4);
            this.groupBox2.Controls.Add(this.volumeEdgeComboBox);
            this.groupBox2.Location = new System.Drawing.Point(0, 128);
            this.groupBox2.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Padding = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.groupBox2.Size = new System.Drawing.Size(621, 57);
            this.groupBox2.TabIndex = 28;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Volume";
            // 
            // groupBox3
            // 
            this.groupBox3.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox3.Controls.Add(this.periodDirComboBox);
            this.groupBox3.Controls.Add(this.pitchLabel1);
            this.groupBox3.Controls.Add(this.periodMinTextBox);
            this.groupBox3.Controls.Add(this.pitchLabel2);
            this.groupBox3.Controls.Add(this.periodMaxTextBox);
            this.groupBox3.Controls.Add(this.pitchLabel3);
            this.groupBox3.Controls.Add(this.periodSpeedTextBox);
            this.groupBox3.Controls.Add(this.pitchLabel4);
            this.groupBox3.Controls.Add(this.periodEdgeComboBox);
            this.groupBox3.Location = new System.Drawing.Point(0, 64);
            this.groupBox3.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Padding = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.groupBox3.Size = new System.Drawing.Size(621, 57);
            this.groupBox3.TabIndex = 29;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "Period";
            // 
            // playButton
            // 
            this.playButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.playButton.Location = new System.Drawing.Point(16, 238);
            this.playButton.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.playButton.Name = "playButton";
            this.playButton.Size = new System.Drawing.Size(100, 28);
            this.playButton.TabIndex = 30;
            this.playButton.Text = "Play";
            this.playButton.UseVisualStyleBackColor = true;
            this.playButton.Click += new System.EventHandler(this.playButton_Click);
            // 
            // squareWavePanel
            // 
            this.squareWavePanel.Controls.Add(this.groupBox1);
            this.squareWavePanel.Controls.Add(this.groupBox2);
            this.squareWavePanel.Controls.Add(this.groupBox3);
            this.squareWavePanel.Location = new System.Drawing.Point(13, 45);
            this.squareWavePanel.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.squareWavePanel.Name = "squareWavePanel";
            this.squareWavePanel.Size = new System.Drawing.Size(621, 185);
            this.squareWavePanel.TabIndex = 31;
            // 
            // noisePanel
            // 
            this.noisePanel.Controls.Add(this.label7);
            this.noisePanel.Controls.Add(this.label6);
            this.noisePanel.Controls.Add(this.label5);
            this.noisePanel.Controls.Add(this.label4);
            this.noisePanel.Controls.Add(this.noiseRepeat);
            this.noisePanel.Controls.Add(this.label3);
            this.noisePanel.Controls.Add(this.noiseLength);
            this.noisePanel.Controls.Add(this.label2);
            this.noisePanel.Controls.Add(this.noiseRegisterValue);
            this.noisePanel.Controls.Add(this.label1);
            this.noisePanel.Controls.Add(this.noiseTapBits);
            this.noisePanel.Location = new System.Drawing.Point(13, 45);
            this.noisePanel.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.noisePanel.Name = "noisePanel";
            this.noisePanel.Size = new System.Drawing.Size(621, 129);
            this.noisePanel.TabIndex = 32;
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(207, 41);
            this.label7.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(96, 17);
            this.label7.TabIndex = 10;
            this.label7.Text = "(hexadecimal)";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(207, 105);
            this.label6.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(123, 17);
            this.label6.TabIndex = 9;
            this.label6.Text = "samples per value";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(207, 73);
            this.label5.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(60, 17);
            this.label5.TabIndex = 8;
            this.label5.Text = "samples";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(4, 105);
            this.label4.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(93, 17);
            this.label4.TabIndex = 7;
            this.label4.Text = "Value repeat:";
            // 
            // noiseRepeat
            // 
            this.noiseRepeat.Location = new System.Drawing.Point(147, 101);
            this.noiseRepeat.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.noiseRepeat.Name = "noiseRepeat";
            this.noiseRepeat.Size = new System.Drawing.Size(51, 22);
            this.noiseRepeat.TabIndex = 6;
            this.noiseRepeat.Text = "255";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(4, 73);
            this.label3.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(56, 17);
            this.label3.TabIndex = 5;
            this.label3.Text = "Length:";
            // 
            // noiseLength
            // 
            this.noiseLength.Location = new System.Drawing.Point(147, 69);
            this.noiseLength.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.noiseLength.Name = "noiseLength";
            this.noiseLength.Size = new System.Drawing.Size(51, 22);
            this.noiseLength.TabIndex = 4;
            this.noiseLength.Text = "58888";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(4, 41);
            this.label2.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(135, 17);
            this.label2.TabIndex = 3;
            this.label2.Text = "Register start value:";
            // 
            // noiseRegisterValue
            // 
            this.noiseRegisterValue.Location = new System.Drawing.Point(147, 37);
            this.noiseRegisterValue.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.noiseRegisterValue.Name = "noiseRegisterValue";
            this.noiseRegisterValue.Size = new System.Drawing.Size(51, 22);
            this.noiseRegisterValue.TabIndex = 2;
            this.noiseRegisterValue.Text = "FFFF";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(4, 11);
            this.label1.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(102, 17);
            this.label1.TabIndex = 1;
            this.label1.Text = "Cycle duration:";
            // 
            // noiseTapBits
            // 
            this.noiseTapBits.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.noiseTapBits.FormattingEnabled = true;
            this.noiseTapBits.Items.AddRange(new object[] {
            "Long",
            "Short"});
            this.noiseTapBits.Location = new System.Drawing.Point(147, 7);
            this.noiseTapBits.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.noiseTapBits.Name = "noiseTapBits";
            this.noiseTapBits.Size = new System.Drawing.Size(72, 24);
            this.noiseTapBits.TabIndex = 0;
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(13, 16);
            this.label8.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(73, 17);
            this.label8.TabIndex = 33;
            this.label8.Text = "Note type:";
            // 
            // noteType
            // 
            this.noteType.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.noteType.FormattingEnabled = true;
            this.noteType.Items.AddRange(new object[] {
            "Square",
            "Noise"});
            this.noteType.Location = new System.Drawing.Point(94, 13);
            this.noteType.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.noteType.Name = "noteType";
            this.noteType.Size = new System.Drawing.Size(160, 24);
            this.noteType.TabIndex = 34;
            this.noteType.SelectedIndexChanged += new System.EventHandler(this.noteType_SelectedIndexChanged);
            // 
            // SquareNoteEditor
            // 
            this.AcceptButton = this.okButton;
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.cancelButton;
            this.ClientSize = new System.Drawing.Size(647, 281);
            this.Controls.Add(this.noteType);
            this.Controls.Add(this.label8);
            this.Controls.Add(this.noisePanel);
            this.Controls.Add(this.squareWavePanel);
            this.Controls.Add(this.playButton);
            this.Controls.Add(this.cancelButton);
            this.Controls.Add(this.okButton);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "SquareNoteEditor";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Edit Square Note";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.SquareNoteEditor_FormClosing);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            this.squareWavePanel.ResumeLayout(false);
            this.noisePanel.ResumeLayout(false);
            this.noisePanel.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion
        private System.Windows.Forms.ComboBox lengthComboBox;
        private System.Windows.Forms.Label lengthLabel1;
        private System.Windows.Forms.TextBox lengthTextBox;
        private System.Windows.Forms.Label lengthLabel2;
        private System.Windows.Forms.ComboBox volumeDirComboBox;
        private System.Windows.Forms.Label volumeLabel1;
        private System.Windows.Forms.TextBox volumeMinTextBox;
        private System.Windows.Forms.Label volumeLabel2;
        private System.Windows.Forms.TextBox volumeMaxTextBox;
        private System.Windows.Forms.Label volumeLabel3;
        private System.Windows.Forms.TextBox volumeSpeedTextBox;
        private System.Windows.Forms.Label volumeLabel4;
        private System.Windows.Forms.ComboBox volumeEdgeComboBox;
        private System.Windows.Forms.ComboBox periodEdgeComboBox;
        private System.Windows.Forms.Label pitchLabel4;
        private System.Windows.Forms.TextBox periodSpeedTextBox;
        private System.Windows.Forms.Label pitchLabel3;
        private System.Windows.Forms.TextBox periodMaxTextBox;
        private System.Windows.Forms.Label pitchLabel2;
        private System.Windows.Forms.TextBox periodMinTextBox;
        private System.Windows.Forms.Label pitchLabel1;
        private System.Windows.Forms.ComboBox periodDirComboBox;
        private System.Windows.Forms.Button okButton;
        private System.Windows.Forms.Button cancelButton;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.Button playButton;
        private System.Windows.Forms.Panel squareWavePanel;
        private System.Windows.Forms.Panel noisePanel;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox noiseRepeat;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox noiseLength;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox noiseRegisterValue;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.ComboBox noiseTapBits;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.ComboBox noteType;
    }
}
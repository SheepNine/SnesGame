namespace Patchwork
{
    partial class View
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">
        /// true if managed resources should be disposed; otherwise, false.</param>
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(View));
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.newToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.loadToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.openRecentToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.saveToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.saveAsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
            this.exportToPNGToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator3 = new System.Windows.Forms.ToolStripSeparator();
            this.closeToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.editToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.undoToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.redoToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.copyRenderedArrangementToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.expandRightToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.expandBottomToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.expandLeftToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.expandTopToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.contractRightToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.contractBottomToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.contractLeftToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.contractTopToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.viewToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.zoomInToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.zoomOutToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toggleGridDisplayToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.addTilesetToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.removeTilesetToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.reloadTilesetToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.stampToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.statusStrip1 = new System.Windows.Forms.StatusStrip();
            this.cursorPositionLabel = new System.Windows.Forms.ToolStripStatusLabel();
            this.editorPanelBorder = new System.Windows.Forms.Panel();
            this.editorPanel = new UnaryHeap.GUI.WysiwygPanel();
            this.tilesetPanelBorder = new System.Windows.Forms.Panel();
            this.tilesetPanel = new UnaryHeap.GUI.WysiwygPanel();
            this.editorGestures = new UnaryHeap.GUI.GestureInterpreter();
            this.tilesetGestures = new UnaryHeap.GUI.GestureInterpreter();
            this.menuStrip1.SuspendLayout();
            this.statusStrip1.SuspendLayout();
            this.editorPanelBorder.SuspendLayout();
            this.tilesetPanelBorder.SuspendLayout();
            this.SuspendLayout();
            // 
            // menuStrip1
            // 
            this.menuStrip1.ImageScalingSize = new System.Drawing.Size(20, 20);
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.editToolStripMenuItem,
            this.viewToolStripMenuItem,
            this.stampToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Padding = new System.Windows.Forms.Padding(5, 2, 0, 2);
            this.menuStrip1.Size = new System.Drawing.Size(1201, 28);
            this.menuStrip1.TabIndex = 2;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.newToolStripMenuItem,
            this.loadToolStripMenuItem,
            this.openRecentToolStripMenuItem,
            this.toolStripSeparator1,
            this.saveToolStripMenuItem,
            this.saveAsToolStripMenuItem,
            this.toolStripSeparator2,
            this.exportToPNGToolStripMenuItem,
            this.toolStripSeparator3,
            this.closeToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(46, 24);
            this.fileToolStripMenuItem.Text = "File";
            this.fileToolStripMenuItem.DropDownOpening += new System.EventHandler(this.fileToolStripMenuItem_DropDownOpening);
            // 
            // newToolStripMenuItem
            // 
            this.newToolStripMenuItem.Name = "newToolStripMenuItem";
            this.newToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.N)));
            this.newToolStripMenuItem.Size = new System.Drawing.Size(233, 26);
            this.newToolStripMenuItem.Text = "New";
            this.newToolStripMenuItem.Click += new System.EventHandler(this.newToolStripMenuItem_Click);
            // 
            // loadToolStripMenuItem
            // 
            this.loadToolStripMenuItem.Name = "loadToolStripMenuItem";
            this.loadToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.O)));
            this.loadToolStripMenuItem.Size = new System.Drawing.Size(233, 26);
            this.loadToolStripMenuItem.Text = "Open...";
            this.loadToolStripMenuItem.Click += new System.EventHandler(this.loadToolStripMenuItem_Click);
            // 
            // openRecentToolStripMenuItem
            // 
            this.openRecentToolStripMenuItem.Name = "openRecentToolStripMenuItem";
            this.openRecentToolStripMenuItem.Size = new System.Drawing.Size(233, 26);
            this.openRecentToolStripMenuItem.Text = "Open Recent";
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(230, 6);
            // 
            // saveToolStripMenuItem
            // 
            this.saveToolStripMenuItem.Name = "saveToolStripMenuItem";
            this.saveToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.S)));
            this.saveToolStripMenuItem.Size = new System.Drawing.Size(233, 26);
            this.saveToolStripMenuItem.Text = "Save";
            this.saveToolStripMenuItem.Click += new System.EventHandler(this.saveToolStripMenuItem_Click);
            // 
            // saveAsToolStripMenuItem
            // 
            this.saveAsToolStripMenuItem.Name = "saveAsToolStripMenuItem";
            this.saveAsToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)(((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.Alt) 
            | System.Windows.Forms.Keys.A)));
            this.saveAsToolStripMenuItem.Size = new System.Drawing.Size(233, 26);
            this.saveAsToolStripMenuItem.Text = "Save As...";
            this.saveAsToolStripMenuItem.Click += new System.EventHandler(this.saveAsToolStripMenuItem_Click);
            // 
            // toolStripSeparator2
            // 
            this.toolStripSeparator2.Name = "toolStripSeparator2";
            this.toolStripSeparator2.Size = new System.Drawing.Size(230, 6);
            // 
            // exportToPNGToolStripMenuItem
            // 
            this.exportToPNGToolStripMenuItem.Name = "exportToPNGToolStripMenuItem";
            this.exportToPNGToolStripMenuItem.Size = new System.Drawing.Size(233, 26);
            this.exportToPNGToolStripMenuItem.Text = "Export to PNG...";
            this.exportToPNGToolStripMenuItem.Click += new System.EventHandler(this.exportToPNGToolStripMenuItem_Click);
            // 
            // toolStripSeparator3
            // 
            this.toolStripSeparator3.Name = "toolStripSeparator3";
            this.toolStripSeparator3.Size = new System.Drawing.Size(230, 6);
            // 
            // closeToolStripMenuItem
            // 
            this.closeToolStripMenuItem.Name = "closeToolStripMenuItem";
            this.closeToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Alt | System.Windows.Forms.Keys.F4)));
            this.closeToolStripMenuItem.Size = new System.Drawing.Size(233, 26);
            this.closeToolStripMenuItem.Text = "Close";
            this.closeToolStripMenuItem.Click += new System.EventHandler(this.closeToolStripMenuItem_Click);
            // 
            // editToolStripMenuItem
            // 
            this.editToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.undoToolStripMenuItem,
            this.redoToolStripMenuItem,
            this.copyRenderedArrangementToolStripMenuItem,
            this.expandRightToolStripMenuItem,
            this.expandBottomToolStripMenuItem,
            this.expandLeftToolStripMenuItem,
            this.expandTopToolStripMenuItem,
            this.contractRightToolStripMenuItem,
            this.contractBottomToolStripMenuItem,
            this.contractLeftToolStripMenuItem,
            this.contractTopToolStripMenuItem});
            this.editToolStripMenuItem.Name = "editToolStripMenuItem";
            this.editToolStripMenuItem.Size = new System.Drawing.Size(49, 24);
            this.editToolStripMenuItem.Text = "Edit";
            // 
            // undoToolStripMenuItem
            // 
            this.undoToolStripMenuItem.Name = "undoToolStripMenuItem";
            this.undoToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.Z)));
            this.undoToolStripMenuItem.Size = new System.Drawing.Size(323, 26);
            this.undoToolStripMenuItem.Text = "Undo";
            this.undoToolStripMenuItem.Click += new System.EventHandler(this.undoToolStripMenuItem_Click);
            // 
            // redoToolStripMenuItem
            // 
            this.redoToolStripMenuItem.Name = "redoToolStripMenuItem";
            this.redoToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.Y)));
            this.redoToolStripMenuItem.Size = new System.Drawing.Size(323, 26);
            this.redoToolStripMenuItem.Text = "Redo";
            this.redoToolStripMenuItem.Click += new System.EventHandler(this.redoToolStripMenuItem_Click);
            // 
            // copyRenderedArrangementToolStripMenuItem
            // 
            this.copyRenderedArrangementToolStripMenuItem.Name = "copyRenderedArrangementToolStripMenuItem";
            this.copyRenderedArrangementToolStripMenuItem.Size = new System.Drawing.Size(323, 26);
            this.copyRenderedArrangementToolStripMenuItem.Text = "Copy Rendered Arrangement";
            this.copyRenderedArrangementToolStripMenuItem.Click += new System.EventHandler(this.copyRenderedArrangementToolStripMenuItem_Click);
            // 
            // expandRightToolStripMenuItem
            // 
            this.expandRightToolStripMenuItem.Name = "expandRightToolStripMenuItem";
            this.expandRightToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.Right)));
            this.expandRightToolStripMenuItem.Size = new System.Drawing.Size(323, 26);
            this.expandRightToolStripMenuItem.Text = "Expand Right";
            this.expandRightToolStripMenuItem.Click += new System.EventHandler(this.expandRightToolStripMenuItem_Click);
            // 
            // expandBottomToolStripMenuItem
            // 
            this.expandBottomToolStripMenuItem.Name = "expandBottomToolStripMenuItem";
            this.expandBottomToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.Down)));
            this.expandBottomToolStripMenuItem.Size = new System.Drawing.Size(323, 26);
            this.expandBottomToolStripMenuItem.Text = "Expand Bottom";
            this.expandBottomToolStripMenuItem.Click += new System.EventHandler(this.expandBottomToolStripMenuItem_Click);
            // 
            // expandLeftToolStripMenuItem
            // 
            this.expandLeftToolStripMenuItem.Name = "expandLeftToolStripMenuItem";
            this.expandLeftToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.Left)));
            this.expandLeftToolStripMenuItem.Size = new System.Drawing.Size(323, 26);
            this.expandLeftToolStripMenuItem.Text = "Expand Left";
            this.expandLeftToolStripMenuItem.Click += new System.EventHandler(this.expandLeftToolStripMenuItem_Click);
            // 
            // expandTopToolStripMenuItem
            // 
            this.expandTopToolStripMenuItem.Name = "expandTopToolStripMenuItem";
            this.expandTopToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.Up)));
            this.expandTopToolStripMenuItem.Size = new System.Drawing.Size(323, 26);
            this.expandTopToolStripMenuItem.Text = "Expand Top";
            this.expandTopToolStripMenuItem.Click += new System.EventHandler(this.expandTopToolStripMenuItem_Click);
            // 
            // contractRightToolStripMenuItem
            // 
            this.contractRightToolStripMenuItem.Name = "contractRightToolStripMenuItem";
            this.contractRightToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)(((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.Shift) 
            | System.Windows.Forms.Keys.Right)));
            this.contractRightToolStripMenuItem.Size = new System.Drawing.Size(323, 26);
            this.contractRightToolStripMenuItem.Text = "Contract Right";
            this.contractRightToolStripMenuItem.Click += new System.EventHandler(this.contractRightToolStripMenuItem_Click);
            // 
            // contractBottomToolStripMenuItem
            // 
            this.contractBottomToolStripMenuItem.Name = "contractBottomToolStripMenuItem";
            this.contractBottomToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)(((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.Shift) 
            | System.Windows.Forms.Keys.Down)));
            this.contractBottomToolStripMenuItem.Size = new System.Drawing.Size(323, 26);
            this.contractBottomToolStripMenuItem.Text = "Contract Bottom";
            this.contractBottomToolStripMenuItem.Click += new System.EventHandler(this.contractBottomToolStripMenuItem_Click);
            // 
            // contractLeftToolStripMenuItem
            // 
            this.contractLeftToolStripMenuItem.Name = "contractLeftToolStripMenuItem";
            this.contractLeftToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)(((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.Shift) 
            | System.Windows.Forms.Keys.Left)));
            this.contractLeftToolStripMenuItem.Size = new System.Drawing.Size(323, 26);
            this.contractLeftToolStripMenuItem.Text = "Contract Left";
            this.contractLeftToolStripMenuItem.Click += new System.EventHandler(this.contractLeftToolStripMenuItem_Click);
            // 
            // contractTopToolStripMenuItem
            // 
            this.contractTopToolStripMenuItem.Name = "contractTopToolStripMenuItem";
            this.contractTopToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)(((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.Shift) 
            | System.Windows.Forms.Keys.Up)));
            this.contractTopToolStripMenuItem.Size = new System.Drawing.Size(323, 26);
            this.contractTopToolStripMenuItem.Text = "Contract Top";
            this.contractTopToolStripMenuItem.Click += new System.EventHandler(this.contractTopToolStripMenuItem_Click);
            // 
            // viewToolStripMenuItem
            // 
            this.viewToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.zoomInToolStripMenuItem,
            this.zoomOutToolStripMenuItem,
            this.toggleGridDisplayToolStripMenuItem,
            this.addTilesetToolStripMenuItem,
            this.removeTilesetToolStripMenuItem,
            this.reloadTilesetToolStripMenuItem});
            this.viewToolStripMenuItem.Name = "viewToolStripMenuItem";
            this.viewToolStripMenuItem.Size = new System.Drawing.Size(55, 24);
            this.viewToolStripMenuItem.Text = "View";
            this.viewToolStripMenuItem.Click += new System.EventHandler(this.viewToolStripMenuItem_Click);
            // 
            // zoomInToolStripMenuItem
            // 
            this.zoomInToolStripMenuItem.Name = "zoomInToolStripMenuItem";
            this.zoomInToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.Oemplus)));
            this.zoomInToolStripMenuItem.Size = new System.Drawing.Size(275, 26);
            this.zoomInToolStripMenuItem.Text = "Zoom In";
            this.zoomInToolStripMenuItem.Click += new System.EventHandler(this.zoomInToolStripMenuItem_Click);
            // 
            // zoomOutToolStripMenuItem
            // 
            this.zoomOutToolStripMenuItem.Name = "zoomOutToolStripMenuItem";
            this.zoomOutToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.OemMinus)));
            this.zoomOutToolStripMenuItem.Size = new System.Drawing.Size(275, 26);
            this.zoomOutToolStripMenuItem.Text = "Zoom Out";
            this.zoomOutToolStripMenuItem.Click += new System.EventHandler(this.zoomOutToolStripMenuItem_Click);
            // 
            // toggleGridDisplayToolStripMenuItem
            // 
            this.toggleGridDisplayToolStripMenuItem.Name = "toggleGridDisplayToolStripMenuItem";
            this.toggleGridDisplayToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.G)));
            this.toggleGridDisplayToolStripMenuItem.Size = new System.Drawing.Size(275, 26);
            this.toggleGridDisplayToolStripMenuItem.Text = "Toggle Grid Display";
            this.toggleGridDisplayToolStripMenuItem.Click += new System.EventHandler(this.toggleGridDisplayToolStripMenuItem_Click);
            // 
            // addTilesetToolStripMenuItem
            // 
            this.addTilesetToolStripMenuItem.Name = "addTilesetToolStripMenuItem";
            this.addTilesetToolStripMenuItem.Size = new System.Drawing.Size(275, 26);
            this.addTilesetToolStripMenuItem.Text = "Add Tileset...";
            this.addTilesetToolStripMenuItem.Click += new System.EventHandler(this.addTilesetToolStripMenuItem_Click);
            // 
            // removeTilesetToolStripMenuItem
            // 
            this.removeTilesetToolStripMenuItem.Name = "removeTilesetToolStripMenuItem";
            this.removeTilesetToolStripMenuItem.Size = new System.Drawing.Size(275, 26);
            this.removeTilesetToolStripMenuItem.Text = "Remove Tileset";
            this.removeTilesetToolStripMenuItem.Click += new System.EventHandler(this.removeTilesetToolStripMenuItem_Click);
            // 
            // reloadTilesetToolStripMenuItem
            // 
            this.reloadTilesetToolStripMenuItem.Name = "reloadTilesetToolStripMenuItem";
            this.reloadTilesetToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.R)));
            this.reloadTilesetToolStripMenuItem.Size = new System.Drawing.Size(275, 26);
            this.reloadTilesetToolStripMenuItem.Text = "Reload Tilesets";
            this.reloadTilesetToolStripMenuItem.Click += new System.EventHandler(this.reloadTilesetsToolStripMenuItem_Click);
            // 
            // stampToolStripMenuItem
            // 
            this.stampToolStripMenuItem.Name = "stampToolStripMenuItem";
            this.stampToolStripMenuItem.Size = new System.Drawing.Size(66, 24);
            this.stampToolStripMenuItem.Text = "Stamp";
            this.stampToolStripMenuItem.DropDownOpening += new System.EventHandler(this.stampToolStripMenuItem_DropDownOpening);
            // 
            // statusStrip1
            // 
            this.statusStrip1.ImageScalingSize = new System.Drawing.Size(20, 20);
            this.statusStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.cursorPositionLabel});
            this.statusStrip1.Location = new System.Drawing.Point(0, 879);
            this.statusStrip1.Name = "statusStrip1";
            this.statusStrip1.Padding = new System.Windows.Forms.Padding(1, 0, 19, 0);
            this.statusStrip1.Size = new System.Drawing.Size(1201, 26);
            this.statusStrip1.TabIndex = 3;
            this.statusStrip1.Text = "statusStrip1";
            // 
            // cursorPositionLabel
            // 
            this.cursorPositionLabel.Name = "cursorPositionLabel";
            this.cursorPositionLabel.Size = new System.Drawing.Size(120, 20);
            this.cursorPositionLabel.Text = "<not initialized>";
            // 
            // editorPanelBorder
            // 
            this.editorPanelBorder.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.editorPanelBorder.Controls.Add(this.editorPanel);
            this.editorPanelBorder.Dock = System.Windows.Forms.DockStyle.Fill;
            this.editorPanelBorder.Location = new System.Drawing.Point(265, 28);
            this.editorPanelBorder.Margin = new System.Windows.Forms.Padding(4);
            this.editorPanelBorder.Name = "editorPanelBorder";
            this.editorPanelBorder.Size = new System.Drawing.Size(936, 851);
            this.editorPanelBorder.TabIndex = 4;
            // 
            // editorPanel
            // 
            this.editorPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.editorPanel.Location = new System.Drawing.Point(0, 0);
            this.editorPanel.Margin = new System.Windows.Forms.Padding(4);
            this.editorPanel.Name = "editorPanel";
            this.editorPanel.Size = new System.Drawing.Size(932, 847);
            this.editorPanel.TabIndex = 0;
            this.editorPanel.Text = "wysiwygPanel1";
            // 
            // tilesetPanelBorder
            // 
            this.tilesetPanelBorder.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.tilesetPanelBorder.Controls.Add(this.tilesetPanel);
            this.tilesetPanelBorder.Dock = System.Windows.Forms.DockStyle.Left;
            this.tilesetPanelBorder.Location = new System.Drawing.Point(0, 28);
            this.tilesetPanelBorder.Margin = new System.Windows.Forms.Padding(4);
            this.tilesetPanelBorder.Name = "tilesetPanelBorder";
            this.tilesetPanelBorder.Size = new System.Drawing.Size(265, 851);
            this.tilesetPanelBorder.TabIndex = 5;
            // 
            // tilesetPanel
            // 
            this.tilesetPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tilesetPanel.Location = new System.Drawing.Point(0, 0);
            this.tilesetPanel.Margin = new System.Windows.Forms.Padding(4);
            this.tilesetPanel.Name = "tilesetPanel";
            this.tilesetPanel.Size = new System.Drawing.Size(261, 847);
            this.tilesetPanel.TabIndex = 1;
            this.tilesetPanel.SizeChanged += new System.EventHandler(this.tilesetPanel_SizeChanged);
            // 
            // editorGestures
            // 
            this.editorGestures.Target = this.editorPanel;
            // 
            // tilesetGestures
            // 
            this.tilesetGestures.Target = this.tilesetPanel;
            // 
            // View
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1201, 905);
            this.Controls.Add(this.editorPanelBorder);
            this.Controls.Add(this.tilesetPanelBorder);
            this.Controls.Add(this.statusStrip1);
            this.Controls.Add(this.menuStrip1);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MainMenuStrip = this.menuStrip1;
            this.Margin = new System.Windows.Forms.Padding(4);
            this.Name = "View";
            this.Text = "Patchwork";
            this.WindowState = System.Windows.Forms.FormWindowState.Maximized;
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.View_FormClosing);
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.statusStrip1.ResumeLayout(false);
            this.statusStrip1.PerformLayout();
            this.editorPanelBorder.ResumeLayout(false);
            this.tilesetPanelBorder.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private UnaryHeap.GUI.WysiwygPanel editorPanel;
        private UnaryHeap.GUI.GestureInterpreter editorGestures;
        private UnaryHeap.GUI.WysiwygPanel tilesetPanel;
        private UnaryHeap.GUI.GestureInterpreter tilesetGestures;
        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem viewToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem zoomInToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem zoomOutToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem toggleGridDisplayToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem exportToPNGToolStripMenuItem;
        private System.Windows.Forms.StatusStrip statusStrip1;
        private System.Windows.Forms.ToolStripStatusLabel cursorPositionLabel;
        private System.Windows.Forms.ToolStripMenuItem newToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem loadToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem saveAsToolStripMenuItem;
        private System.Windows.Forms.Panel editorPanelBorder;
        private System.Windows.Forms.Panel tilesetPanelBorder;
        private System.Windows.Forms.ToolStripMenuItem editToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem undoToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem redoToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem copyRenderedArrangementToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem addTilesetToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem openRecentToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem expandRightToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem expandBottomToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem expandLeftToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem expandTopToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem contractRightToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem contractLeftToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem contractBottomToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem contractTopToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem saveToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator3;
        private System.Windows.Forms.ToolStripMenuItem closeToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem reloadTilesetToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem stampToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem removeTilesetToolStripMenuItem;
    }
}


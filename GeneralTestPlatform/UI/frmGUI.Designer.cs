namespace GeneralTestPlatform.UI
{
    partial class frmGUI
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
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(frmGUI));
            this.timer1 = new System.Windows.Forms.Timer(this.components);
            this.timer2 = new System.Windows.Forms.Timer(this.components);
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.ToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
            this.ToolStripMenuItem_cal = new System.Windows.Forms.ToolStripMenuItem();
            this.ToolStripMenuItem2 = new System.Windows.Forms.ToolStripMenuItem();
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.tabPage_cal = new System.Windows.Forms.TabPage();
            this.treeView_cal = new System.Windows.Forms.TreeView();
            this.tabPage_test = new System.Windows.Forms.TabPage();
            this.flowLayoutPanel_log = new System.Windows.Forms.FlowLayoutPanel();
            this.menuStrip1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).BeginInit();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            this.tabControl1.SuspendLayout();
            this.tabPage_cal.SuspendLayout();
            this.SuspendLayout();
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.ToolStripMenuItem1,
            this.ToolStripMenuItem2});
            resources.ApplyResources(this.menuStrip1, "menuStrip1");
            this.menuStrip1.Name = "menuStrip1";
            // 
            // ToolStripMenuItem1
            // 
            this.ToolStripMenuItem1.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.ToolStripMenuItem_cal});
            this.ToolStripMenuItem1.Name = "ToolStripMenuItem1";
            resources.ApplyResources(this.ToolStripMenuItem1, "ToolStripMenuItem1");
            // 
            // ToolStripMenuItem_cal
            // 
            this.ToolStripMenuItem_cal.Name = "ToolStripMenuItem_cal";
            resources.ApplyResources(this.ToolStripMenuItem_cal, "ToolStripMenuItem_cal");
            this.ToolStripMenuItem_cal.Click += new System.EventHandler(this.ToolStripMenuItem_cal_Click);
            // 
            // ToolStripMenuItem2
            // 
            this.ToolStripMenuItem2.Name = "ToolStripMenuItem2";
            resources.ApplyResources(this.ToolStripMenuItem2, "ToolStripMenuItem2");
            this.ToolStripMenuItem2.Click += new System.EventHandler(this.ToolStripMenuItem2_Click);
            // 
            // splitContainer1
            // 
            resources.ApplyResources(this.splitContainer1, "splitContainer1");
            this.splitContainer1.Name = "splitContainer1";
            // 
            // splitContainer1.Panel1
            // 
            this.splitContainer1.Panel1.Controls.Add(this.tabControl1);
            // 
            // splitContainer1.Panel2
            // 
            this.splitContainer1.Panel2.Controls.Add(this.flowLayoutPanel_log);
            // 
            // tabControl1
            // 
            this.tabControl1.Controls.Add(this.tabPage_cal);
            this.tabControl1.Controls.Add(this.tabPage_test);
            resources.ApplyResources(this.tabControl1, "tabControl1");
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            // 
            // tabPage_cal
            // 
            this.tabPage_cal.Controls.Add(this.treeView_cal);
            resources.ApplyResources(this.tabPage_cal, "tabPage_cal");
            this.tabPage_cal.Name = "tabPage_cal";
            this.tabPage_cal.UseVisualStyleBackColor = true;
            // 
            // treeView_cal
            // 
            this.treeView_cal.CheckBoxes = true;
            resources.ApplyResources(this.treeView_cal, "treeView_cal");
            this.treeView_cal.Name = "treeView_cal";
            this.treeView_cal.Nodes.AddRange(new System.Windows.Forms.TreeNode[] {
            ((System.Windows.Forms.TreeNode)(resources.GetObject("treeView_cal.Nodes")))});
            this.treeView_cal.AfterCheck += new System.Windows.Forms.TreeViewEventHandler(this.treeView_cal_AfterCheck);
            // 
            // tabPage_test
            // 
            resources.ApplyResources(this.tabPage_test, "tabPage_test");
            this.tabPage_test.Name = "tabPage_test";
            this.tabPage_test.UseVisualStyleBackColor = true;
            // 
            // flowLayoutPanel_log
            // 
            resources.ApplyResources(this.flowLayoutPanel_log, "flowLayoutPanel_log");
            this.flowLayoutPanel_log.Name = "flowLayoutPanel_log";
            this.flowLayoutPanel_log.Layout += new System.Windows.Forms.LayoutEventHandler(this.flowLayoutPanel_log_Layout);
            // 
            // frmGUI
            // 
            resources.ApplyResources(this, "$this");
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.splitContainer1);
            this.Controls.Add(this.menuStrip1);
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "frmGUI";
            this.Resize += new System.EventHandler(this.frmGUI_Resize);
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.splitContainer1.Panel1.ResumeLayout(false);
            this.splitContainer1.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).EndInit();
            this.splitContainer1.ResumeLayout(false);
            this.tabControl1.ResumeLayout(false);
            this.tabPage_cal.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Timer timer1;
        private System.Windows.Forms.Timer timer2;
        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem ToolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem ToolStripMenuItem2;
        private System.Windows.Forms.ToolStripMenuItem ToolStripMenuItem_cal;
        private System.Windows.Forms.SplitContainer splitContainer1;
        private System.Windows.Forms.TabControl tabControl1;
        private System.Windows.Forms.TabPage tabPage_cal;
        public System.Windows.Forms.TreeView treeView_cal;
        private System.Windows.Forms.TabPage tabPage_test;
        public System.Windows.Forms.FlowLayoutPanel flowLayoutPanel_log;
    }
}
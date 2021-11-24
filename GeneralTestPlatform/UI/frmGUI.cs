using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using GeneralTestPlatform;
using System.Threading;
using System.Runtime.InteropServices;

namespace GeneralTestPlatform.UI
{
    public partial class frmGUI : Form
    {
        //存储一开始窗体的长和宽
        private double m_X;
        private double m_Y;


        public frmGUI()
        {
            InitializeComponent();
            //初始化私有字段
            m_X = this.Width;
            m_Y = this.Height;

            //初始化窗体内每个控件的tag
            GlobalConstants.setTag(this);
            //初始化控件的内容和属性
            Icon = new Icon(GlobalConstants.ResourceDir + "\\iot.ico");
            //非管理员不能配置
            if (!frmLogin.bAdmin) { menuStrip1.Items[0].Enabled = false; }
            this.treeView_cal.ExpandAll();
            this.WindowState = FormWindowState.Maximized;
        }
        //自动调整frmGUI的大小
        private void frmGUI_Resize(object sender, EventArgs e)
        {
            double newx = (this.Width) / m_X;
            double newy = this.Height / m_Y;
            GlobalConstants.setControls(newx, newy, this);
            //if (WindowState == FormWindowState.Normal) { WindowState = FormWindowState.Maximized; }
        }
        //实现智能勾选
        private void treeView_cal_AfterCheck(object sender, TreeViewEventArgs e)
        {
            if (e.Action == TreeViewAction.ByMouse)
            {
                GlobalConstants.UpdateChildNodes(e.Node);
                GlobalConstants.UpdateParentNodes(e.Node);
            }
        }
        //打开连接配置
        private void ToolStripMenuItem2_Click(object sender, EventArgs e)
        {
            LinkConfig frm = new LinkConfig();
            frm.Show(this);
        }
        //打开校准配置
        private void ToolStripMenuItem_cal_Click(object sender, EventArgs e)
        {
            CalConfig.getInstance().Show();
            UpdateCheckbox();
        }
        //流布局里面实现flowlayoutpanel的子控件大小调整
        private void flowLayoutPanel_log_Layout(object sender, LayoutEventArgs e)
        {
            foreach (Control c in flowLayoutPanel_log.Controls)
            {
                c.Height = Convert.ToInt32(flowLayoutPanel_log.Height * 0.96);
                c.Width = Convert.ToInt32(flowLayoutPanel_log.Width * 0.48);
                c.Margin = new Padding(0, 0, 10, 30);
            }
        }






        //更新CalConfig里面的checkbox的状态
        void UpdateCheckbox()
        {
            //APC
            TreeNode bandCalEnable = FindNode(CalConfig.getInstance().treeView1.Nodes[0],"频段校准使能");
            bool flag = false;
            for (int i = 0; i < bandCalEnable.Nodes.Count; i++)
            {
                flag = ((bandCalEnable.Nodes[i].Checked = CalConfig.getInstance().apcConfig.bandCalEnable[i]) || flag);
            }
            TreeNode bCWSinaling = FindNode(CalConfig.getInstance().treeView1.Nodes[0], "上行波标志位");
            bCWSinaling.Checked = CalConfig.getInstance().apcConfig.bCWSinaling;
            bandCalEnable.Checked = flag;
            //AFC
            TreeNode bCWSinaling_afc = FindNode(CalConfig.getInstance().treeView2.Nodes[0], "上行波标志位");
            bCWSinaling_afc.Checked = CalConfig.getInstance().afcConfig.bCWSinaling;
            //AGC
            TreeNode bandCalEnable_agc = FindNode(CalConfig.getInstance().treeView3.Nodes[0],"频段校准使能");
            bool flag_agc = false;
            for (int i = 0; i < bandCalEnable_agc.Nodes.Count; i++)
            {
                flag_agc = ((bandCalEnable_agc.Nodes[i].Checked = CalConfig.getInstance().agcConfig.bandCalEnable[i]) || flag_agc);
            }
            bandCalEnable_agc.Checked = flag_agc;
        }
        //递归查询,找到返回该节点
        private TreeNode FindNode(TreeNode node, string name)
        {
            TreeNode ret = null;
            foreach (TreeNode temp in node.Nodes)
            {
                if (temp.Nodes.Count != 0)
                {
                    if ((ret = FindNode(temp, name)) != null)
                    {
                        return ret;
                    }
                }
                if (string.Equals(temp.Text, name))
                {
                    return temp;
                }
            }
            return ret;
        }

    }
}

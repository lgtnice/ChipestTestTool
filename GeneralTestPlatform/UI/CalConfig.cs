using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Xml;
using ColloCalNamespace;
using System.Runtime.InteropServices;
using System.Collections;
using System.Text.RegularExpressions;

namespace GeneralTestPlatform.UI
{
    public partial class CalConfig : Form
    {
        public AFCConf afcConfig;
        public APCConf apcConfig;
        public AGCConf agcConfig;

        //存储一开始窗体的长和宽
        private double m_X;
        private double m_Y;

        private static CalConfig calInstance = new CalConfig();

        private CalConfig()
        {
            UpdateMemoryConfig();
            InitializeComponent();
            //初始化私有字段
            m_X = this.Width;
            m_Y = this.Height;

            //初始化窗体内每个控件的tag
            GlobalConstants.setTag(this);

            this.treeView1.ExpandAll();
            this.treeView2.ExpandAll();
            this.treeView3.ExpandAll();
        }
        ~CalConfig()
        {
            UpdateDiskConfig();
        }

        public static CalConfig getInstance()
        {
            return calInstance;
        }
        private void CalConfig_Resize(object sender, EventArgs e)
        {
            double newx = (this.Width) / m_X;
            double newy = this.Height / m_Y;
            GlobalConstants.setControls(newx, newy, this);
        }
        private void treeView1_DrawNode(object sender, DrawTreeNodeEventArgs e)
        {
            //隐藏节点前的checkbox
            if (e.Node.Parent == null)
            {
                HideCheckBox(this.treeView1, e.Node);
            }
            else if( (e.Node.Text != "频段校准使能") && (e.Node.Parent.Text != "频段校准使能") && (e.Node.Text != "上行波标志位"))
            {
                HideCheckBox(this.treeView1, e.Node);
            }

            e.DrawDefault = true;
        }
        private void treeView2_DrawNode(object sender, DrawTreeNodeEventArgs e)
        {
            if (e.Node.Text != "上行波标志位") { HideCheckBox(this.treeView2,e.Node); }
            e.DrawDefault = true;
        }
        private void treeView3_DrawNode(object sender, DrawTreeNodeEventArgs e)
        {
            if (e.Node.Parent == null)
            {
                HideCheckBox(this.treeView3, e.Node);
            }
            else if ((e.Node.Text != "频段校准使能") && (e.Node.Parent.Text != "频段校准使能"))
            {
                HideCheckBox(this.treeView3, e.Node);
            }

            e.DrawDefault = true;
        }
        private void treeView1_AfterCheck(object sender, TreeViewEventArgs e)
        {
            if (e.Action == TreeViewAction.ByMouse)
            {
                if (e.Node.Parent.Text == "频段校准使能")
                {
                    for (int i = 0; i < e.Node.Parent.Nodes.Count; i++)
                    {
                        this.apcConfig.bandCalEnable[i] = e.Node.Parent.Nodes[i].Checked;
                    }
                }
                if (e.Node.Text == "上行波标志位") { this.apcConfig.bCWSinaling = e.Node.Checked; }
                GlobalConstants.UpdateChildNodes(e.Node);
                GlobalConstants.UpdateParentNodes(e.Node);
            }
        }
        private void treeView2_AfterCheck(object sender, TreeViewEventArgs e)
        {
            if (e.Action == TreeViewAction.ByMouse)
            {
                if (e.Node.Text == "上行波标志位") { this.afcConfig.bCWSinaling = e.Node.Checked; }
            }
        }
        private void treeView3_AfterCheck(object sender, TreeViewEventArgs e)
        {
            if (e.Action == TreeViewAction.ByMouse)
            {
                if (e.Node.Parent.Text == "频段校准使能")
                {
                    for (int i = 0; i < e.Node.Parent.Nodes.Count; i++)
                    {
                        this.agcConfig.bandCalEnable[i] = e.Node.Parent.Nodes[i].Checked;
                    }
                }
                GlobalConstants.UpdateChildNodes(e.Node);
                GlobalConstants.UpdateParentNodes(e.Node);
            }
        }
        private void treeView1_AfterSelect(object sender, TreeViewEventArgs e)
        {
            if (e.Node.Parent == null) { this.textBox1.Clear(); return; }
            if (e.Node.Parent.Text == "中间信道")
            {
                this.textBox1.Clear();
                this.textBox1.AppendText(e.Node.Parent.Text + "\\" + "在此位置的值为：");
                this.textBox1.AppendText(this.apcConfig.centerChannel[e.Node.Index].ToString());
                return;
            }
            if (e.Node.Parent.Text == "高补偿信道")
            {
                this.textBox1.Clear();
                this.textBox1.AppendText(e.Node.Parent.Text + "\\" + "在此位置的值为：");
                this.textBox1.AppendText(this.apcConfig.compChannelHigh[e.Node.Index].ToString());
                return;
            }
            if (e.Node.Parent.Text == "低补偿信道")
            {
                this.textBox1.Clear();
                this.textBox1.AppendText(e.Node.Parent.Text + "\\" + "在此位置的值为：");
                this.textBox1.AppendText(this.apcConfig.compChannelLow[e.Node.Index].ToString());
                return;
            }
            if (e.Node.Parent.Text == "第一个功率点的APC控制字")
            {
                this.textBox1.Clear();
                this.textBox1.AppendText(e.Node.Parent.Text + "\\" + "在此位置的值为：");
                this.textBox1.AppendText(this.apcConfig.apcData1[e.Node.Index].ToString());
                return;
            }
            if (e.Node.Parent.Text == "第二个功率点的APC控制字")
            {
                this.textBox1.Clear();
                this.textBox1.AppendText(e.Node.Parent.Text + "\\" + "在此位置的值为：");
                this.textBox1.AppendText(this.apcConfig.apcData2[e.Node.Index].ToString());
                return;
            }
            if (e.Node.Text == "初始化功率控制字")
            {
                this.textBox1.Clear();
                this.textBox1.AppendText(e.Node.Text + "的值为：");
                this.textBox1.AppendText(this.apcConfig.initApcData.ToString());
                return;
            }
            if (e.Node.Text == "初始化PA档位")
            {
                this.textBox1.Clear();
                this.textBox1.AppendText(e.Node.Text + "的值为：");
                this.textBox1.AppendText(this.apcConfig.paMode.ToString());
                return;
            }
            if (e.Node.Text == "初始化发射机频率")
            {
                this.textBox1.Clear();
                this.textBox1.AppendText(e.Node.Text + "的值为：");
                this.textBox1.AppendText(this.apcConfig.freq.ToString("F2"));
                return;
            }
            if (e.Node.Parent.Text == "中心频点的上行线损")
            {
                this.textBox1.Clear();
                this.textBox1.AppendText(e.Node.Parent.Text + "\\" + "在此位置的值为：");
                this.textBox1.AppendText(this.apcConfig.centerFreqPathLossUL[e.Node.Index].ToString("F2"));
                return;
            }
            if (e.Node.Parent.Text == "高频点的上行线损")
            {
                this.textBox1.Clear();
                this.textBox1.AppendText(e.Node.Parent.Text + "\\" + "在此位置的值为：");
                this.textBox1.AppendText(this.apcConfig.compFreqPathLossHighUL[e.Node.Index].ToString("F2"));
                return;
            }
            if (e.Node.Parent.Text == "低频点的上行线损")
            {
                this.textBox1.Clear();
                this.textBox1.AppendText(e.Node.Parent.Text + "\\" + "在此位置的值为：");
                this.textBox1.AppendText(this.apcConfig.compFreqPathLossLowUL[e.Node.Index].ToString("F2"));
                return;
            }
            this.textBox1.Clear();
        }
        private void treeView2_AfterSelect(object sender, TreeViewEventArgs e)
        {
            switch (e.Node.Text)
            {
                case "UE发射机功率控制字":
                    {
                        this.textBox2.Clear();
                        this.textBox2.AppendText(e.Node.Text + "\\" + "在此位置的值为：");
                        this.textBox2.AppendText(this.afcConfig.apcData.ToString());
                        break;
                    }
                case "UE发射机PA档位":
                    {
                        this.textBox2.Clear();
                        this.textBox2.AppendText(e.Node.Text + "\\" + "在此位置的值为：");
                        this.textBox2.AppendText(this.afcConfig.paMode.ToString());
                        break;
                    }
                case "第一个AFC控制字":
                    {
                        this.textBox2.Clear();
                        this.textBox2.AppendText(e.Node.Text + "\\" + "在此位置的值为：");
                        this.textBox2.AppendText(this.afcConfig.afcData1.ToString());
                        break;
                    }
                case "第二个AFC控制字":
                    {
                        this.textBox2.Clear();
                        this.textBox2.AppendText(e.Node.Text + "\\" + "在此位置的值为：");
                        this.textBox2.AppendText(this.afcConfig.afcData2.ToString());
                        break;
                    }
                case "AFC微调循环次数":
                    {
                        this.textBox2.Clear();
                        this.textBox2.AppendText(e.Node.Text + "\\" + "在此位置的值为：");
                        this.textBox2.AppendText(this.afcConfig.cycleNum.ToString());
                        break;
                    }
                case "上行频点":
                    {
                        this.textBox2.Clear();
                        this.textBox2.AppendText(e.Node.Text + "\\" + "在此位置的值为：");
                        this.textBox2.AppendText(this.afcConfig.freq.ToString("F2"));
                        break;
                    }
                case "上行期望功率":
                    {
                        this.textBox2.Clear();
                        this.textBox2.AppendText(e.Node.Text + "\\" + "在此位置的值为：");
                        this.textBox2.AppendText(this.afcConfig.expPower.ToString("F2"));
                        break;
                    }
                case "线损":
                    {
                        this.textBox2.Clear();
                        this.textBox2.AppendText(e.Node.Text + "\\" + "在此位置的值为：");
                        this.textBox2.AppendText(this.afcConfig.pathLoss.ToString("F2"));
                        break;
                    }
                default:
                    {
                        this.textBox2.Clear();
                        break;
                    }
            }
        }
        private void treeView3_AfterSelect(object sender, TreeViewEventArgs e)
        {
            if (e.Node.Parent == null) { this.textBox3.Clear(); return; }
            if (e.Node.Parent.Text == "中间信道")
            {
                this.textBox3.Clear();
                this.textBox3.AppendText(e.Node.Parent.Text + "\\" + "在此位置的值为：");
                this.textBox3.AppendText(this.agcConfig.centerChannel[e.Node.Index].ToString());
                return;
            }
            if (e.Node.Parent.Text == "高补偿信道")
            {
                this.textBox3.Clear();
                this.textBox3.AppendText(e.Node.Parent.Text + "\\" + "在此位置的值为：");
                this.textBox3.AppendText(this.agcConfig.compChannelHigh[e.Node.Index].ToString());
                return;
            }
            if (e.Node.Parent.Text == "低补偿信道")
            {
                this.textBox3.Clear();
                this.textBox3.AppendText(e.Node.Parent.Text + "\\" + "在此位置的值为：");
                this.textBox3.AppendText(this.agcConfig.compChannelLow[e.Node.Index].ToString());
                return;
            }
            if (e.Node.Parent.Text == "仪表下行功率")
            {
                this.textBox3.Clear();
                this.textBox3.AppendText(e.Node.Parent.Text + "\\" + "在此位置的值为：");
                this.textBox3.AppendText(this.agcConfig.agcPower[e.Node.Index].ToString("F2"));
                return;
            }
            if (e.Node.Text == "初始化接收机频率")
            {
                this.textBox3.Clear();
                this.textBox3.AppendText(e.Node.Text + "\\" + "的值为：");
                this.textBox3.AppendText(this.agcConfig.freq.ToString("F2"));
                return;
            }
            if (e.Node.Text == "初始化接收机增益档位")
            {
                this.textBox3.Clear();
                this.textBox3.AppendText(e.Node.Text + "\\" + "的值为：");
                this.textBox3.AppendText(this.agcConfig.rxGainMode.ToString("F2"));
                return;
            }
            if (e.Node.Text == "初始化仪表下行功率")
            {
                this.textBox3.Clear();
                this.textBox3.AppendText(e.Node.Text + "\\" + "的值为：");
                this.textBox3.AppendText(this.agcConfig.cellPower.ToString("F2"));
                return;
            }
            if (e.Node.Parent.Text == "中心频点的下行线损")
            {
                this.textBox3.Clear();
                this.textBox3.AppendText(e.Node.Parent.Text + "\\" + "在此位置的值为：");
                this.textBox3.AppendText(this.agcConfig.centerFreqPathLossDL[e.Node.Index].ToString("F2"));
                return;
            }
            if (e.Node.Parent.Text == "高频点的下行线损")
            {
                this.textBox3.Clear();
                this.textBox3.AppendText(e.Node.Parent.Text + "\\" + "在此位置的值为：");
                this.textBox3.AppendText(this.agcConfig.compFreqPathLossHighDL[e.Node.Index].ToString("F2"));
                return;
            }
            if (e.Node.Parent.Text == "低频点的下行线损")
            {
                this.textBox3.Clear();
                this.textBox3.AppendText(e.Node.Parent.Text + "\\" + "在此位置的值为：");
                this.textBox3.AppendText(this.agcConfig.compFreqPathLossLowDL[e.Node.Index].ToString("F2"));
                return;
            }
            this.textBox3.Clear();
        }
        private void button1_Click(object sender, EventArgs e)
        {
            TreeNode node = this.treeView1.SelectedNode;
            if (node.Parent == null) { MessageBox.Show("保存失败");return; }
            if (node.Parent.Text == "中间信道")
            {
                int result;
                if (int.TryParse(Regex.Replace(this.textBox1.Text, @"[^\d.]*", "") , out result)) { this.apcConfig.centerChannel[node.Index] = result; }
                else { MessageBox.Show("保存失败"); }
                return;
            }
            if (node.Parent.Text == "高补偿信道")
            {
                int result;
                if (int.TryParse(Regex.Replace(this.textBox1.Text, @"[^\d.]*", ""), out result)) { this.apcConfig.compChannelHigh[node.Index] = result; }
                else { MessageBox.Show("保存失败"); }
                return;
            }
            if (node.Parent.Text == "低补偿信道")
            {
                int result;
                if (int.TryParse(Regex.Replace(this.textBox1.Text, @"[^\d.]*", ""), out result)) { this.apcConfig.compChannelLow[node.Index] = result; }
                else { MessageBox.Show("保存失败"); }
                return;
            }
            if (node.Parent.Text == "第一个功率点的APC控制字")
            {
                int result;
                if (int.TryParse(Regex.Replace(this.textBox1.Text, @"[^\d.]*", ""), out result)) { this.apcConfig.apcData1[node.Index] = result; }
                else { MessageBox.Show("保存失败"); }
                return;
            }
            if (node.Parent.Text == "第二个功率点的APC控制字")
            {
                int result;
                if (int.TryParse(Regex.Replace(this.textBox1.Text, @"[^\d.]*", ""), out result)) { this.apcConfig.apcData2[node.Index] = result; }
                else { MessageBox.Show("保存失败"); }
                return;
            }
            if (node.Text == "初始化功率控制字")
            {
                int result;
                if (int.TryParse(Regex.Replace(this.textBox1.Text, @"[^\d.]*", ""), out result)) { this.apcConfig.initApcData = result; }
                else { MessageBox.Show("保存失败"); }
                return;
            }
            if (node.Text == "初始化PA档位")
            {
                int result;
                if (int.TryParse(Regex.Replace(this.textBox1.Text, @"[^\d.]*", ""), out result)) { this.apcConfig.paMode = result; }
                else { MessageBox.Show("保存失败"); }
                return;
            }
            if (node.Text == "初始化发射机频率")
            {
                double result;
                if (double.TryParse(Regex.Replace(this.textBox1.Text, @"[^\d.]*", ""), out result)) { this.apcConfig.freq = result; }
                else { MessageBox.Show("保存失败"); }
                return;
            }
            if (node.Parent.Text == "中心频点的上行线损")
            {
                double result;
                if (double.TryParse(Regex.Replace(this.textBox1.Text, @"[^\d.]*", ""), out result)) { this.apcConfig.centerFreqPathLossUL[node.Index] = result; }
                else { MessageBox.Show("保存失败"); }
                return;
            }
            if (node.Parent.Text == "高频点的上行线损")
            {
                double result;
                if (double.TryParse(Regex.Replace(this.textBox1.Text, @"[^\d.]*", ""), out result)) { this.apcConfig.compFreqPathLossHighUL[node.Index] = result; }
                else { MessageBox.Show("保存失败"); }
                return;
            }
            if (node.Parent.Text == "低频点的上行线损")
            {
                double result;
                if (double.TryParse(Regex.Replace(this.textBox1.Text, @"[^\d.]*", ""), out result)) { this.apcConfig.compFreqPathLossLowUL[node.Index] = result; }
                else { MessageBox.Show("保存失败"); }
                return;
            }
            MessageBox.Show("保存失败");
        }
        private void button2_Click(object sender, EventArgs e)
        {
            TreeNode node = this.treeView2.SelectedNode;
            switch (node.Text)
            {
                case "UE发射机功率控制字":
                    {
                        int result;
                        if (int.TryParse(Regex.Replace(this.textBox2.Text, @"[^\d.]*", ""), out result)) { this.afcConfig.apcData = result; }
                        else { MessageBox.Show("保存失败"); }
                        break;
                    }
                case "UE发射机PA档位":
                    {
                        int result;
                        if (int.TryParse(Regex.Replace(this.textBox2.Text, @"[^\d.]*", ""), out result)) { this.afcConfig.paMode = result; }
                        else { MessageBox.Show("保存失败"); }
                        break;
                    }
                case "第一个AFC控制字":
                    {
                        int result;
                        if (int.TryParse(Regex.Replace(this.textBox2.Text, @"[^\d.]*", ""), out result)) { this.afcConfig.afcData1 = result; }
                        else { MessageBox.Show("保存失败"); }
                        break;
                    }
                case "第二个AFC控制字":
                    {
                        int result;
                        if (int.TryParse(Regex.Replace(this.textBox2.Text, @"[^\d.]*", ""), out result)) { this.afcConfig.afcData2 = result; }
                        else { MessageBox.Show("保存失败"); }
                        break;
                    }
                case "AFC微调循环次数":
                    {
                        int result;
                        if (int.TryParse(Regex.Replace(this.textBox2.Text, @"[^\d.]*", ""), out result)) { this.afcConfig.cycleNum = result; }
                        else { MessageBox.Show("保存失败"); }
                        break;
                    }
                case "上行频点":
                    {
                        double result;
                        if (double.TryParse(Regex.Replace(this.textBox2.Text, @"[^\d.]*", ""), out result)) { this.afcConfig.freq = result; }
                        else { MessageBox.Show("保存失败"); }
                        break;
                    }
                case "上行期望功率":
                    {
                        double result;
                        if (double.TryParse(Regex.Replace(this.textBox2.Text, @"[^\d.]*", ""), out result)) { this.afcConfig.expPower = result; }
                        else { MessageBox.Show("保存失败"); }
                        break;
                    }
                case "线损":
                    {
                        double result;
                        if (double.TryParse(Regex.Replace(this.textBox2.Text, @"[^\d.]*", ""), out result)) { this.afcConfig.pathLoss = result; }
                        else { MessageBox.Show("保存失败"); }
                        break;
                    }
                default:
                    {
                        MessageBox.Show("保存失败");
                        break;
                    }
            }
        }
        private void button3_Click(object sender, EventArgs e)
        {
            TreeNode node = this.treeView3.SelectedNode;
            if (node.Parent == null) { MessageBox.Show("保存失败"); return; }
            if (node.Parent.Text == "中间信道")
            {
                int result;
                if (int.TryParse(Regex.Replace(this.textBox3.Text, @"[^\d.]*", ""), out result)) { this.agcConfig.centerChannel[node.Index] = result; }
                else { MessageBox.Show("保存失败"); }
                return;
            }
            if (node.Parent.Text == "高补偿信道")
            {
                int result;
                if (int.TryParse(Regex.Replace(this.textBox3.Text, @"[^\d.]*", ""), out result)) { this.agcConfig.compChannelHigh[node.Index] = result; }
                else { MessageBox.Show("保存失败"); }
                return;
            }
            if (node.Parent.Text == "低补偿信道")
            {
                int result;
                if (int.TryParse(Regex.Replace(this.textBox3.Text, @"[^\d.]*", ""), out result)) { this.agcConfig.compChannelLow[node.Index] = result; }
                else { MessageBox.Show("保存失败"); }
                return;
            }
            if (node.Parent.Text == "仪表下行功率")
            {
                double result;
                if (double.TryParse(Regex.Replace(this.textBox3.Text, @"[^\d.]*", ""), out result)) { this.agcConfig.agcPower[node.Index] = result; }
                else { MessageBox.Show("保存失败"); }
                return;
            }
            if (node.Text == "初始化接收机频率")
            {
                double result;
                if (double.TryParse(Regex.Replace(this.textBox3.Text, @"[^\d.]*", ""), out result)) { this.agcConfig.freq = result; }
                else { MessageBox.Show("保存失败"); }
                return;
            }
            if (node.Text == "初始化接收机增益档位")
            {
                double result;
                if (double.TryParse(Regex.Replace(this.textBox3.Text, @"[^\d.]*", ""), out result)) { this.agcConfig.rxGainMode = result; }
                else { MessageBox.Show("保存失败"); }
                return;
            }
            if (node.Text == "初始化仪表下行功率")
            {
                double result;
                if (double.TryParse(Regex.Replace(this.textBox3.Text, @"[^\d.]*", ""), out result)) { this.agcConfig.cellPower = result; }
                else { MessageBox.Show("保存失败"); }
                return;
            }
            if (node.Parent.Text == "中心频点的下行线损")
            {
                double result;
                if (double.TryParse(Regex.Replace(this.textBox3.Text, @"[^\d.]*", ""), out result)) { this.agcConfig.centerFreqPathLossDL[node.Index] = result; }
                else { MessageBox.Show("保存失败"); }
                return;
            }
            if (node.Parent.Text == "高频点的下行线损")
            {
                double result;
                if (double.TryParse(Regex.Replace(this.textBox3.Text, @"[^\d.]*", ""), out result)) { this.agcConfig.compFreqPathLossHighDL[node.Index] = result; }
                else { MessageBox.Show("保存失败"); }
                return;
            }
            if (node.Parent.Text == "低频点的下行线损")
            {
                double result;
                if (double.TryParse(Regex.Replace(this.textBox3.Text, @"[^\d.]*", ""), out result)) { this.agcConfig.compFreqPathLossLowDL[node.Index] = result; }
                else { MessageBox.Show("保存失败"); }
                return;
            }

            MessageBox.Show("保存失败");
        }
        private void CalConfig_FormClosing(object sender, FormClosingEventArgs e)
        {
            this.Hide();
            e.Cancel = true;
        }


        private const int TVIF_STATE = 0x8;
        private const int TVIS_STATEIMAGEMASK = 0xF000;
        private const int TV_FIRST = 0x1100;
        private const int TVM_SETITEM = TV_FIRST + 63;
        private void HideCheckBox(TreeView tvw, TreeNode node)
        {
            TVITEM tvi = new TVITEM();
            tvi.hItem = node.Handle;
            tvi.mask = TVIF_STATE;
            tvi.stateMask = TVIS_STATEIMAGEMASK;
            tvi.state = 0;
            SendMessage(tvw.Handle, TVM_SETITEM, IntPtr.Zero, ref tvi);
        }
        [StructLayout(LayoutKind.Sequential, Pack = 8, CharSet = CharSet.Auto)]
        private struct TVITEM
        {
            public int mask;
            public IntPtr hItem;
            public int state;
            public int stateMask;
            [MarshalAs(UnmanagedType.LPTStr)]
            public string lpszText;
            public int cchTextMax;
            public int iImage;
            public int iSelectedImage; public int cChildren; public IntPtr lParam;
        }
        [DllImport("user32.dll", CharSet = CharSet.Auto)]
        private static extern IntPtr SendMessage(IntPtr hWnd, int Msg, IntPtr wParam, ref TVITEM lParam);

        
        private void UpdateMemoryConfig()
        {
            string path = GlobalConstants.ConfigDir + "\\calConfig.xml";
            XmlDocument xmlFile = new XmlDocument();
            xmlFile.Load(path);

            UpdateMemoryConfigAFC(xmlFile);
            UpdateMemoryConfigAPC(xmlFile);
            UpdateMemoryConfigAGC(xmlFile);

        }
        private void UpdateDiskConfig()
        {
            string path = GlobalConstants.ConfigDir + "\\calConfig.xml";
            XmlDocument xmlFile = new XmlDocument();
            xmlFile.Load(path);

            UpdateDiskConfigAFC(xmlFile);
            UpdateDiskConfigAPC(xmlFile);
            UpdateDiskConfigAGC(xmlFile);

            xmlFile.Save(GlobalConstants.ConfigDir + "\\calConfig.xml");
        }
        private void UpdateMemoryConfigAFC(XmlDocument doc)
        {
            XmlNode afc = doc.SelectSingleNode("/config/afcConfig");
            this.afcConfig.apcData = Convert.ToInt32(afc.SelectSingleNode("apcData").InnerText);
            this.afcConfig.paMode = Convert.ToInt32(afc.SelectSingleNode("paMode").InnerText);
            this.afcConfig.afcData1 = Convert.ToInt32(afc.SelectSingleNode("afcData1").InnerText);
            this.afcConfig.afcData2 = Convert.ToInt32(afc.SelectSingleNode("afcData2").InnerText);
            this.afcConfig.cycleNum = Convert.ToInt32(afc.SelectSingleNode("cycleNum").InnerText);
            this.afcConfig.freq = Convert.ToDouble(afc.SelectSingleNode("freq").InnerText);
            this.afcConfig.expPower = Convert.ToDouble(afc.SelectSingleNode("expPower").InnerText);
            this.afcConfig.pathLoss = Convert.ToDouble(afc.SelectSingleNode("pathLoss").InnerText);
            this.afcConfig.bCWSinaling = Convert.ToBoolean(afc.SelectSingleNode("bCWSinaling").InnerText);
        }
        private void UpdateDiskConfigAFC(XmlDocument doc)
        {
            XmlNode afc = doc.SelectSingleNode("/config/afcConfig");
            afc.SelectSingleNode("apcData").InnerText = this.afcConfig.apcData.ToString();
            afc.SelectSingleNode("paMode").InnerText = this.afcConfig.paMode.ToString();
            afc.SelectSingleNode("afcData1").InnerText = this.afcConfig.afcData1.ToString();
            afc.SelectSingleNode("afcData2").InnerText = this.afcConfig.afcData2.ToString();
            afc.SelectSingleNode("cycleNum").InnerText = this.afcConfig.cycleNum.ToString();
            afc.SelectSingleNode("freq").InnerText = this.afcConfig.freq.ToString("F2");
            afc.SelectSingleNode("expPower").InnerText = this.afcConfig.expPower.ToString("F2");
            afc.SelectSingleNode("pathLoss").InnerText = this.afcConfig.pathLoss.ToString("F2");
            afc.SelectSingleNode("bCWSinaling").InnerText = this.afcConfig.bCWSinaling.ToString();
        }
        private void UpdateMemoryConfigAPC(XmlDocument doc)
        {
            XmlNode apc = doc.SelectSingleNode("/config/apcConfig");
            XmlNode bandCalEnable = apc.SelectSingleNode("bandCalEnable");
            List<bool> bandCalEnableF = new List<bool>();
            foreach (XmlNode node in bandCalEnable.ChildNodes)
            {
                bandCalEnableF.Add(Convert.ToBoolean(node.InnerText));
            }
            this.apcConfig.bandCalEnable = bandCalEnableF.ToArray();
            //-----------------------------------------------------------
            XmlNode centerChannel = apc.SelectSingleNode("centerChannel");
            List<int> centerChannelF = new List<int>();
            foreach (XmlNode node in centerChannel.ChildNodes)
            {
                centerChannelF.Add(Convert.ToInt32(node.InnerText));
            }
            this.apcConfig.centerChannel = centerChannelF.ToArray();
            //-----------------------------------------------------------
            XmlNode compChannelHigh = apc.SelectSingleNode("compChannelHigh");
            List<int> compChannelHighF = new List<int>();
            foreach (XmlNode node in compChannelHigh.ChildNodes)
            {
                compChannelHighF.Add(Convert.ToInt32(node.InnerText));
            }
            this.apcConfig.compChannelHigh = compChannelHighF.ToArray();
            //-----------------------------------------------------------
            XmlNode compChannelLow = apc.SelectSingleNode("compChannelLow");
            List<int> compChannelLowF = new List<int>();
            foreach (XmlNode node in compChannelLow.ChildNodes)
            {
                compChannelLowF.Add(Convert.ToInt32(node.InnerText));
            }
            this.apcConfig.compChannelLow = compChannelLowF.ToArray();
            //-----------------------------------------------------------
            XmlNode apcData1 = apc.SelectSingleNode("apcData1");
            List<int> apcData1F = new List<int>();
            foreach (XmlNode node in apcData1.ChildNodes)
            {
                apcData1F.Add(Convert.ToInt32(node.InnerText));
            }
            this.apcConfig.apcData1 = apcData1F.ToArray();
            //-----------------------------------------------------------
            XmlNode apcData2 = apc.SelectSingleNode("apcData2");
            List<int> apcData2F = new List<int>();
            foreach (XmlNode node in apcData2.ChildNodes)
            {
                apcData2F.Add(Convert.ToInt32(node.InnerText));
            }
            this.apcConfig.apcData2 = apcData2F.ToArray();
            //-----------------------------------------------------------
            this.apcConfig.initApcData = Convert.ToInt32(apc.SelectSingleNode("initApcData").InnerText);
            //-----------------------------------------------------------
            this.apcConfig.paMode = Convert.ToInt32(apc.SelectSingleNode("paMode").InnerText);
            //-----------------------------------------------------------
            this.apcConfig.freq = Convert.ToDouble(apc.SelectSingleNode("freq").InnerText);
            //-----------------------------------------------------------
            XmlNode centerFreqPathLossUL = apc.SelectSingleNode("centerFreqPathLossUL");
            List<double> centerFreqPathLossULF = new List<double>();
            foreach (XmlNode node in centerFreqPathLossUL.ChildNodes)
            {
                centerFreqPathLossULF.Add(Convert.ToDouble(node.InnerText));
            }
            this.apcConfig.centerFreqPathLossUL = centerFreqPathLossULF.ToArray();
            //-----------------------------------------------------------
            XmlNode compFreqPathLossHighUL = apc.SelectSingleNode("compFreqPathLossHighUL");
            List<double> compFreqPathLossHighULF = new List<double>();
            foreach (XmlNode node in compFreqPathLossHighUL.ChildNodes)
            {
                compFreqPathLossHighULF.Add(Convert.ToDouble(node.InnerText));
            }
            this.apcConfig.compFreqPathLossHighUL = compFreqPathLossHighULF.ToArray();
            //-----------------------------------------------------------
            XmlNode compFreqPathLossLowUL = apc.SelectSingleNode("compFreqPathLossLowUL");
            List<double> compFreqPathLossLowULF = new List<double>();
            foreach (XmlNode node in compFreqPathLossLowUL.ChildNodes)
            {
                compFreqPathLossLowULF.Add(Convert.ToDouble(node.InnerText));
            }
            this.apcConfig.compFreqPathLossLowUL = compFreqPathLossLowULF.ToArray();
            //-----------------------------------------------------------
            this.apcConfig.bCWSinaling = Convert.ToBoolean(apc.SelectSingleNode("bCWSinaling").InnerText);
        }
        private void UpdateDiskConfigAPC(XmlDocument doc)
        {
            XmlNode apc = doc.SelectSingleNode("/config/apcConfig");

            for (int i = 0; i < apc.SelectSingleNode("bandCalEnable").ChildNodes.Count; i++)
            {
                apc.SelectSingleNode("bandCalEnable").ChildNodes[i].InnerText = this.apcConfig.bandCalEnable[i].ToString();
            }
            for (int i = 0; i < apc.SelectSingleNode("centerChannel").ChildNodes.Count; i++)
            {
                apc.SelectSingleNode("centerChannel").ChildNodes[i].InnerText = this.apcConfig.centerChannel[i].ToString();
            }
            for (int i = 0; i < apc.SelectSingleNode("compChannelHigh").ChildNodes.Count; i++)
            {
                apc.SelectSingleNode("compChannelHigh").ChildNodes[i].InnerText = this.apcConfig.compChannelHigh[i].ToString();
            }
            for (int i = 0; i < apc.SelectSingleNode("compChannelLow").ChildNodes.Count; i++)
            {
                apc.SelectSingleNode("compChannelLow").ChildNodes[i].InnerText = this.apcConfig.compChannelLow[i].ToString();
            }
            for (int i = 0; i < apc.SelectSingleNode("apcData1").ChildNodes.Count; i++)
            {
                apc.SelectSingleNode("apcData1").ChildNodes[i].InnerText = this.apcConfig.apcData1[i].ToString();
            }
            for (int i = 0; i < apc.SelectSingleNode("apcData2").ChildNodes.Count; i++)
            {
                apc.SelectSingleNode("apcData2").ChildNodes[i].InnerText = this.apcConfig.apcData2[i].ToString();
            }
            apc.SelectSingleNode("initApcData").InnerText = this.apcConfig.initApcData.ToString();
            apc.SelectSingleNode("paMode").InnerText = this.apcConfig.paMode.ToString();
            apc.SelectSingleNode("freq").InnerText = this.apcConfig.freq.ToString("F2");
            for (int i = 0; i < apc.SelectSingleNode("centerFreqPathLossUL").ChildNodes.Count; i++)
            {
                apc.SelectSingleNode("centerFreqPathLossUL").ChildNodes[i].InnerText = this.apcConfig.centerFreqPathLossUL[i].ToString("F2");
            }
            for (int i = 0; i < apc.SelectSingleNode("compFreqPathLossHighUL").ChildNodes.Count; i++)
            {
                apc.SelectSingleNode("compFreqPathLossHighUL").ChildNodes[i].InnerText = this.apcConfig.compFreqPathLossHighUL[i].ToString("F2");
            }
            for (int i = 0; i < apc.SelectSingleNode("compFreqPathLossLowUL").ChildNodes.Count; i++)
            {
                apc.SelectSingleNode("compFreqPathLossLowUL").ChildNodes[i].InnerText = this.apcConfig.compFreqPathLossLowUL[i].ToString("F2");
            }
            apc.SelectSingleNode("bCWSinaling").InnerText = this.apcConfig.bCWSinaling.ToString();
        }
        private void UpdateMemoryConfigAGC(XmlDocument doc)
        {
            XmlNode agc = doc.SelectSingleNode("/config/agcConfig");
            XmlNode bandCalEnable = agc.SelectSingleNode("bandCalEnable");
            List<bool> bandCalEnableF = new List<bool>();
            foreach (XmlNode node in bandCalEnable.ChildNodes)
            {
                bandCalEnableF.Add(Convert.ToBoolean(node.InnerText));
            }
            this.agcConfig.bandCalEnable = bandCalEnableF.ToArray();
            //-----------------------------------------------------------
            XmlNode centerChannel = agc.SelectSingleNode("centerChannel");
            List<int> centerChannelF = new List<int>();
            foreach (XmlNode node in centerChannel.ChildNodes)
            {
                centerChannelF.Add(Convert.ToInt32(node.InnerText));
            }
            this.agcConfig.centerChannel = centerChannelF.ToArray();
            //-----------------------------------------------------------
            XmlNode compChannelHigh = agc.SelectSingleNode("compChannelHigh");
            List<int> compChannelHighF = new List<int>();
            foreach (XmlNode node in compChannelHigh.ChildNodes)
            {
                compChannelHighF.Add(Convert.ToInt32(node.InnerText));
            }
            this.agcConfig.compChannelHigh = compChannelHighF.ToArray();
            //-----------------------------------------------------------
            XmlNode compChannelLow = agc.SelectSingleNode("compChannelLow");
            List<int> compChannelLowF = new List<int>();
            foreach (XmlNode node in compChannelLow.ChildNodes)
            {
                compChannelLowF.Add(Convert.ToInt32(node.InnerText));
            }
            this.agcConfig.compChannelLow = compChannelLowF.ToArray();
            //-----------------------------------------------------------
            XmlNode agcPower = agc.SelectSingleNode("agcPower");
            List<double> agcPowerF = new List<double>();
            foreach (XmlNode node in agcPower.ChildNodes)
            {
                agcPowerF.Add(Convert.ToDouble(node.InnerText));
            }
            this.agcConfig.agcPower = agcPowerF.ToArray();
            //-----------------------------------------------------------
            this.agcConfig.freq = Convert.ToDouble(agc.SelectSingleNode("freq").InnerText);
            //-----------------------------------------------------------
            this.agcConfig.rxGainMode = Convert.ToDouble(agc.SelectSingleNode("rxGainMode").InnerText);
            //-----------------------------------------------------------
            this.agcConfig.cellPower = Convert.ToDouble(agc.SelectSingleNode("cellPower").InnerText);
            //-----------------------------------------------------------
            XmlNode centerFreqPathLossDL = agc.SelectSingleNode("centerFreqPathLossDL");
            List<double> centerFreqPathLossDLF = new List<double>();
            foreach (XmlNode node in centerFreqPathLossDL.ChildNodes)
            {
                centerFreqPathLossDLF.Add(Convert.ToDouble(node.InnerText));
            }
            this.agcConfig.centerFreqPathLossDL = centerFreqPathLossDLF.ToArray();
            //-----------------------------------------------------------
            XmlNode compFreqPathLossHighDL = agc.SelectSingleNode("compFreqPathLossHighDL");
            List<double> compFreqPathLossHighDLF = new List<double>();
            foreach (XmlNode node in compFreqPathLossHighDL.ChildNodes)
            {
                compFreqPathLossHighDLF.Add(Convert.ToDouble(node.InnerText));
            }
            this.agcConfig.compFreqPathLossHighDL = compFreqPathLossHighDLF.ToArray();
            //-----------------------------------------------------------
            XmlNode compFreqPathLossLowDL = agc.SelectSingleNode("compFreqPathLossLowDL");
            List<double> compFreqPathLossLowDLF = new List<double>();
            foreach (XmlNode node in compFreqPathLossLowDL.ChildNodes)
            {
                compFreqPathLossLowDLF.Add(Convert.ToDouble(node.InnerText));
            }
            this.agcConfig.compFreqPathLossLowDL = compFreqPathLossLowDLF.ToArray();
        }
        private void UpdateDiskConfigAGC(XmlDocument doc)
        {
            XmlNode agc = doc.SelectSingleNode("/config/agcConfig");

            for (int i = 0; i < agc.SelectSingleNode("bandCalEnable").ChildNodes.Count; i++)
            {
                agc.SelectSingleNode("bandCalEnable").ChildNodes[i].InnerText = this.agcConfig.bandCalEnable[i].ToString();
            }
            for (int i = 0; i < agc.SelectSingleNode("centerChannel").ChildNodes.Count; i++)
            {
                agc.SelectSingleNode("centerChannel").ChildNodes[i].InnerText = this.agcConfig.centerChannel[i].ToString();
            }
            for (int i = 0; i < agc.SelectSingleNode("compChannelHigh").ChildNodes.Count; i++)
            {
                agc.SelectSingleNode("compChannelHigh").ChildNodes[i].InnerText = this.agcConfig.compChannelHigh[i].ToString();
            }
            for (int i = 0; i < agc.SelectSingleNode("compChannelLow").ChildNodes.Count; i++)
            {
                agc.SelectSingleNode("compChannelLow").ChildNodes[i].InnerText = this.agcConfig.compChannelLow[i].ToString();
            }
            for (int i = 0; i < agc.SelectSingleNode("agcPower").ChildNodes.Count; i++)
            {
                agc.SelectSingleNode("agcPower").ChildNodes[i].InnerText = this.agcConfig.agcPower[i].ToString("F2");
            }
            agc.SelectSingleNode("freq").InnerText = this.agcConfig.freq.ToString("F2");
            agc.SelectSingleNode("rxGainMode").InnerText = this.agcConfig.rxGainMode.ToString("F2");
            agc.SelectSingleNode("cellPower").InnerText = this.agcConfig.cellPower.ToString("F2");
            for (int i = 0; i < agc.SelectSingleNode("centerFreqPathLossDL").ChildNodes.Count; i++)
            {
                agc.SelectSingleNode("centerFreqPathLossDL").ChildNodes[i].InnerText = this.agcConfig.centerFreqPathLossDL[i].ToString("F2");
            }
            for (int i = 0; i < agc.SelectSingleNode("compFreqPathLossHighDL").ChildNodes.Count; i++)
            {
                agc.SelectSingleNode("compFreqPathLossHighDL").ChildNodes[i].InnerText = this.agcConfig.compFreqPathLossHighDL[i].ToString("F2");
            }
            for (int i = 0; i < agc.SelectSingleNode("compFreqPathLossLowDL").ChildNodes.Count; i++)
            {
                agc.SelectSingleNode("compFreqPathLossLowDL").ChildNodes[i].InnerText = this.agcConfig.compFreqPathLossLowDL[i].ToString("F2");
            }
        }
        

        
    }
}

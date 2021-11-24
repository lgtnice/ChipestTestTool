using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using GeneralTestPlatform.UI;
using System.Runtime.InteropServices;
using System.Drawing;

namespace GeneralTestPlatform
{
    public static class GlobalConstants
    {
        public static string ResourceDir = Path.Combine(System.AppDomain.CurrentDomain.BaseDirectory, "Resource");
        public static string ConfigDir = Path.Combine(System.AppDomain.CurrentDomain.BaseDirectory, "Config");
        public static string LogDir = Path.Combine(System.AppDomain.CurrentDomain.BaseDirectory, "Log");

        //Treeview智能勾选实现
        public static void UpdateChildNodes(TreeNode node)
        {
            if (node.Nodes.Count > 0)
            {
                foreach (TreeNode child in node.Nodes)
                {
                    child.Checked = node.Checked;
                    UpdateChildNodes(child);
                }
            }
        }
        public static void UpdateParentNodes(TreeNode node)
        {
            if (node.Parent != null)
            {
                TreeNodeState initState = node.Checked ? TreeNodeState.CHECKED : TreeNodeState.EMPTY;
                GetNodeState(ref initState, node.Parent);
                if (initState == TreeNodeState.EMPTY)
                {
                    node.Parent.Checked = false;
                    UpdateParentNodes(node.Parent);
                }
                else if (initState == TreeNodeState.CHECKED)
                {
                    node.Parent.Checked = true;
                    UpdateParentNodes(node.Parent);
                }
                else
                {
                    do
                    {
                        node = node.Parent;
                        //这里写三态中的半选态的处理代码，其余部分可以不做任何修改!
                        node.Checked = true;
                    } while (node.Parent != null);
                }
            }
        }
        private static void GetNodeState(ref TreeNodeState state, TreeNode node)
        {
            if (state == TreeNodeState.HALFCHECKED) { return; }
            if (node.Nodes.Count > 0)
            {
                foreach (TreeNode child in node.Nodes)
                {
                    if (child.Checked == true)
                    {
                        if (state == TreeNodeState.EMPTY) { state = TreeNodeState.HALFCHECKED; return; }
                    }
                    else
                    {
                        if (state == TreeNodeState.CHECKED) { state = TreeNodeState.HALFCHECKED; return; }
                    }
                    GetNodeState(ref state, child);
                }
            }
        }
        //初始化窗体内每个控件的tag
        public static void setTag(Control cons)
        {
            foreach (Control con in cons.Controls)
            {
                con.Tag = con.Width + ":" + con.Height + ":" + con.Left + ":" + con.Top + ":" + con.Font.Size;
                if (con.Controls.Count > 0) { setTag(con); }
            }
        }
        //保证子控件随父控件大小变化而变化
        public static void setControls(double newx, double newy, Control cons)
        {
            foreach (Control con in cons.Controls)
            {
                if (con is Form) { continue; }
                string[] mytag = con.Tag.ToString().Split(new char[] { ':' });
                double a = Convert.ToDouble(mytag[0]) * newx;
                con.Width = (int)a;
                a = Convert.ToDouble(mytag[1]) * newy;
                con.Height = (int)(a);
                a = Convert.ToDouble(mytag[2]) * newx;
                con.Left = (int)(a);
                a = Convert.ToDouble(mytag[3]) * newy;
                con.Top = (int)(a);
                double currentSize = Convert.ToDouble(mytag[4]) * Math.Min(newx, newy);
                con.Font = new Font(con.Font.Name, Convert.ToSingle(currentSize), con.Font.Style, con.Font.Unit);
                if (con.Controls.Count > 0) { setControls(newx, newy, con); }
            }
        }

    }

    public enum ButtonState
    {
        UNDO = 0,
        DOING = 1
    }
    public enum PictureState
    {
        NONSTART = 0,
        DOING = 1,
        SUCCESS = 2,
        FAIL = 3
    }
    public enum TreeNodeState
    {
        EMPTY = 0,
        HALFCHECKED = 1,
        CHECKED = 2
    }
    //连接的配置参数
    public struct LinkConf
    {
        public string m_serialNum;
        public string m_IP;
        public int m_baudRate;
        public LinkConf(string serialNum, string IP, int baudRate)
        {
            this.m_serialNum = serialNum;
            this.m_IP = IP;
            this.m_baudRate = baudRate;
        }
    }
    //AFC配置参数结构体
    [System.Runtime.InteropServices.StructLayoutAttribute(System.Runtime.InteropServices.LayoutKind.Sequential,Pack =8)]
    public struct AFCConf
    {
        [MarshalAs(UnmanagedType.I4)]
        public int apcData;
        [MarshalAs(UnmanagedType.I4)]
        public int paMode;
        [MarshalAs(UnmanagedType.I4)]
        public int afcData1;
        [MarshalAs(UnmanagedType.I4)]
        public int afcData2;
        [MarshalAs(UnmanagedType.I4)]
        public int cycleNum;
        [MarshalAs(UnmanagedType.R8)]
        public double freq;
        [MarshalAs(UnmanagedType.R8)]
        public double expPower;
        [MarshalAs(UnmanagedType.R8)]
        public double pathLoss;
        [System.Runtime.InteropServices.MarshalAsAttribute(System.Runtime.InteropServices.UnmanagedType.I1)]
        public bool bCWSinaling;
    }
    //APC配置参数结构体
    [System.Runtime.InteropServices.StructLayoutAttribute(System.Runtime.InteropServices.LayoutKind.Sequential,Pack =8)]
    public struct APCConf
    {
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 18,ArraySubType =UnmanagedType.I1)]
        public bool [] bandCalEnable ;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 18,ArraySubType =UnmanagedType.I4)]
        public int[] centerChannel;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 18, ArraySubType = UnmanagedType.I4)]
        public int[] compChannelHigh;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 18, ArraySubType = UnmanagedType.I4)]
        public int[] compChannelLow;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3, ArraySubType = UnmanagedType.I4)]
        public int[] apcData1;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3, ArraySubType = UnmanagedType.I4)]
        public int[] apcData2;
        [MarshalAs(UnmanagedType.I4)]
        public int initApcData;
        [MarshalAs(UnmanagedType.I4)]
        public int paMode;
        [MarshalAs(UnmanagedType.R8)]
        public double freq;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 18, ArraySubType = UnmanagedType.R8)]
        public double[] centerFreqPathLossUL;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 18, ArraySubType = UnmanagedType.R8)]
        public double[] compFreqPathLossHighUL;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 18, ArraySubType = UnmanagedType.R8)]
        public double[] compFreqPathLossLowUL;
        [System.Runtime.InteropServices.MarshalAsAttribute(System.Runtime.InteropServices.UnmanagedType.I1)]
        public bool bCWSinaling;
    }
    //AGC配置参数结构体
    [System.Runtime.InteropServices.StructLayoutAttribute(System.Runtime.InteropServices.LayoutKind.Sequential, Pack = 8)]
    public struct AGCConf
    {
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 18, ArraySubType = UnmanagedType.I1)]
        public bool[] bandCalEnable;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 18, ArraySubType = UnmanagedType.I4)]
        public int[] centerChannel;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 18, ArraySubType = UnmanagedType.I4)]
        public int[] compChannelHigh;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 18, ArraySubType = UnmanagedType.I4)]
        public int[] compChannelLow;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3, ArraySubType = UnmanagedType.R8)]
        public double[] agcPower;
        [MarshalAs(UnmanagedType.R8)]
        public double freq;
        [MarshalAs(UnmanagedType.R8)]
        public double rxGainMode;
        [MarshalAs(UnmanagedType.R8)]
        public double cellPower;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 18, ArraySubType = UnmanagedType.R8)]
        public double[] centerFreqPathLossDL;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 18, ArraySubType = UnmanagedType.R8)]
        public double[] compFreqPathLossHighDL;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 18, ArraySubType = UnmanagedType.R8)]
        public double[] compFreqPathLossLowDL;
    }


}

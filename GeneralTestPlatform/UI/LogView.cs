using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Runtime.InteropServices;

//导入托管代码的命名空间
using ColloCalNamespace;
using ColloTestNamespace;

namespace GeneralTestPlatform.UI
{
    public partial class LogView : Form
    {
        //存储一开始窗体的长和宽
        private double m_X;
        private double m_Y;
        private Thread m_thread;
        private Thread m_threadLog;
        //存放所有配置参数结构体
        private LinkConf m_linkConf;
        private frmGUI m_frm;
        private LogBuffer m_logbuf;

        public LogView(LinkConf st,frmGUI frm)
        {
            InitializeComponent();
            this.m_linkConf = st;
            //初始化私有字段
            m_X = this.Width;
            m_Y = this.Height;
            this.Text = "串口号：" + st.m_serialNum + "  " + "IP地址：" + st.m_IP;
            //初始化窗体内每个控件的tag
            GlobalConstants.setTag(this);
            updatePictureState(PictureState.NONSTART);
            updateButtonState(ButtonState.UNDO);
            this.m_frm = frm;
            m_logbuf = new LogBuffer(this,false);
        }
        ~LogView()
        {
            if ((m_thread.ThreadState != System.Threading.ThreadState.Stopped) && (m_thread.ThreadState != System.Threading.ThreadState.Aborted))
            {
                m_thread.Abort();
            }
            m_logbuf.SetFlag(true);
            while ((m_threadLog.ThreadState != System.Threading.ThreadState.Stopped) && (m_threadLog.ThreadState != System.Threading.ThreadState.Aborted))
            {
                Thread.Sleep(10);
            }
        }
        private void LogView_Resize(object sender, EventArgs e)
        {
            double newx = (this.Width) / m_X;
            double newy = this.Height / m_Y;
            GlobalConstants.setControls(newx, newy, this);
        }
        private void LogView_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (m_thread != null)
            {
                if ((m_thread.ThreadState != System.Threading.ThreadState.Stopped) && (m_thread.ThreadState != System.Threading.ThreadState.Aborted))
                {
                    m_thread.Abort();
                }
            }
            if (m_threadLog != null)
            {
                m_logbuf.SetFlag(true);
                while ((m_threadLog.ThreadState != System.Threading.ThreadState.Stopped) && (m_threadLog.ThreadState != System.Threading.ThreadState.Aborted))
                {
                    Thread.Sleep(10);
                }
            }
        }
        private void button_start_Click(object sender, EventArgs e)
        {
            //启动打日志线程
            m_threadLog = new Thread(m_logbuf.ThreadActionLog);
            m_threadLog.IsBackground = true;
            m_threadLog.Start();

            //启动测试线程
            m_thread = new Thread(ThreadAction);
            m_thread.IsBackground = true;
            m_thread.Start();
        }
        private void button_stop_Click(object sender, EventArgs e)
        {
            if ((m_thread.ThreadState != System.Threading.ThreadState.Stopped) && (m_thread.ThreadState != System.Threading.ThreadState.Aborted))
            {
                m_thread.Abort();
            }
            m_logbuf.SetFlag(true);
            while ((m_threadLog.ThreadState != System.Threading.ThreadState.Stopped) && (m_threadLog.ThreadState != System.Threading.ThreadState.Aborted))
            {
                Thread.Sleep(10);
            }
            m_logbuf.m_buffer.Clear();
            updatePictureState(PictureState.NONSTART);
            updateButtonState(ButtonState.UNDO);
        }


        //线程执行代码
        private void ThreadAction()
        {
            updatePictureState(PictureState.DOING);
            updateButtonState(ButtonState.DOING);


            int result = 0;
            if (result == 0) { result = calProcess(); } 


            
            m_logbuf.SetFlag(true);
            while ((m_threadLog.ThreadState != System.Threading.ThreadState.Stopped) && (m_threadLog.ThreadState != System.Threading.ThreadState.Aborted))
            {
                Thread.Sleep(10);
            }
            updatePictureState((result == 0) ? PictureState.SUCCESS : PictureState.FAIL);
            updateButtonState(ButtonState.UNDO);
            if ((m_thread.ThreadState != System.Threading.ThreadState.Stopped) && (m_thread.ThreadState != System.Threading.ThreadState.Aborted))
            {
                m_thread.Abort();
            }
        }
        //更新picturebox的图片内容
        private void updatePictureState( PictureState state)
        {
            string path = string.Empty;
            switch (state)
            {
                case PictureState.DOING: { path = GlobalConstants.ResourceDir + "\\Running.jpg"; break; }
                case PictureState.FAIL: { path = GlobalConstants.ResourceDir + "\\Fail.jpg"; break; }
                case PictureState.NONSTART: { path = GlobalConstants.ResourceDir + "\\Ready.jpg"; break; }
                case PictureState.SUCCESS: { path = GlobalConstants.ResourceDir + "\\Pass.jpg"; break; }
                default: break;
            }
            if (this.InvokeRequired)
            {
                Invoke(new EventHandler(delegate { pictureBox1.Load(path); }));
            }
            else
            {
                pictureBox1.Load(path);
            }
        }
        //更新button的unable属性
        private void updateButtonState(ButtonState state)
        {
            bool tag = (state == ButtonState.DOING);
            if (this.InvokeRequired)
            {
                Invoke(new EventHandler(delegate { button_start.Enabled = !tag; this.button_stop.Enabled = tag; }));
            }
            else
            {
                button_start.Enabled = !tag;
                this.button_stop.Enabled = tag;
            }
        }
        //校准流程
        private int calProcess()
        {
            ColloCal calTestProc = new ColloCal();
            ColloCalNamespace.PrintLogDelegate func = new ColloCalNamespace.PrintLogDelegate(m_logbuf.AddLog);

            int result = 0;
            if (result == 0) { result = calTestProc.SetLogPtr(func); } else { return result; }
            if (result == 0) { result = calTestProc.InitDUTAPI(); } else { return result; }
            if (result == 0) { result = calTestProc.InitInstrumentAPI(); } else { return result; }
            if (result == 0) { result = calTestProc.OpenSerialPortEX(m_linkConf.m_baudRate, m_linkConf.m_serialNum); } else { return result; }
            if (result == 0) { result = calTestProc.OpenInstrument(m_linkConf.m_IP); } else { return result; }


            TreeNode node = m_frm.treeView_cal.Nodes[0];
            TreeNode[] d = node.TreeView.Nodes.Find("Element_AFC", true);
            unsafe
            {
                AFCConf config = CalConfig.getInstance().afcConfig;
                IntPtr p = Marshal.AllocHGlobal(Marshal.SizeOf(config));
                Marshal.StructureToPtr(config, p, true);
                if (d[0].Checked)
                {
                    if (result == 0) { result = calTestProc.AFCCal(p); } else { return result; }
                }
                Marshal.FreeHGlobal(p);
            }
            d = node.TreeView.Nodes.Find("Element_APC", true);
            unsafe
            {
                APCConf config = CalConfig.getInstance().apcConfig;
                IntPtr p = Marshal.AllocHGlobal(Marshal.SizeOf(config));
                Marshal.StructureToPtr(config, p, true);
                if (d[0].Checked)
                {
                    if (result == 0) { result = calTestProc.APCCal(p); } else { return result; }
                }
                Marshal.FreeHGlobal(p);
            }
            d = node.TreeView.Nodes.Find("Element_AGC", true);
            unsafe
            {
                AGCConf config = CalConfig.getInstance().agcConfig;
                IntPtr p = Marshal.AllocHGlobal(Marshal.SizeOf(config));
                Marshal.StructureToPtr(config, p, true);
                if (d[0].Checked)
                {
                    if (result == 0) { result = calTestProc.AGCCal(p); } else { return result; }
                }
                Marshal.FreeHGlobal(p);
            }


            if (result == 0) { result = calTestProc.CloseSerialPortEX(); } else { return result; }
            if (result == 0) { result = calTestProc.ReleaseInstrument(); } else { return result; }
            if (result == 0) { result = calTestProc.ReleaseDUTAPI(); } else { return result; }
            if (result == 0) { result = calTestProc.ReleaseInstrumentAPI(); } else { return result; }

            return result;
        }

        
    }
}

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace GeneralTestPlatform.UI
{
    public partial class LinkConfig : Form
    {
        //存储一开始窗体的长和宽
        private double m_X;
        private double m_Y;

        public LinkConfig()
        {
            InitializeComponent();
            //初始化私有字段
            m_X = this.Width;
            m_Y = this.Height;

            //初始化窗体内每个控件的tag
            GlobalConstants.setTag(this);
        }
        private void LinkConfig_Resize(object sender, EventArgs e)
        {
            double newx = (this.Width) / m_X;
            double newy = this.Height / m_Y;
            GlobalConstants.setControls(newx, newy, this);
        }
        private void button1_Click(object sender, EventArgs e)
        {
            frmGUI parentFrm = this.Owner as frmGUI;

            int baudRate = 0;
            int result;
            if (int.TryParse(comboBox2.Text,out result)) { baudRate = int.Parse(comboBox2.Text); }
            LinkConf st = new LinkConf(comboBox1.Text, textBox1.Text, baudRate);

            LogView testFrm = new LogView(st, parentFrm);
            testFrm.TopLevel = false;
            testFrm.Show();
            lock (parentFrm.flowLayoutPanel_log)
            {
                parentFrm.flowLayoutPanel_log.Controls.Add(testFrm);
            }
            Dispose();
        }
        private void button2_Click(object sender, EventArgs e)
        {
            Dispose();
        }

    }
}

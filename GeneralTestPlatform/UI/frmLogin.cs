using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Xml.Linq;
using System.Text.RegularExpressions;

namespace GeneralTestPlatform.UI
{
    public partial class frmLogin : Form
    {
        //是否按下取消按钮
        public bool bCancelClicked = false;
        //是否已经登录成功
        public bool bLogin = false;

        //存放以哪个ID登录程序
        public static string userId = "";
        //存放是否以管理员身份登录
        public static bool bAdmin = false;

        //从配置文件读取到的用户列表
        List<Uesrs> users = new List<Uesrs>();
        //引用加密解密方法
        CryptXml crypXml = new CryptXml();

        //存储一开始窗体的长和宽
        private double m_X;
        private double m_Y;

        public frmLogin()
        {
            InitializeComponent();
            UpdateContentFromXml();
            this.CenterToScreen();
            this.ResumeLayout(false);
            this.PerformLayout();
            this.Icon = new Icon(GlobalConstants.ResourceDir + "\\iot.ico");
            //初始化私有字段
            m_X = this.Width;
            m_Y = this.Height;

            //初始化窗体内每个控件的tag
            GlobalConstants.setTag(this);
            this.WindowState = FormWindowState.Maximized;
        }

        public void UpdateContentFromXml()
        {
            string xmlPath = "userInfo.xml";
            if (System.IO.File.Exists(xmlPath))
            {
                crypXml.fileDcryption(xmlPath);
                XDocument xDoc = XDocument.Load(xmlPath);
                XElement rootNode = xDoc.Element("CONFIG");
                int num = System.Convert.ToInt32(rootNode.Element("UserNum").Value);
                for (int i = 0; i < num; i++)
                {
                    Uesrs temp = new Uesrs();
                    string subNode = string.Format("User{0}", i);
                    XElement node = rootNode.Element(subNode);
                    temp.LoginName = (node.Element("UserName").Value);
                    temp.LoingPassword = (node.Element("Password").Value);
                    users.Add(temp);
                }
                crypXml.fileEncryption(xmlPath);
            }
            else
            {
                MessageBox.Show("未导入用户文件,将只能已管理员权限登陆", "TIPS");
            }

        }

        protected override bool ProcessCmdKey(ref Message msg, Keys keyData)
        {
            // 实现按Enter键来执行按钮的点击事件方法
            if (keyData == Keys.Enter)
            {
                btnOK.PerformClick();
                return true;
            }
            
            return base.ProcessCmdKey(ref msg, keyData); // 其他键按默认处理　

        }

        private void btnOK_Click(object sender, EventArgs e)
        {
            Login();
        }

        private void Login()
        {
            string inputName = tbxUserName.Text ;
            if (tbxUserName.Text.ToLower().Equals("admin"))
            {
                inputName = "Admin";
            }
            string inputPassword = tbxPassword.Text;
            
            int i = 0;
            for (; i < users.Count; i++)
            {
                if (inputName.Equals(users[i].LoginName))
                {
                    if (inputPassword.Equals(users[i].LoingPassword))
                    {
                        bLogin = true;
                        this.DialogResult = DialogResult.OK;
                        userId = tbxUserName.Text;
                        Dispose();
                        break;
                    }
                    else
                    {
                        MessageBox.Show("密码错误");
                        return;
                    }
                }
            }
            if (i == users.Count)
            {
                MessageBox.Show("未找到此用户名");
            }
            if (bLogin && inputName.Equals("Admin"))
            {
                bAdmin = true;
            }
        }

        private void btnCancel_Click(object sender, EventArgs e)
        {
            bCancelClicked = true;
            Dispose();
        }

        private void frmLogin_Resize(object sender, EventArgs e)
        {
            double newx = (this.Width) / m_X;
            double newy = this.Height / m_Y;
            GlobalConstants.setControls(newx, newy, this);
        }
    }

    public class Uesrs
    {
        string _loginName;
        public string LoginName
        {
            get { return _loginName; }
            set { _loginName = value; }
        }
        string _loingPassword;
        public string LoingPassword
        {
            get { return _loingPassword; }
            set { _loingPassword = value; }
        }
    }
}

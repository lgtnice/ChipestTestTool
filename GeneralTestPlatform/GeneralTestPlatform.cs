using System;
using System.Windows.Forms;
using GeneralTestPlatform.UI;
using System.Threading;

namespace GeneralTestPlatform
{
    public static class GeneralTestPlatform
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            //这里加载配置文件xml至内存



            frmLogin login = new frmLogin();
            Application.Run(login);
            
            //frmlogin的dispose执行之后
            if (login.bLogin)
            {
                Application.Run(new frmGUI());
            }
            else
            {
                Application.Exit();
            }
            //这里把内存里的xml存入磁盘


        }
    }
}

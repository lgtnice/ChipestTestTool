using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using GeneralTestPlatform.UI;
using System.Windows.Forms;

namespace GeneralTestPlatform
{
    public class LogBuffer 
    {
        public Queue<String> m_buffer;
        private LogView m_parentForm;
        private bool m_ThreadStopFlag;
        static ReaderWriterLockSlim LogWriteLock = new ReaderWriterLockSlim();

        public LogBuffer(LogView frm,bool flag)
        {
            m_buffer = new Queue<String>();
            m_parentForm = frm;
            m_ThreadStopFlag = flag;
        }

        public void AddLog(String data)
        {
            lock (m_buffer)
            {
                m_buffer.Enqueue(data);
            }
        }
        public void SetFlag(bool flag){ m_ThreadStopFlag = flag; }
        //打log方法
        public void ThreadActionLog()
        {
            while (true)
            {
                if ((m_ThreadStopFlag == true) && (m_buffer.Count == 0)) { m_ThreadStopFlag = false; return; }
                if (this.m_buffer.Count > 0)
                {
                    
                    while (this.m_buffer.Count > 0)
                    {
                        string tempString = this.m_buffer.Dequeue();
                        if (m_parentForm.richTextBox1.InvokeRequired)
                        {
                            m_parentForm.Invoke(new EventHandler(delegate { this.m_parentForm.richTextBox1.AppendText(tempString + "\n"); }));
                        }
                        else
                        {
                            m_parentForm.richTextBox1.AppendText(tempString);
                        }
                        OutputToFile(tempString);
                    }
                }
                Thread.Sleep(10);
            }
        }
        private static void OutputToFile(string log)
        {
            try
            {
                LogWriteLock.EnterWriteLock();
                if (!Directory.Exists(GlobalConstants.LogDir)) { Directory.CreateDirectory(GlobalConstants.LogDir); }
                string path = GlobalConstants.LogDir + "\\" + DateTime.Now.ToString("yyyyMMdd--hh") + ".txt";
                if (!File.Exists(path))
                {
                    FileStream fs = File.Create(path);
                    fs.Close();
                    fs.Dispose();
                }

                //FileStream fs = new FileStream(path,FileMode.Append,FileAccess.Write,FileShare.None);
                //StreamWriter sw = new StreamWriter(fs,Encoding.Unicode);
                //sw.WriteLine(tempString);
                //sw.Flush();
                //sw.Close();
                //fs.Close();
                //fs.Dispose();
            }
            catch (Exception)
            {

                throw;
            }
            finally
            {
                LogWriteLock.ExitWriteLock();
            }
        }



    }
}

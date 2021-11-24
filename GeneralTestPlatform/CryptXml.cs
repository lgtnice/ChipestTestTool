using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml; //XML Namespace
using System.Security.Cryptography;
using System.Security.Cryptography.Xml;
//using System.Security.Cryptography.Xml;//xml encryption namespace
using System.Windows.Forms; //Application namespace

namespace GeneralTestPlatform
{
    public class CryptXml
    {
        private void Encrypt(XmlDocument Doc, string ElementName, SymmetricAlgorithm Key)
        {
            XmlElement elementToEncrypt = Doc.GetElementsByTagName(ElementName)[0] as XmlElement;
            EncryptedXml eXml = new EncryptedXml();
            byte[] encryptedElement = eXml.EncryptData(elementToEncrypt, Key, false);
            EncryptedData edElement = new EncryptedData();
            edElement.Type = EncryptedXml.XmlEncElementUrl;
            string encryptionMethod = null;

            if (Key is TripleDES)
            {
                encryptionMethod = EncryptedXml.XmlEncTripleDESUrl;
            }
            else if (Key is DES)
            {
                encryptionMethod = EncryptedXml.XmlEncDESUrl;
            }
            if (Key is Rijndael)
            {
                switch (Key.KeySize)
                {
                    case 128:
                        encryptionMethod = EncryptedXml.XmlEncAES128Url;
                        break;
                    case 192:
                        encryptionMethod = EncryptedXml.XmlEncAES192Url;
                        break;
                    case 256:
                        encryptionMethod = EncryptedXml.XmlEncAES256Url;
                        break;
                }
            }
            edElement.EncryptionMethod = new EncryptionMethod(encryptionMethod);
            edElement.CipherData.CipherValue = encryptedElement;
            EncryptedXml.ReplaceElement(elementToEncrypt, edElement, false);
        }

        //xml解密方法
        private void Decrypt(XmlDocument Doc, SymmetricAlgorithm Alg)
        {
            XmlElement encryptedElement = Doc.GetElementsByTagName("EncryptedData")[0] as XmlElement;
            EncryptedData edElement = new EncryptedData();
            edElement.LoadXml(encryptedElement);
            EncryptedXml exml = new EncryptedXml();
            byte[] rgbOutput = exml.DecryptData(edElement, Alg);
            exml.ReplaceData(encryptedElement, rgbOutput);
        }

        //对相应xml文件 加密
        public void fileEncryption(string filepath)
        {
            if (!System.IO.File.Exists(filepath))
            {
                return;
            }
            RijndaelManaged key = new RijndaelManaged();
            //设置密钥:key为32位=数字或字母16个=汉字8个
            byte[] byteKey = Encoding.Unicode.GetBytes("1111111111111111");//every load get time as key
            key.Key = byteKey;
            XmlDocument xmlDoc = new XmlDocument();
            xmlDoc.PreserveWhitespace = true;
            xmlDoc.Load(filepath);//加载要加密的XML文件
            Encrypt(xmlDoc, "CONFIG", key);//需要加密的节点
            if (key != null)
            {
                key.Clear();
            }
            xmlDoc.Save(filepath);//生成加密后的XML文件
            //MessageBox.Show("OK");
        }

        //对相应xml文件 解密
        public void fileDcryption(string filepath)
        {
            if (!System.IO.File.Exists(filepath))
            {
                return;
            }
                RijndaelManaged key = new RijndaelManaged();
            //设置密钥:key为32位=数字或字母16个=汉字8个
            byte[] byteKey = Encoding.Unicode.GetBytes("1111111111111111");
            key.Key = byteKey;
            XmlDocument xmlDoc = new XmlDocument();
            xmlDoc.PreserveWhitespace = true;
            xmlDoc.Load(filepath);//加载要解密的XML文件
            Decrypt(xmlDoc, key);
            if (key != null)
            {
                key.Clear();
            }
            xmlDoc.Save(filepath);//生成解密后的XML文件
            // MessageBox.Show("OK");
        }
    }
}

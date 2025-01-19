using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace uclabhazi
{
    public partial class Form1 : Form
    {      
        private int tiltx = 0;
        private int tilty = 0;
        private bool setbrigthness = false;
        private byte brightness = 0;
        private bool textvalid = true;
        private string prevtext;
        public Form1()
        {
            InitializeComponent();
            timer1.Interval =300;
            timer2.Interval = 50;
            timer1.Start();
            timer2.Start();

            textBox1.Text = "50";

            if(!serialPort1.IsOpen)serialPort1.Open();
            serialPort1.DtrEnable = true;
            serialPort1.DiscardInBuffer();

 


        }
        private void port_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            Console.WriteLine(serialPort1.ReadExisting());
        }

        private void portRequestData()
        {
            byte[] buffer = new byte[3];
            buffer[0] = 0xAA;
            buffer[1] = 0x01;
            buffer[2] = 0x02;
            serialPort1.Write(buffer, 0, 3);
        }
        private void portSetBrightness(byte n)
        {
            

            byte[] buffer = new byte[4];
            System.Threading.Thread.Sleep(1);
            buffer[0] = 0xAA;
            buffer[1] = 0x02;
            buffer[2] = 0x01;
            buffer[3] = n;
            if(!serialPort1.IsOpen) serialPort1.Open();

            serialPort1.Write(buffer, 0, 1);
            System.Threading.Thread.Sleep(1);
            serialPort1.Write(buffer, 1, 1);
            System.Threading.Thread.Sleep(1);
            serialPort1.Write(buffer, 2, 1);
            System.Threading.Thread.Sleep(1);
            serialPort1.Write(buffer, 3, 1);
            System.Threading.Thread.Sleep(1);



        }

        private void button1_Click(object sender, EventArgs e)
        {
            setbrigthness = true;
            brightness = byte.Parse(textBox1.Text);
            
            }

        private void timer1_Tick(object sender, EventArgs e)
        {
            Invalidate();
        }

        private void Form1_Paint(object sender, PaintEventArgs e)
        {
            //e.Graphics.DrawRectangle(System.Drawing.Pens.Blue, 10, ClientSize.Height / 2, ClientSize.Width-20, ClientSize.Height/2-10);
            //Point center = new Point(ClientSize.Width/2, ClientSize.Height/2);
            //Point bubble = new Point(center.X+(tiltx*(ClientSize.Width/2)) / 30, (center.Y + (tilty * (ClientSize.Height / 2) / 30)));
            //e.Graphics.DrawEllipse(System.Drawing.Pens.Blue, bubble.X, bubble.Y, 40, 40);
        }

        private void serialPort1_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {

            if (serialPort1.BytesToRead!=0)
            {
                byte[] buffer = new byte[2];
                serialPort1.Read(buffer, 0, 2);
                if (buffer[0] != 0) tilty = buffer[0] - 90;
                if (buffer[1] != 0) tiltx = buffer[1] - 90;
            }
            panel2.Invalidate();
   
        }

        private void timer2_Tick(object sender, EventArgs e)
        {
            //serialPort1.Open();
        }

        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {
            if (textvalid)
            {
                if (textBox1.Text.Length > 0 && textBox1.Text.Length < 3)
                {
                    brightness = byte.Parse(textBox1.Text);
                    portSetBrightness(byte.Parse(textBox1.Text));
                }
                else if (textBox1.Text.Length >= 3)
                {
                    brightness = 100;
                    portSetBrightness(100);
                    textBox1.Text = "100";
                }
                else
                {
                    portSetBrightness(0);
                    brightness = 0;
                }
                prevtext = textBox1.Text;
            }
            else textBox1.Text = prevtext;
            progressBar1.Value = brightness;
        }

        private void textBox1_PreviewKeyDown(object sender, PreviewKeyDownEventArgs e)
        {
            if ((e.KeyValue < '0' || e.KeyValue > '9') && e.KeyValue!= 8 && e.KeyValue != 46)
            {
                textvalid = false;
            }
            else textvalid = true;
        }

        private void splitContainer1_Panel1_Paint(object sender, PaintEventArgs e)
        {

        }

        private void panel2_Paint(object sender, PaintEventArgs e)
        {
            e.Graphics.DrawLine(System.Drawing.Pens.Blue, 10, panel2.Height/2, panel2.Width-10, panel2.Height/2);
            e.Graphics.DrawLine(System.Drawing.Pens.Blue, panel2.Width / 2, panel2.Height - 10, panel2.Width / 2, 10);
            e.Graphics.DrawRectangle(System.Drawing.Pens.Blue, 10, 10, panel2.Width - 20, panel2.Height - 20);
            Point center = new Point(panel2.Width / 2, panel2.Height / 2);
            Point bubble = new Point(center.X + (tiltx * (panel2.Width / 2)) / 30, (center.Y + (tilty * (panel2.Height / 2) / 30)));
            if(bubble.X > panel2.Width-30) bubble.X = panel2.Width-30;
            if(bubble.X < 30) bubble.X = 30;
            if(bubble.Y <30 ) bubble.Y = 30;
            if(bubble.Y > panel2.Height-30) bubble.Y= panel2.Height-30;
            e.Graphics.DrawEllipse(System.Drawing.Pens.Blue, bubble.X-20, bubble.Y-20, 40, 40);
        }

        private void panel2_SizeChanged(object sender, EventArgs e)
        {
            panel2.Invalidate();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            if (brightness > 90) brightness = 100;
            else brightness = (byte)(brightness + 10);
            textBox1.Text = brightness.ToString();
        }

        private void button1_Click_1(object sender, EventArgs e)
        {
            if (brightness < 10) brightness = 0;
            else brightness = (byte)(brightness - 10);
            textBox1.Text= brightness.ToString();
        }

        private void label1_Click(object sender, EventArgs e)
        {

        }
    }
}

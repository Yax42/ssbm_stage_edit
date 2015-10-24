using System;
using System.IO;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace BTTx2
{
	public partial class Form1 : Form
	{
		static Dictionary<string, string> FileNames = new Dictionary<string, string>
		{
			{"Bowser","GrTKp.dat"},
			{"Captain Falcon", "GrTCa.dat" },
			{"Donkey Kong", "GrTDk.dat" },
			{"Dr. Mario","GrTDr.dat"},
			{"Falco","GrTFc.dat"},
			{"Fox","GrTFx.dat"},
			{"Ganondorf","GrTGn.dat"},
			{"Ice Climbers","GrTIc.dat"},
			{"Jigglypuff","GrTPr.dat"},
			{"Kirby","GrTKb.dat"},
			{"Link","GrTLk.dat"},
			{"Luigi","GrTLg.dat"},
			{"Mario","GrTMr.dat"},
			{"Marth","GrTMs.dat"},
			{"Mewtwo","GrTMt.dat"},
			{"Mr. Game & Watch","GrTGw.dat"},
			{"Ness","GrTNs.dat"},
			{"Peach","GrTPe.dat"},
			{"Pichu","GrTPc.dat"},
			{"Pikachu","GrTPk.dat"},
			{"Roy","GrTFe.dat"},
			{"Samus","GrTSs.dat"},
			{"Yoshi","GrTYs.dat"},
			{"Young Link", "GrTCl.dat" },
			{"Zelda","GrTZd.dat"},
		};

		private string FileName;
		private string imgPath = "";
		public Form1()
		{
			InitializeComponent();
			this.listBox1.Items.AddRange(FileNames.Keys.ToArray());
			MyEnabled = false;
			if (File.Exists("config.txt"))
			{
				using (TextReader tw = new StreamReader("config.txt"))
				{
					OpenImage(tw.ReadLine());
					tw.Close();
				}

			}
		}

		private bool MyEnabled
		{
			set
			{
				button1.Enabled = value;
				button2.Enabled = value;
				listBox1.Enabled = value;
				numericUpDown1.Enabled = value;
				label2.Enabled = value;
				groupBox1.Enabled = value;
			}
		}
		private Process initProc()
		{
			Process p = new Process();
			p.StartInfo.UseShellExecute = false;
			p.StartInfo.CreateNoWindow = true;
			p.StartInfo.RedirectStandardOutput = true;
			return p;
		}

		private void button1_Click(object sender, EventArgs e)
		{
			MyEnabled = false;
			this.label3.Text = "Export " + FileName + ": ";
			string aa = numericUpDown1.Value.ToString();
			progressBar1.Increment(20);
			Process p = initProc();
			p.StartInfo.FileName = "gcrc";
			p.StartInfo.Arguments = "\"" + imgPath + "\"" + " " + "\"" + FileName + "\"" + " " + "tmp";
			p.Start();
			string Output = p.StandardOutput.ReadToEnd();
			p.Exited += Rescale1;
		}

		private void Rescale1(object sender, EventArgs e)
		{

			if (!string.IsNullOrEmpty(Output))
			{
				this.label3.Text += Output;
				MyEnabled = true;
			}
			else
			{
				this.label3.Text += "ok\nRescale " + FileName + ": ";
				progressBar1.Increment(30);
				Process p = initProc();
				p.StartInfo.FileName = "msr";
				p.StartInfo.Arguments = "tmp tmp2 " + numericUpDown1.Value.ToString().Replace(',', '.');
				p.Start();
				Output = p.StandardOutput.ReadToEnd();
				p.Exited += Rescale2;
			}
		}

		private void Rescale2(object sender, EventArgs e)
		{
			if (!string.IsNullOrEmpty(Output))
			{
				this.label3.Text += Output;
				MyEnabled = true;
			}
			else
			{
				this.label3.Text += "ok\nImport back " + FileName + ": ";
				progressBar1.Increment(30);
				Process p = initProc();
				p.StartInfo.FileName = "gcrc";
				p.StartInfo.Arguments = "-i \"" + imgPath + "\"" + " " + "\"" + FileName + "\"" + " " + "tmp2";
				p.Start();
				Output = p.StandardOutput.ReadToEnd();
				p.Exited += Rescale3;
			}
		}

		private void Rescale3(object sender, EventArgs e)
		{
			if (!string.IsNullOrEmpty(Output))
			{
				this.label3.Text += Output;
			}
			else
			{
				this.label3.Text += "ok";
				progressBar1.Increment(20);
				label3.Text += "\nDone";
				progressBar1.Value = 0;
			}

			MyEnabled = true;
		}

		private void listBox1_SelectedIndexChanged_1(object sender, EventArgs e)
		{
			this.textBox1.Text = "";
			string curItem = listBox1.SelectedItem.ToString();
			FileName = FileNames[curItem];
			MyEnabled = true;
			label3.Text = "";
			this.button2.Text = "Current scale";
		}

		private void miRootOpen_Click(object sender, EventArgs e)
		{

			OpenFileDialog openFileDialog = new OpenFileDialog
			{
				Filter = "GameCube ISO (*.iso)|*.iso|GameCube Image File (*.gcm)|*.gcm|All files (*.*)|*.*",
				Title = "Open image"
			};
			if (this.imgPath != "")
			{
				openFileDialog.FileName = this.imgPath;
			}
			if (openFileDialog.ShowDialog() == DialogResult.OK)
			{
				using (TextWriter tw = new StreamWriter("config.txt"))
				{
					tw.WriteLine(openFileDialog.FileName);
					tw.Close();
				}
				OpenImage(openFileDialog.FileName);
			}
		}

		private void OpenImage(string imagePath)
		{
			if (File.Exists(imagePath))
			{
				label1.Text = System.IO.Path.GetFileName(imagePath);
				this.imgPath = imagePath;
				MyEnabled = true;
				button1.Enabled = false;
				button2.Enabled = false;
			}
			else
			{
				this.label3.Text = imagePath + " doesn't exist.";
			}
		}
		private void numericUpDown1_ValueChanged(object sender, EventArgs e)
		{

		}

		private void progressBar1_Click(object sender, EventArgs e)
		{

		}

		private string Output;
		private void button2_Click(object sender, EventArgs e)
		{
			MyEnabled = false;
			this.label3.Text = "Export " + FileName + ": ";
			string aa = numericUpDown1.Value.ToString();
			progressBar1.Increment(20);
			Process p = initProc();
			p.StartInfo.FileName = "gcrc";
			p.StartInfo.Arguments = "\"" + imgPath + "\"" + " " + "\"" + FileName + "\"" + " " + "tmp";
			p.Start();
			Output = p.StandardOutput.ReadToEnd();
			p.Exited += EventGetScale2;
			p.WaitForExit();
		}

		private void EventGetScale2(object sender, EventArgs e)
		{
			if (!string.IsNullOrEmpty(Output))
			{
				this.label3.Text += Output;
				MyEnabled = true;
			}
			else
			{
				this.label3.Text += "ok\nGet " + FileName + "'s scale: ";
				progressBar1.Increment(30);
				Process p = initProc();
				p.StartInfo.FileName = "msr";
				p.StartInfo.Arguments = "tmp";
				p.Start();
				Output = p.StandardOutput.ReadToEnd();
				p.Exited += EventGetScale3;
			}

		}

		private void EventGetScale3(object sender, EventArgs e)
		{
			this.label3.Text += "ok";
			this.textBox1.Text = Output;
			progressBar1.Value = 0;
			MyEnabled = true;
		}

		private void label2_Click(object sender, EventArgs e)
		{

		}

		private void label1_Click(object sender, EventArgs e)
		{

		}

		private void menuStrip_ItemClicked(object sender, ToolStripItemClickedEventArgs e)
		{

		}
	}
}

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
		private string FilePath { get { return "backup/" + FileName; } }
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
				SetEnabled(value);
			}

		}
		delegate void SetEnabledCallback(bool v);
		private void SetEnabled(bool value)
		{
			if (this.textBox1.InvokeRequired)
			{
				SetEnabledCallback d = new SetEnabledCallback(SetEnabled);
				this.Invoke(d, new object[] { value });
			}
			else
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
			p.StartInfo.RedirectStandardError = true;
			p.EnableRaisingEvents = true;

			return p;
		}

		private void CreateBackup()
		{
			Directory.CreateDirectory("backup");
			this.label3.Text = "Export backup " + FileName + ": ";
			Process p = initProc();
			p.StartInfo.FileName = "gcrc";
			p.StartInfo.Arguments = "\"" + imgPath + "\"" + " " + "\"" + FileName + "\"" + " " + "\"" + FilePath + "\"";
			p.Exited += Rescale1;
			p.Start();
		}

		private void button1_Click(object sender, EventArgs e)
		{
			this.label3.Text = "";
			progressBar1.Increment(40);
			MyEnabled = false;
			if (!File.Exists(FilePath))
				CreateBackup();
			else
				Rescale1(null, null);
		}

		private bool ProcessErrors(string output, string error)
		{
			bool res = false;
			if (!string.IsNullOrEmpty(output))
			{
				AddTextInfo(output + " ");
				res = true;
			}
			if (!string.IsNullOrEmpty(error))
			{
				AddTextInfo(error);
				res = true;
			}
			return res;
		}

		private void Rescale1(object sender, EventArgs e)
		{
			string output = sender == null ? null : ((Process)sender).StandardOutput.ReadToEnd();
			string errorOutput = sender == null ? null : ((Process)sender).StandardError.ReadToEnd();
			if (ProcessErrors(output, errorOutput))
			{
				MyEnabled = true;
			}
			else
			{
				if (sender != null)
					AddTextInfo("ok\n");
				AddTextInfo("Rescale " + FileName + ": ");
				ProgressBar(80);
				Process p = initProc();
				p.StartInfo.FileName = "msr";
				p.StartInfo.Arguments = "\"" + FilePath + "\" tmp2 " + numericUpDown1.Value.ToString().Replace(',', '.');
				p.Exited += Rescale2;
				p.Start();
			}
		}

		private void Rescale2(object sender, EventArgs e)
		{
			string output = ((Process)sender).StandardOutput.ReadToEnd();
			string errorOutput = ((Process)sender).StandardError.ReadToEnd();
			if (ProcessErrors(output, errorOutput))
			{
				MyEnabled = true;
			}
			else
			{
				AddTextInfo("ok\nImport back " + FileName + ": ");
				ProgressBar(90);
				Process p = initProc();
				p.StartInfo.FileName = "gcrc";
				p.StartInfo.Arguments = "-i \"" + imgPath + "\"" + " " + "\"" + FileName + "\"" + " " + "tmp2";
				p.Exited += Rescale3;
				p.Start();
			}
		}

		private void Rescale3(object sender, EventArgs e)
		{
			string output = ((Process)sender).StandardOutput.ReadToEnd();
			string errorOutput = ((Process)sender).StandardError.ReadToEnd();
			if (!ProcessErrors(output, errorOutput))
			{
				AddTextInfo("ok\nDone");
				ProgressBar(0);
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

		private void button2_Click(object sender, EventArgs e)
		{
			MyEnabled = false;
			this.label3.Text = "Export " + FileName + ": ";
			ProgressBar(90);
			Process p = initProc();
			p.StartInfo.FileName = "gcrc";
			p.StartInfo.Arguments = "\"" + imgPath + "\"" + " " + "\"" + FileName + "\"" + " " + "tmp";
			p.Exited += new EventHandler(EventGetScale2);
			p.Start();
		}

		private void EventGetScale2(object sender, EventArgs e)
		{
			string output = ((Process)sender).StandardOutput.ReadToEnd();
			string errorOutput = ((Process)sender).StandardError.ReadToEnd();
			if (ProcessErrors(output, errorOutput))
			{
				MyEnabled = true;
			}
			else
			{

				AddTextInfo("ok\nGet " + FileName + "'s scale: ");
				ProgressBar(95);
				Process p = initProc();
				p.StartInfo.FileName = "msr";
				p.StartInfo.Arguments = "tmp";
				p.Exited += EventGetScale3;
				p.Start();
			}

		}
		delegate void AddTextInfoCallback(string text);
		private void AddTextInfo(string text)
		{
			if (this.textBox1.InvokeRequired)
			{
				AddTextInfoCallback d = new AddTextInfoCallback(AddTextInfo);
				this.Invoke(d, new object[] { text });
			}
			else
			{
				this.label3.Text += text;
			}
		}

		delegate void ProgressBarCallback(int v);
		private void ProgressBar(int v)
		{
			if (this.label3.InvokeRequired)
			{
				ProgressBarCallback d = new ProgressBarCallback(ProgressBar);
				this.Invoke(d, new object[] { v });
			}
			else
			{
				progressBar1.Value = v;
			}
		}

		delegate void ScaleBoxCallback(string text);
		private void ScaleBox(string text)
		{
			if (this.textBox1.InvokeRequired)
			{
				ScaleBoxCallback d = new ScaleBoxCallback(ScaleBox);
				this.Invoke(d, new object[] { text });
			}
			else
			{
				this.textBox1.Text = text;
			}
		}

		private void EventGetScale3(object sender, EventArgs e)
		{
			string output = ((Process)sender).StandardOutput.ReadToEnd();
			string errorOutput = ((Process)sender).StandardError.ReadToEnd();
			if (ProcessErrors(output.Contains("rror") ? output : null, errorOutput))
			{
				AddTextInfo("" + output);
			}
			else
			{
				AddTextInfo("ok");
				ScaleBox(output);
				ProgressBar(0);
			}
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

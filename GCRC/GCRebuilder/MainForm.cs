using GCRebuilder.Properties;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.IO;
using System.Text;
using System.Threading;
using System.Windows.Forms;

namespace GCRebuilder
{
	public class MainForm : Form
	{
		private delegate string ShowMTFolderDialogCB(string path);

		private delegate bool ShowMTMBoxCB(string text, string caption, MessageBoxButtons btns, MessageBoxIcon icon, MessageBoxDefaultButton defBtn, DialogResult desRes);

		private delegate void ResetProgressBarCB(int min, int max, int val);

		private delegate void UpdateProgressBarCB(int val);

		private delegate void UpdateActionLabelCB(string text);

		private delegate void ResetControlsCB(bool error, string errorText);

		private delegate int ModCB(int val);

		private class TOCClass : IComparer<MainForm.TOCItemFil>, ICloneable
		{
			public List<MainForm.TOCItemFil> fils;

			public int totalLen;

			public int dataStart;

			public int startIdx;

			public int endIdx;

			public int lastIdx;

			public int dirCount = 1;

			public int filCount = 4;

			public TOCClass(string resPath)
			{
				this.fils = new List<MainForm.TOCItemFil>();
				this.fils.Add(new MainForm.TOCItemFil(0, 0, 0, 99999, true, "root", "", resPath));
				this.fils.Add(new MainForm.TOCItemFil(1, 0, 0, 6, true, "&&SystemData", "&&systemdata\\", resPath + "&&systemdata\\"));
				this.fils.Add(new MainForm.TOCItemFil(2, 1, 0, 99999, false, "ISO.hdr", "&&SystemData\\iso.hdr", resPath + "&&SystemData\\iso.hdr"));
				this.fils.Add(new MainForm.TOCItemFil(3, 1, 9280, 99999, false, "AppLoader.ldr", "&&SystemData\\apploader.ldr", resPath + "&&SystemData\\apploader.ldr"));
				this.fils.Add(new MainForm.TOCItemFil(4, 1, 0, 99999, false, "Start.dol", "&&SystemData\\start.dol", resPath + "&&SystemData\\start.dol"));
				this.fils.Add(new MainForm.TOCItemFil(5, 1, 0, 99999, false, "Game.toc", "&&SystemData\\game.toc", resPath + "&&SystemData\\game.toc"));
				this.totalLen = 0;
				this.dataStart = this.totalLen;
				this.startIdx = this.totalLen;
			}

			public int Compare(MainForm.TOCItemFil x, MainForm.TOCItemFil y)
			{
				int result;
				if (x.pos > y.pos)
				{
					result = 1;
				}
				else if (x.pos < y.pos)
				{
					result = -1;
				}
				else
				{
					result = 0;
				}
				return result;
			}

			public object Clone()
			{
				MainForm.TOCClass tOCClass = new MainForm.TOCClass(this.fils[0].path);
				tOCClass.fils.Clear();
				tOCClass.dirCount = this.dirCount;
				tOCClass.filCount = this.filCount;
				for (int i = 0; i < this.fils.Count; i++)
				{
					tOCClass.fils.Add((MainForm.TOCItemFil)this.fils[i].Clone());
				}
				return tOCClass;
			}
		}

		private class TOCItemFil : ICloneable
		{
			public int TOCIdx;

			public int dirIdx;

			public int nextIdx;

			public int prevIdx;

			public int pos;

			public int len;

			public bool isDir;

			public string name;

			public string path;

			public string gamePath;

			public TreeNode node;

			public TOCItemFil()
			{
			}

			public TOCItemFil(int TOCIdx, int dirIdx, int pos, int len, bool isDir, string name, string gamePath, string path)
			{
				this.TOCIdx = TOCIdx;
				this.dirIdx = dirIdx;
				this.pos = pos;
				this.len = len;
				this.isDir = isDir;
				this.name = name;
				this.gamePath = gamePath;
				this.path = path;
			}

			public override string ToString()
			{
				return string.Format("pos: {0:d10} , path: '{1}'", this.pos, this.path);
			}

			public object Clone()
			{
				return new MainForm.TOCItemFil(this.TOCIdx, this.dirIdx, this.pos, this.len, this.isDir, this.name, this.gamePath, this.path);
			}
		}

		public string resPath = "";

		public string imgPath = "";

		private bool resChecked = false;

		private bool imgChecked = false;

		private bool rootOpened = true;

		private bool retrieveFilesInfo = true;

		private bool appendImage = true;

		private bool addressRebuild = true;

		private bool ignoreBannerAlpha = true;

		private bool updateImageTOC = true;

		private bool canEditTOC = false;

		private int filesMod = 2048;

		private int maxImageSize = 1459978240;

		private bool isRebuilding = false;

		private bool isWipeing = false;

		private bool isImpExping = false;

		private bool stopCurrProc = false;

		private bool escapePressed = false;

		private bool loading = true;

		private bool bannerLoaded = false;

		private bool fileNameSort = true;

		private char region = 'n';

		private TreeNode selNode;

		private ThreadStart ths;

		private Thread th;

		public static string[] args;

		private int expImpIdx;

		private string expImpPath;

		private MainForm.TOCClass toc;

		private IContainer components = null;

		private TreeView tvTOC;

		private ImageList imageList;

		private ContextMenuStrip cmsTVTOC;

		private ToolStripMenuItem miImport;

		private ToolStripMenuItem miExport;

		private ToolStripSeparator misep1;

		private ToolStripMenuItem miImpFT;

		private ToolStripMenuItem miExpFT;

		private ToolStripSeparator misep2;

		private ToolStripMenuItem miRename;

		private GroupBox gbStruct;

		private StatusStrip statusStrip;

		private ToolStripStatusLabel sslblAction;

		private ToolStripProgressBar sspbProgress;

		private GroupBox gbBannerDetails;

		private TextBox tbBDVersion;

		private Label lblBDVersion;

		private TextBox tbBDShortName;

		private Label lblBDShortName;

		private TextBox tbBDLongMaker;

		private Label lblBDLongMaker;

		private TextBox tbBDLongName;

		private Label lblBDLongName;

		private TextBox tbBDShortMaker;

		private Label lblBDShortMaker;

		private Label lblBDLanguage;

		private ComboBox cbBDLanguage;

		private Label lblBDFile;

		private ComboBox cbBDFile;

		private Label lblBDBanner;

		private TextBox tbBDDescription;

		private Label lblBDDescription;

		private PictureBox pbBDBanner;

		private Button btnBDExport;

		private Button btnBDImport;

		private Button btnBDSave;

		private MenuStrip menuStrip;

		private ToolStripMenuItem miRoot;

		private GroupBox gbISODetails;

		private TextBox tbIDDate;

		private Label lblIDDate;

		private TextBox tbIDDiscID;

		private Label lblIDDiskID;

		private TextBox tbIDMakerCode;

		private Label lblIDMakerCode;

		private TextBox tbIDRegion;

		private Label lblIDRegion;

		private TextBox tbIDGameCode;

		private Label lblIDGameCode;

		private TextBox tbIDName;

		private Label lblIDName;

		private ToolStripMenuItem miRootOpen;

		private ToolStripMenuItem miRootSave;

		private ToolStripMenuItem miRootClose;

		private ToolStripSeparator toolStripMenuItem1;

		private ToolStripMenuItem miRootStart;

		private ToolStripSeparator toolStripMenuItem2;

		private ToolStripMenuItem miRootExit;

		private ToolStripMenuItem miImage;

		private ToolStripMenuItem miImageOpen;

		private ToolStripMenuItem miImageClose;

		private ToolStripMenuItem miOptions;

		private ToolStripMenuItem miHelp;

		private ToolStripMenuItem miHelpAbout;

		private GroupBox gbSort;

		private RadioButton rbSortAddress;

		private RadioButton rbSortFileName;

		private ToolStripMenuItem miOptionsModifySystemFiles;

		private ToolStripMenuItem miOptionsDoNotUseGameToc;

		private ToolStripSeparator toolStripSeparator1;

		private ToolStripMenuItem miImageExit;

		private ToolStripMenuItem miImageWipeGarbage;

		private ToolStripSeparator toolStripSeparator2;

		private TextBox tbStartIdx;

		private Label lblStartIdx;

		private TextBox tbEndIdx;

		private Label lblEndIdx;

		private Panel panel1;

		private ToolStripMenuItem miCancel;

		private MemoryStream bnr = null;

		private BinaryReader bnrr = null;

		private BinaryWriter bnrw = null;

		private Encoding bannerEnc;

		private Image img;

		public MainForm()
		{
			this.InitializeComponent();
			this.tbBDShortName.TextChanged += new EventHandler(this.bnrInfoChanged);
			this.tbBDShortMaker.TextChanged += new EventHandler(this.bnrInfoChanged);
			this.tbBDLongName.TextChanged += new EventHandler(this.bnrInfoChanged);
			this.tbBDLongMaker.TextChanged += new EventHandler(this.bnrInfoChanged);
			this.tbBDDescription.TextChanged += new EventHandler(this.bnrInfoChanged);
			base.Icon = Resources.ico;
			this.imageList.Images.Add(Resources.root);
			this.imageList.Images.Add(Resources.dir_cl);
			this.imageList.Images.Add(Resources.dir_op);
			this.imageList.Images.Add(Resources.fil);
			this.cbBDLanguage.SelectedIndex = 0;
			this.loading = false;
		}

		private void MainForm_Load(object sender, EventArgs e)
		{
			if (MainForm.args.Length == 1 && args[0][0] != '?')
			{
				this.LoadArgs(MainForm.args[0]);

			}
		}

		//YAX
		public void RunCommand()
		{
			bool importing;
			{
				int impIdx = Array.FindIndex(args, s => s.Equals("-i") || s.Equals("--import"));
				var argList = new List<string>(args);
				importing = (impIdx != -1);
				if (importing)
				{
					argList.RemoveAt(impIdx);
					args = argList.ToArray();
				}
			}
			if (args.Length < 3)
			{
				System.Console.WriteLine("usage: to export: 'gcr file.iso file_from.data file_to'");
				System.Console.WriteLine("usage: to import: 'gcr -i file.iso file_to.dat file_from'");
			}
			else
			{
				try
				{
					FileInfo fileInfo1 = new FileInfo(args[0]);
					if (!fileInfo1.Exists)
					{
						System.Console.WriteLine("error: " + args[0] + " doesn't exist");
					}
					else
					{
						this.ImageOpen(args[0]);
						int datFileIdx = toc.fils.FindIndex(f => f.name == args[1]);
						if (datFileIdx == -1)
						{
							System.Console.WriteLine("error: " + args[1] + " doens't exist inside the iso");
						}
						else
						{
							if (importing)
							{
								FileInfo fileInfo2 = new FileInfo(args[2]);
								if (!fileInfo1.Exists)
								{
									System.Console.WriteLine("error: " + args[2] + " doesn't exist");
								}
								Import(datFileIdx, args[2]);
							}
							else
							{
								Export(datFileIdx, args[2]);
							}
						}
					}
				}
				catch (Exception ex)
				{
					System.Console.WriteLine("error: exception thrown: " + ex.Message);
				}
			}
		}

		protected override void OnClosing(CancelEventArgs e)
		{
			base.OnClosing(e);
			if (this.isRebuilding || this.isWipeing || this.isImpExping)
			{
				if (MessageBox.Show("Are yoy sure you want to close the program?", "Question", MessageBoxButtons.YesNo, MessageBoxIcon.Question, MessageBoxDefaultButton.Button2) == DialogResult.Yes)
				{
					this.th.Abort();
				}
				else
				{
					e.Cancel = true;
				}
			}
		}

		private void MainForm_KeyDown(object sender, KeyEventArgs e)
		{
			if (e.KeyCode == Keys.Escape)
			{
				this.escapePressed = true;
			}
		}

		private bool CheckResPath()
		{
			string text = "&&systemdata";
			string[] array = new string[]
			{
				"apploader.ldr",
				"game.toc",
				"iso.hdr",
				"start.dol"
			};
			bool result;
			try
			{
				DirectoryInfo directoryInfo = new DirectoryInfo(this.resPath + text);
				if (!directoryInfo.Exists)
				{
					System.Console.WriteLine("error: " + string.Format("Folder '{0}' not found", text));
					result = false;
					return result;
				}
				FileInfo[] files = directoryInfo.GetFiles();
				for (int i = 0; i < array.Length; i++)
				{
					int j;
					for (j = 0; j < files.Length; j++)
					{
						if (array[i] == files[j].Name.ToLower())
						{
							break;
						}
					}
					if (j == files.Length)
					{
						System.Console.WriteLine("error: " + string.Format("File '{0}' not found in '{1}' folder", array[i], text));
						result = false;
						return result;
					}
				}
				if (this.miOptionsDoNotUseGameToc.Checked)
				{
					if (files.Length > 4)
					{
						System.Console.WriteLine("error: " + string.Format("Misc files are not allowed in '{0}' folder", text));
						result = false;
						return result;
					}
					if (directoryInfo.GetDirectories().Length > 0)
					{
						System.Console.WriteLine("error: " + string.Format("Subfolders are not allowed in '{0}' folder", text));
						result = false;
						return result;
					}
				}
			}
			catch (Exception ex)
			{
				System.Console.WriteLine("error: " + ex.Message);
				result = false;
				return result;
			}
			result = true;
			return result;
		}

		private bool CheckImage()
		{
			FileStream fileStream = null;
			BinaryReader binaryReader = null;
			bool flag = false;
			try
			{
				fileStream = new FileStream(this.imgPath, FileMode.Open, FileAccess.Read, FileShare.Read);
				binaryReader = new BinaryReader(fileStream, Encoding.Default);
				fileStream.Position = 28L;
				if (binaryReader.ReadInt32() != 1033843650)
				{
					System.Console.WriteLine("error: " + "Not a GameCube image");
					flag = true;
				}
			}
			catch (Exception ex)
			{
				System.Console.WriteLine("error: " + ex.Message);
				flag = true;
			}
			finally
			{
				if (binaryReader != null)
				{
					binaryReader.Close();
				}
				if (fileStream != null)
				{
					fileStream.Close();
				}
			}
			return !flag;
		}

		private void LoadArgs(string arg)
		{
			try
			{
				FileInfo fileInfo = new FileInfo(arg);
				DirectoryInfo directoryInfo = new DirectoryInfo(arg);
				if (fileInfo.Exists)
				{
					this.ImageOpen(arg);
				}
				else if (directoryInfo.Exists)
				{
					this.RootOpen(arg);
				}
			}
			catch (Exception ex)
			{
				System.Console.WriteLine("error: " + ex.Message);
			}
		}

		private void ResetControls()
		{
			this.bannerLoaded = false;
			this.region = 'n';
			this.gbStruct.Text = "Structure";
			this.tvTOC.Enabled = false;
			this.tvTOC.Nodes.Clear();
			this.rbSortFileName.Enabled = false;
			this.rbSortAddress.Enabled = false;
			this.tbStartIdx.Enabled = false;
			this.tbEndIdx.Enabled = false;
			this.tbStartIdx.Text = "";
			this.tbEndIdx.Text = "";
			this.miExpFT.Visible = false;
			this.miExport.Visible = false;
			this.miImpFT.Visible = false;
			this.miImport.Visible = false;
			this.miRename.Visible = false;
			this.misep1.Visible = false;
			this.misep2.Visible = false;
			this.tbIDName.Text = "";
			this.tbIDName.Enabled = false;
			this.tbIDDiscID.Text = "";
			this.tbIDDiscID.Enabled = false;
			this.tbIDGameCode.Text = "";
			this.tbIDGameCode.Enabled = false;
			this.tbIDRegion.Text = "";
			this.tbIDRegion.Enabled = false;
			this.tbIDMakerCode.Text = "";
			this.tbIDMakerCode.Enabled = false;
			this.tbIDDate.Text = "";
			this.tbIDDate.Enabled = false;
			this.cbBDFile.Items.Clear();
			this.cbBDFile.Enabled = false;
			this.cbBDLanguage.Enabled = false;
			this.tbBDVersion.Text = "";
			this.tbBDVersion.Enabled = false;
			this.tbBDShortName.Text = "";
			this.tbBDShortName.Enabled = false;
			this.tbBDShortMaker.Text = "";
			this.tbBDShortMaker.Enabled = false;
			this.tbBDLongName.Text = "";
			this.tbBDLongName.Enabled = false;
			this.tbBDLongMaker.Text = "";
			this.tbBDLongMaker.Enabled = false;
			this.tbBDDescription.Text = "";
			this.tbBDDescription.Enabled = false;
			this.btnBDImport.Enabled = false;
			this.btnBDExport.Enabled = false;
			this.btnBDSave.Enabled = false;
			this.img = null;
			this.pbBDBanner.Invalidate();
		}

		public void RootOpen(string path)
		{
			string text = this.resPath;
			if (path.Length == 0)
			{
				FolderBrowserDialog folderBrowserDialog = new FolderBrowserDialog
				{
					Description = "Choose root folder",
					ShowNewFolderButton = false
				};
				if (this.resPath != "")
				{
					folderBrowserDialog.SelectedPath = this.resPath;
				}
				if (folderBrowserDialog.ShowDialog() == DialogResult.OK)
				{
					this.resPath = folderBrowserDialog.SelectedPath;
					path = this.resPath;
				}
			}
			if (path.Length != 0)
			{
				this.resPath = path;
				if (this.resPath.LastIndexOf('\\') != this.resPath.Length - 1)
				{
					this.resPath += '\\';
				}
				bool flag = this.CheckResPath();
				if (flag)
				{
					if (this.miOptionsDoNotUseGameToc.Checked)
					{
						flag = this.GenerateTOC();
					}
					else if (flag)
					{
						flag = this.ReadTOC();
					}
				}
				if (flag)
				{
					this.rootOpened = true;
					if (!this.fileNameSort && this.canEditTOC)
					{
						this.gbStruct.Text = "Structure (editable)";
					}
					this.miImage.Enabled = false;
					this.miOptions.Enabled = true;
					this.miOptionsDoNotUseGameToc.Enabled = false;
					this.miRootClose.Enabled = true;
					this.miRootOpen.ToolTipText = this.resPath;
					this.resChecked = true;
					if (this.resChecked && this.imgChecked)
					{
						this.miRootStart.Enabled = true;
					}
				}
				else
				{
					this.resPath = text;
				}
			}
		}

		public void ImageOpen(string path)
		{
			if (path.Length == 0)
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
					this.imgPath = openFileDialog.FileName;
					path = this.imgPath;
				}
			}
			if (path.Length != 0)
			{
				this.imgPath = path;
				if (this.CheckImage() && this.ReadImageTOC())
				{
					this.miImageOpen.ToolTipText = this.imgPath;
					this.miImageClose.Enabled = true;
					this.miImageWipeGarbage.Enabled = true;
					this.miRoot.Enabled = false;
					this.miOptions.Enabled = false;
					this.miImport.Visible = true;
					this.miExport.Visible = true;
					this.rootOpened = false;
					this.gbStruct.Text = "Structure";
				}
			}
		}

		private void cmsTVTOC_Opening(object sender, CancelEventArgs e)
		{
			if (this.tvTOC.SelectedNode == null)
			{
				e.Cancel = true;
			}
			else if (!this.rootOpened)
			{
				if (this.toc.fils[Convert.ToInt32(this.selNode.Name)].isDir)
				{
					this.miImport.Enabled = false;
				}
				else
				{
					this.miImport.Enabled = true;
				}
			}
		}

		private void tvTOC_MouseDown(object sender, MouseEventArgs e)
		{
			this.selNode = this.tvTOC.GetNodeAt(e.X, e.Y);
			if (this.selNode != null)
			{
				this.tvTOC.SelectedNode = this.selNode;
			}
			else
			{
				this.tvTOC.SelectedNode = null;
			}
		}

		private void tvTOC_KeyDown(object sender, KeyEventArgs e)
		{
			if (this.selNode == null)
			{
			}
		}

		private void tvTOC_AfterSelect(object sender, TreeViewEventArgs e)
		{
			this.selNode = this.tvTOC.SelectedNode;
			if (this.selNode != null)
			{
				int index = Convert.ToInt32(this.selNode.Name);
				if (this.toc.fils[index].isDir)
				{
					this.lblStartIdx.Text = "Start index:";
					this.lblEndIdx.Text = "End index:";
				}
				else
				{
					this.lblStartIdx.Text = "File start:";
					this.lblEndIdx.Text = "File length:";
				}
				this.tbStartIdx.Text = this.toc.fils[index].pos.ToString();
				this.tbEndIdx.Text = this.toc.fils[index].len.ToString();
			}
		}

		private void tvTOC_AfterLabelEdit(object sender, NodeLabelEditEventArgs e)
		{
		}

		private void miImport_Click(object sender, EventArgs e)
		{
			if (this.selNode != null)
			{
				int num = Convert.ToInt32(this.selNode.Name);
				if (this.toc.fils[num].isDir)
				{
					this.ImportDir(num, "");
				}
				else
				{
					this.Import(num, "");
				}
			}
		}

		private void miExport_Click(object sender, EventArgs e)
		{
			if (this.selNode != null)
			{
				int num = Convert.ToInt32(this.selNode.Name);
				if (this.toc.fils[num].isDir)
				{
					this.miImage.Enabled = false;
					this.miImport.Visible = false;
					this.miExport.Visible = false;
					this.miCancel.Visible = true;
					this.cbBDFile.Enabled = false;
					this.btnBDSave.Enabled = false;
					this.stopCurrProc = false;
					this.isImpExping = true;
					this.expImpIdx = num;
					this.expImpPath = "";
					this.ths = new ThreadStart(this.ExportDir);
					this.th = new Thread(this.ths);
					this.th.Start();
				}
				else
				{
					this.Export(num, "");
				}
			}
		}

		private void miImpFT_Click(object sender, EventArgs e)
		{
		}

		private void miExpFT_Click(object sender, EventArgs e)
		{
		}

		private void miRename_Click(object sender, EventArgs e)
		{
		}

		private void miRootOpen_Click(object sender, EventArgs e)
		{
			this.RootOpen("");
		}

		private void miRootSave_Click(object sender, EventArgs e)
		{
			SaveFileDialog saveFileDialog = new SaveFileDialog
			{
				Filter = "GameCube ISO (*.iso)|*.iso|GameCube Image File (*.gcm)|*.gcm",
				Title = "Save image"
			};
			if (saveFileDialog.ShowDialog() == DialogResult.OK)
			{
				this.imgPath = saveFileDialog.FileName;
				this.miRootSave.ToolTipText = this.imgPath;
				this.imgChecked = true;
				if (this.resChecked && this.imgChecked)
				{
					this.miRootStart.Enabled = true;
				}
			}
		}

		private void miRootClose_Click(object sender, EventArgs e)
		{
			this.miImage.Enabled = true;
			this.miOptionsDoNotUseGameToc.Enabled = true;
			this.ResetControls();
			this.miRootClose.Enabled = false;
			this.miRootStart.Enabled = false;
			this.resChecked = false;
		}

		private void miRootStart_Click(object sender, EventArgs e)
		{
			if (this.isRebuilding)
			{
				this.stopCurrProc = true;
			}
			else
			{
				this.gbStruct.Text = "Structure";
				this.miRename.Enabled = false;
				this.miRootOpen.Enabled = false;
				this.miRootSave.Enabled = false;
				this.miRootClose.Enabled = false;
				this.miRootStart.Text = "Cancel";
				this.miRootStart.Image = Resources.root_stop;
				this.miOptions.Enabled = false;
				this.cbBDFile.Enabled = false;
				this.btnBDSave.Enabled = false;
				this.stopCurrProc = false;
				this.isRebuilding = true;
				this.ths = new ThreadStart(this.Rebuild);
				this.th = new Thread(this.ths);
				this.th.Start();
			}
		}

		private void miRootExit_Click(object sender, EventArgs e)
		{
			base.Close();
		}

		private void miImageOpen_Click(object sender, EventArgs e)
		{
			this.ImageOpen("");
		}

		private void miImageClose_Click(object sender, EventArgs e)
		{
			this.miRoot.Enabled = true;
			this.miOptions.Enabled = true;
			this.ResetControls();
			this.miImageClose.Enabled = false;
			this.miImageWipeGarbage.Enabled = false;
		}

		private void miImageWipeGarbage_Click(object sender, EventArgs e)
		{
			if (this.isWipeing)
			{
				this.stopCurrProc = true;
			}
			else
			{
				this.gbStruct.Text = "Structure";
				this.miImport.Enabled = false;
				this.miImageOpen.Enabled = false;
				this.miImageClose.Enabled = false;
				this.miImport.Visible = false;
				this.miExport.Visible = false;
				this.miImageWipeGarbage.Text = "Cancel";
				this.miImageWipeGarbage.Image = Resources.root_stop;
				this.cbBDFile.Enabled = false;
				this.btnBDSave.Enabled = false;
				this.stopCurrProc = false;
				this.isWipeing = true;
				this.ths = new ThreadStart(this.WipeGarbage);
				this.th = new Thread(this.ths);
				this.th.Start();
			}
		}

		private void miImageExit_Click(object sender, EventArgs e)
		{
			base.Close();
		}

		private void miHelpAbout_Click(object sender, EventArgs e)
		{
			MessageBox.Show("Nintendo GameCube images rebuilder v1.0\r\nCreated by BSV (bsv798@gmail.com)", "About", MessageBoxButtons.OK, MessageBoxIcon.Asterisk);
		}

		private void cbBDFile_SelectedIndexChanged(object sender, EventArgs e)
		{
			if (!this.isRebuilding)
			{
				this.LoadBannerInfo(this.cbBDFile.SelectedIndex, !this.rootOpened);
			}
		}

		private void cbBDLanguage_SelectedIndexChanged(object sender, EventArgs e)
		{
			if (!this.cbBDLanguage.Enabled)
			{
				this.cbBDLanguage.SelectedIndex = 0;
			}
			else if (!this.loading)
			{
				this.LoadBannerLang(this.cbBDLanguage.SelectedIndex);
			}
		}

		private void pbBDBanner_Paint(object sender, PaintEventArgs e)
		{
			if (this.img != null)
			{
				e.Graphics.ScaleTransform(2f, 2f);
				e.Graphics.DrawImage(this.img, new Point(0, 0));
			}
		}

		private void btnBDImport_Click(object sender, EventArgs e)
		{
			OpenFileDialog openFileDialog = new OpenFileDialog
			{
				Filter = "Windows Bitmap (*.bmp)|*.bmp",
				Title = "Import banner"
			};
			if (openFileDialog.ShowDialog() == DialogResult.OK)
			{
				this.SaveBannerImage(openFileDialog.FileName);
			}
		}

		private void btnBDExport_Click(object sender, EventArgs e)
		{
			SaveFileDialog saveFileDialog = new SaveFileDialog
			{
				Filter = "Windows Bitmap (*.bmp)|*.bmp",
				Title = "Export banner"
			};
			if (saveFileDialog.ShowDialog() == DialogResult.OK)
			{
				this.img.Save(saveFileDialog.FileName);
			}
		}

		private void btnBDSave_Click(object sender, EventArgs e)
		{
			this.SaveBannerInfo(this.cbBDFile.SelectedIndex, !this.rootOpened);
		}

		private void bnrInfoChanged(object sender, EventArgs e)
		{
			this.SaveBannerLang(this.cbBDLanguage.SelectedIndex);
		}

		private void rbSortFileName_CheckedChanged(object sender, EventArgs e)
		{
			this.fileNameSort = this.rbSortFileName.Checked;
			if (!this.fileNameSort)
			{
				if (!this.isRebuilding && this.rootOpened && this.canEditTOC)
				{
					this.gbStruct.Text = "Structure (editable)";
				}
			}
			else
			{
				this.gbStruct.Text = "Structure";
			}
			this.GenerateTreeView(this.fileNameSort);
		}

		private void rbSortAddress_CheckedChanged(object sender, EventArgs e)
		{
		}

		private void miCancel_Click(object sender, EventArgs e)
		{
			if (this.isImpExping)
			{
				this.stopCurrProc = true;
			}
		}

		private bool ReadImageTOC()
		{
			bool flag = false;
			string gamePath = "";
			int[] array = new int[512];
			int num = 0;
			array[1] = 99999999;
			int num2 = 1;
			bool flag2 = false;
			string text = "";
			this.toc = new MainForm.TOCClass(this.resPath);
			int num3 = this.toc.fils.Count;
			int num4 = this.toc.fils.Count - 1;
			FileStream fileStream = new FileStream(this.imgPath, FileMode.Open, FileAccess.Read, FileShare.Read);
			BinaryReader binaryReader = new BinaryReader(fileStream, Encoding.Default);
			if (fileStream.Length > 1080L)
			{
				fileStream.Position = 1024L;
				this.toc.fils[2].pos = 0;
				this.toc.fils[2].len = 9280;
				this.toc.fils[3].pos = 9280;
				this.toc.fils[3].len = binaryReader.ReadInt32BE();
				fileStream.Position += 28L;
				this.toc.fils[4].pos = binaryReader.ReadInt32BE();
				this.toc.fils[5].pos = binaryReader.ReadInt32BE();
				this.toc.fils[5].len = binaryReader.ReadInt32BE();
				this.toc.fils[4].len = this.toc.fils[5].pos - this.toc.fils[4].pos;
				fileStream.Position += 8L;
				this.toc.dataStart = binaryReader.ReadInt32BE();
				this.toc.totalLen = (int)fileStream.Length;
			}
			else
			{
				text = "Image is too short";
				flag2 = true;
			}
			if (fileStream.Length < (long)this.toc.dataStart)
			{
				text = "Image is too short";
				flag2 = true;
			}
			if (!flag2)
			{
				fileStream.Position = (long)this.toc.fils[5].pos;
				MemoryStream memoryStream = new MemoryStream(binaryReader.ReadBytes(this.toc.fils[5].len));
				BinaryReader binaryReader2 = new BinaryReader(memoryStream, Encoding.Default);
				int i = binaryReader2.ReadInt32();
				if (i != 1)
				{
					flag2 = true;
					text = "Multiple FST image?\r\nPlease mail me info about this image";
				}
				i = binaryReader2.ReadInt32();
				if (i != 0)
				{
					flag2 = true;
					text = "Multiple FST image?\r\nPlease mail me info about this image";
				}
				int num5 = binaryReader2.ReadInt32BE() - 1;
				int num6 = num5 * 12 + 12;
				if (this.retrieveFilesInfo)
				{
					this.sspbProgress.Minimum = 0;
					this.sspbProgress.Maximum = 100;
					this.sspbProgress.Step = 1;
					this.sspbProgress.Value = 0;
					num2 = (int)Math.Floor((double)((float)(num5 + num3) / (float)this.sspbProgress.Maximum));
					if (num2 == 0)
					{
						this.sspbProgress.Maximum = num5 + num3;
						num2 = 1;
					}
				}
				for (int j = 0; j < num5; j++)
				{
					int num7 = binaryReader2.ReadInt32BE();
					if (num7 >> 24 == 1)
					{
						flag = true;
					}
					num7 &= 16777215;
					int num8 = binaryReader2.ReadInt32BE();
					int num9 = binaryReader2.ReadInt32BE();
					long position = memoryStream.Position;
					long position2 = (long)(num6 + num7);
					memoryStream.Position = position2;
					string text2 = binaryReader2.ReadStringNT();
					memoryStream.Position = position;
					while (array[num + 1] <= num3)
					{
						num -= 2;
					}
					if (flag)
					{
						num += 2;
						array[num] = ((num8 > 0) ? (num8 + num4) : num8);
						num8 += num4;
						num9 += num4;
						array[num + 1] = num9;
						this.toc.dirCount++;
					}
					else
					{
						this.toc.filCount++;
					}
					string text3 = text2;
					int num10 = array[num];
					for (i = 0; i < 256; i++)
					{
						if (num10 == 0)
						{
							gamePath = text3;
							text3 = this.resPath + text3;
							break;
						}
						text3 = text3.Insert(0, this.toc.fils[num10].name + '\\');
						num10 = this.toc.fils[num10].dirIdx;
					}
					if (flag)
					{
						text3 += '\\';
					}
					if (this.retrieveFilesInfo)
					{
						if (!flag && fileStream.Length < (long)(num8 + num9))
						{
							text = string.Format("File '{0}' not found", text3);
							flag2 = true;
						}
						if (flag2)
						{
							break;
						}
						if (num3 % num2 == 0)
						{
							if (this.sspbProgress.Value < this.sspbProgress.Maximum)
							{
								this.sspbProgress.Value++;
							}
						}
					}
					MainForm.TOCItemFil item = new MainForm.TOCItemFil(num3, array[num], num8, num9, flag, text2, gamePath, text3);
					this.toc.fils.Add(item);
					this.toc.fils[0].len = this.toc.fils.Count;
					if (flag)
					{
						array[num] = num3;
						flag = false;
					}
					num3++;
				}
				binaryReader2.Close();
				memoryStream.Close();
			}
			binaryReader.Close();
			fileStream.Close();
			if (this.retrieveFilesInfo)
			{
				this.sspbProgress.Value = 0;
			}
			bool result;
			if (flag2)
			{
				System.Console.WriteLine("error: " + text);
				result = false;
			}
			else
			{
				this.CalcNextFileIds();
				flag2 = this.GenerateTreeView(this.fileNameSort);
				this.rootOpened = false;
				this.LoadInfo(!this.rootOpened);
				result = flag2;
			}
			return result;
		}

		private void ExportDir()
		{
			int num = this.expImpIdx;
			string text = this.expImpPath;
			int[] array = new int[256];
			string[] array2 = new string[256];
			int num2 = 1;
			bool flag = false;
			bool flag2 = false;
			string errorText = "";
			if (text.Length == 0)
			{
				text = this.ShowMTFolderDialog(this.toc.fils[num].name);
				if (text != "-1")
				{
					flag = true;
				}
				else
				{
					text = "";
				}
			}
			if (text.Length == 0)
			{
				flag2 = true;
				errorText = "";
			}
			if (!flag2)
			{
				this.ResetProgressBar(0, this.toc.fils[num].len - num, 0);
				text = ((text[text.Length - 1] == '\\') ? text : (text + '\\'));
				string oldValue;
				if (num == 0)
				{
					text += "root";
					oldValue = "root:";
				}
				else
				{
					int i = this.toc.fils[num].gamePath.LastIndexOf('\\', this.toc.fils[num].gamePath.Length - 2);
					if (i < 0)
					{
						oldValue = "root:";
					}
					else
					{
						oldValue = "root:\\" + this.toc.fils[num].gamePath.Substring(0, i);
					}
				}
				array[num2] = -1;
				for (int i = num; i < this.toc.fils[num].len; i++)
				{
					while (i == array[num2])
					{
						num2--;
					}
					if (this.toc.fils[i].isDir)
					{
						DirectoryInfo directoryInfo = new DirectoryInfo(text + ("root:\\" + this.toc.fils[i].gamePath).Replace(oldValue, ""));
						if (!directoryInfo.Exists)
						{
							directoryInfo.Create();
						}
						num2++;
						array[num2] = this.toc.fils[i].len;
						array2[num2] = ((directoryInfo.FullName[directoryInfo.FullName.Length - 1] == '\\') ? directoryInfo.FullName : (directoryInfo.FullName + '\\'));
					}
					else
					{
						this.Export(i, array2[num2] + this.toc.fils[i].name);
					}
					this.UpdateProgressBar(1);
					this.UpdateActionLabel(string.Format("Export: '{0}'", this.toc.fils[i].gamePath));
					if (this.stopCurrProc)
					{
						break;
					}
				}
			}
			if (!flag)
			{
				errorText = null;
			}
			this.ResetControlsWipeing(flag2, errorText);
			this.isImpExping = false;
			this.stopCurrProc = false;
		}

		private void Export(int idx, string expPath)
		{
			bool flag = false;
			this.escapePressed = false;
			if (expPath.Length == 0)
			{
				SaveFileDialog saveFileDialog = new SaveFileDialog
				{
					Filter = "All files (*.*)|*.*",
					Title = "Export file"
				};
				saveFileDialog.FileName = this.toc.fils[idx].name;
				if (saveFileDialog.ShowDialog() == DialogResult.OK)
				{
					expPath = saveFileDialog.FileName;
					flag = true;
				}
			}
			if (expPath.Length != 0)
			{
				FileStream fileStream = new FileStream(this.imgPath, FileMode.Open, FileAccess.Read, FileShare.Read);
				BinaryReader binaryReader = new BinaryReader(fileStream, Encoding.Default);
				FileStream fileStream2 = new FileStream(expPath, FileMode.Create, FileAccess.Write, FileShare.None);
				BinaryWriter binaryWriter = new BinaryWriter(fileStream2, Encoding.Default);
				fileStream.Position = (long)this.toc.fils[idx].pos;
				long num = (long)(this.toc.fils[idx].pos + this.toc.fils[idx].len);
				int num2 = 32768;
				int num3 = this.toc.fils[idx].len;
				while (fileStream.Position < num)
				{
					num3 -= num2;
					int count;
					if (num3 < 0)
					{
						count = num2 + num3;
					}
					else
					{
						count = num2;
					}
					byte[] buffer = binaryReader.ReadBytes(count);
					binaryWriter.Write(buffer);
					if (this.escapePressed && this.ShowMTMBox("Cancel current process?", "Question", MessageBoxButtons.YesNo, MessageBoxIcon.Question, MessageBoxDefaultButton.Button2, DialogResult.Yes))
					{
						this.stopCurrProc = true;
					}
					if (this.stopCurrProc)
					{
						break;
					}
				}
				binaryWriter.Close();
				fileStream2.Close();
				binaryReader.Close();
				fileStream.Close();
				if (flag)
				{
					System.Console.WriteLine("Success");
				}
			}
		}

		private void ImportDir(int idx, string impPath)
		{
		}

		private void Import(int idx, string impPath)
		{
			bool flag = false;
			this.escapePressed = false;
			if (impPath.Length == 0)
			{
				OpenFileDialog openFileDialog = new OpenFileDialog
				{
					Filter = "All files (*.*)|*.*",
					Title = "Import file"
				};
				openFileDialog.FileName = this.toc.fils[idx].name;
				if (openFileDialog.ShowDialog() == DialogResult.OK)
				{
					impPath = openFileDialog.FileName;
					flag = true;
				}
			}
			if (impPath.Length != 0)
			{
				FileInfo fileInfo = new FileInfo(impPath);
				int prevIdx = this.toc.fils[idx].prevIdx;
				int i;
				for (i = prevIdx + 1; i < this.toc.fils.Count - 1; i++)
				{
					if (!this.toc.fils[i].isDir)
					{
						break;
					}
				}
				int num = this.toc.fils[this.toc.fils[i].nextIdx].pos;
				num = ((i == this.toc.lastIdx) ? (this.toc.totalLen - this.toc.fils[idx].pos) : (num - this.toc.fils[idx].pos));
				long num2 = (long)(this.toc.fils[idx].pos + num);
				if (fileInfo.Length > (long)num)
				{
					System.Console.WriteLine("error: " + "File to import is too large");
				}
				else
				{
					FileStream fileStream = new FileStream(impPath, FileMode.Open, FileAccess.Read, FileShare.Read);
					BinaryReader binaryReader = new BinaryReader(fileStream, Encoding.Default);
					FileStream fileStream2 = new FileStream(this.imgPath, FileMode.Open, FileAccess.Write, FileShare.None);
					BinaryWriter binaryWriter = new BinaryWriter(fileStream2, Encoding.Default);
					fileStream2.Position = (long)this.toc.fils[idx].pos;
					int num3 = 32768;
					int num4 = (int)fileInfo.Length;
					do
					{
						num4 -= num3;
						int num5;
						if (num4 < 0)
						{
							num5 = num3 + num4;
						}
						else
						{
							num5 = num3;
						}
						if (num5 < 0)
						{
							break;
						}
						byte[] buffer = binaryReader.ReadBytes(num5);
						binaryWriter.Write(buffer);
						if (this.escapePressed && this.ShowMTMBox("Cancel current process?", "Question", MessageBoxButtons.YesNo, MessageBoxIcon.Question, MessageBoxDefaultButton.Button2, DialogResult.Yes))
						{
							this.stopCurrProc = true;
						}
					}
					while (!this.stopCurrProc);
					if (!this.stopCurrProc)
					{
						while (fileStream2.Position < num2)
						{
							binaryWriter.Write(0);
						}
						this.toc.fils[idx].len = (int)fileInfo.Length;
						if (this.updateImageTOC)
						{
							idx -= 5;
							fileStream2.Position = (long)(this.toc.fils[5].pos + (idx << 3) + (idx << 2) + 8);
							binaryWriter.WriteInt32BE((int)fileStream.Length);
						}
					}
					binaryWriter.Close();
					fileStream2.Close();
					binaryReader.Close();
					fileStream.Close();
					if (flag)
					{
						System.Console.WriteLine("Success");
					}
				}
			}
		}

		private void WipeGarbage()
		{
			FileStream fileStream = null;
			BinaryWriter binaryWriter = null;
			int[] array = new int[32768];
			int num = 0;
			int num2 = 0;
			int num3 = 0;
			int num4 = 32768;
			int num5 = 0;
			byte[] buffer = new byte[num4];
			bool flag = false;
			string errorText = "";
			this.escapePressed = false;
			int index = 5;
			for (int i = 5; i < this.toc.fils.Count; i++)
			{
				if (!this.toc.fils[i].isDir)
				{
					array[num] = this.toc.fils[index].pos + this.toc.fils[index].len;
					num++;
					index = this.toc.fils[i].nextIdx;
					array[num] = this.toc.fils[index].pos - array[num - 1];
					num2 += array[num];
					num++;
				}
			}
			num -= 2;
			array[num] = this.toc.fils[index].pos + this.toc.fils[index].len;
			num++;
			num2 -= array[num];
			array[num] = this.toc.totalLen - array[num - 1];
			num2 += array[num];
			num++;
			this.ResetProgressBar(0, 100, 0);
			int num6 = (int)Math.Floor((double)((float)num2 / (float)this.sspbProgress.Maximum));
			num6 = (num6 | num4 - 1) + 1;
			int j = (int)Math.Ceiling((double)((float)num2 / (float)num6));
			if (j < 100)
			{
				this.ResetProgressBar(0, j, 0);
			}
			int num7 = (int)Math.Floor((double)((float)num2 / 1000f));
			num7 = (num7 | num4 - 1) + 1;
			try
			{
				fileStream = new FileStream(this.imgPath, FileMode.Open, FileAccess.Write, FileShare.None);
				binaryWriter = new BinaryWriter(fileStream, Encoding.Default);
			}
			catch (Exception ex)
			{
				flag = true;
				errorText = ex.Message;
			}
			this.UpdateActionLabel(string.Format("Wiping garbage: {0}/{1} bytes wiped", num3, num2));
			if (!flag)
			{
				for (int i = 0; i < num; i += 2)
				{
					int num8 = array[i + 1];
					if (num8 > 0)
					{
						fileStream.Position = (long)array[i];
						for (j = 0; j < num8; j += num4)
						{
							int num9;
							if (j + num4 < num8)
							{
								num9 = num4 - num3 % num4;
								if (num5 == 0)
								{
									num5 = num3 % num4;
								}
							}
							else
							{
								num9 = num8 % num4;
							}
							num3 += num9;
							fileStream.Write(buffer, 0, num9);
							if (num3 % num7 == 0)
							{
								this.UpdateActionLabel(string.Format("Wiping garbage: {0}/{1} bytes wiped", num3, num2));
							}
							if (num3 % num6 == 0)
							{
								this.UpdateProgressBar(1);
							}
							if (this.escapePressed && this.ShowMTMBox("Cancel current process?", "Question", MessageBoxButtons.YesNo, MessageBoxIcon.Question, MessageBoxDefaultButton.Button2, DialogResult.Yes))
							{
								this.stopCurrProc = true;
							}
							if (this.stopCurrProc)
							{
								break;
							}
						}
						if (num5 > 0)
						{
							fileStream.Write(buffer, 0, num5);
						}
						num5 = 0;
					}
				}
			}
			if (binaryWriter != null)
			{
				binaryWriter.Close();
			}
			if (fileStream != null)
			{
				fileStream.Close();
			}
			this.ResetControlsWipeing(flag, errorText);
			this.isWipeing = false;
			this.stopCurrProc = false;
		}

		private void ResetControlsWipeing(bool error, string errorText)
		{
			if (this.statusStrip.InvokeRequired)
			{
				MainForm.ResetControlsCB method = new MainForm.ResetControlsCB(this.ResetControlsWipeing);
				base.Invoke(method, new object[]
				{
					error,
					errorText
				});
			}
			else
			{
				if (errorText != null)
				{
					if (error)
					{
						System.Console.WriteLine("error: " + this, errorText);
					}
					else if (this.stopCurrProc)
					{
						MessageBox.Show(this, "Process cancelled", "Message", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
					}
					else
					{
						System.Console.WriteLine("Success");
					}
				}
				this.miImage.Enabled = true;
				this.miImageOpen.Enabled = true;
				this.miImageClose.Enabled = true;
				this.miImport.Visible = true;
				this.miExport.Visible = true;
				this.miCancel.Visible = false;
				this.miImageWipeGarbage.Text = "Wipe garbage";
				this.miImageWipeGarbage.Image = Resources.wipe;
				if (this.bannerLoaded)
				{
					this.cbBDFile.Enabled = true;
					this.btnBDSave.Enabled = true;
				}
				this.sslblAction.Text = "Ready";
				if (this.bannerLoaded)
				{
					this.cbBDFile.Enabled = true;
					this.btnBDSave.Enabled = true;
				}
				this.sspbProgress.Value = 0;
			}
		}

		private string ShowMTFolderDialog(string path)
		{
			string text = "-1";
			string result;
			if (this.statusStrip.InvokeRequired)
			{
				MainForm.ShowMTFolderDialogCB method = new MainForm.ShowMTFolderDialogCB(this.ShowMTFolderDialog);
				text = (string)base.Invoke(method, new object[]
				{
					path
				});
				result = text;
			}
			else
			{
				FolderBrowserDialog folderBrowserDialog = new FolderBrowserDialog
				{
					Description = "Export folder"
				};
				folderBrowserDialog.SelectedPath = path;
				if (folderBrowserDialog.ShowDialog() == DialogResult.OK)
				{
					text = folderBrowserDialog.SelectedPath;
				}
				result = text;
			}
			return result;
		}

		private bool ShowMTMBox(string text, string caption, MessageBoxButtons btns, MessageBoxIcon icon, MessageBoxDefaultButton defBtn, DialogResult desRes)
		{
			bool flag = false;
			bool result;
			if (this.statusStrip.InvokeRequired)
			{
				MainForm.ShowMTMBoxCB method = new MainForm.ShowMTMBoxCB(this.ShowMTMBox);
				flag = (bool)base.Invoke(method, new object[]
				{
					text,
					caption,
					btns,
					icon,
					defBtn,
					desRes
				});
				result = flag;
			}
			else
			{
				if (MessageBox.Show(this, text, caption, btns, icon, defBtn) == desRes)
				{
					flag = true;
				}
				this.escapePressed = false;
				result = flag;
			}
			return result;
		}

		private bool ReadTOC()
		{
			string str = "game.toc";
			bool flag = false;
			string gamePath = "";
			int[] array = new int[512];
			int num = 0;
			array[1] = 99999999;
			int num2 = 1;
			bool flag2 = false;
			string text = "";
			this.toc = new MainForm.TOCClass(this.resPath);
			int num3 = this.toc.fils.Count;
			int num4 = this.toc.fils.Count - 1;
			for (int i = 2; i < 6; i++)
			{
				FileInfo fileInfo = new FileInfo(this.toc.fils[i].path);
				if (!fileInfo.Exists)
				{
					text = string.Format("File '{0}' not found", this.toc.fils[i].path);
					flag2 = true;
				}
				else
				{
					this.toc.fils[i].len = (int)fileInfo.Length;
					this.toc.totalLen += this.toc.fils[i].len;
				}
			}
			this.toc.dataStart = this.toc.totalLen;
			if (!flag2)
			{
				FileStream fileStream = new FileStream(this.toc.fils[2].path, FileMode.Open, FileAccess.Read);
				BinaryReader binaryReader = new BinaryReader(fileStream, Encoding.Default);
				fileStream.Position = 28L;
				if (binaryReader.ReadInt32() != 1033843650)
				{
					text = "Not a GameCube image";
					flag2 = true;
				}
				fileStream.Position = 1024L;
				this.toc.fils[2].pos = 0;
				this.toc.fils[3].pos = 9280;
				binaryReader.ReadInt32BE();
				fileStream.Position += 28L;
				this.toc.fils[4].pos = binaryReader.ReadInt32BE();
				this.toc.fils[5].pos = binaryReader.ReadInt32BE();
				binaryReader.ReadInt32BE();
				fileStream.Position += 8L;
				this.toc.dataStart = binaryReader.ReadInt32BE();
				binaryReader.Close();
				fileStream.Close();
			}
			if (!flag2)
			{
				FileStream fileStream = new FileStream(this.resPath + "&&systemdata\\" + str, FileMode.Open, FileAccess.Read);
				BinaryReader binaryReader = new BinaryReader(fileStream, Encoding.Default);
				int i = binaryReader.ReadInt32();
				if (i != 1)
				{
					flag2 = true;
					text = "Multiple FST image?\r\nPlease mail me info about this image";
				}
				i = binaryReader.ReadInt32();
				if (i != 0)
				{
					flag2 = true;
					text = "Multiple FST image?\r\nPlease mail me info about this image";
				}
				int num5 = binaryReader.ReadInt32BE() - 1;
				int num6 = num5 * 12 + 12;
				if (this.retrieveFilesInfo)
				{
					this.sspbProgress.Minimum = 0;
					this.sspbProgress.Maximum = 100;
					this.sspbProgress.Step = 1;
					this.sspbProgress.Value = 0;
					num2 = (int)Math.Floor((double)((float)(num5 + num3) / (float)this.sspbProgress.Maximum));
					if (num2 == 0)
					{
						this.sspbProgress.Maximum = num5 + num3;
						num2 = 1;
					}
				}
				for (int j = 0; j < num5; j++)
				{
					int num7 = binaryReader.ReadInt32BE();
					if (num7 >> 24 == 1)
					{
						flag = true;
					}
					num7 &= 16777215;
					int num8 = binaryReader.ReadInt32BE();
					int num9 = binaryReader.ReadInt32BE();
					long position = fileStream.Position;
					long position2 = (long)(num6 + num7);
					fileStream.Position = position2;
					string text2 = binaryReader.ReadStringNT();
					fileStream.Position = position;
					while (array[num + 1] <= num3)
					{
						num -= 2;
					}
					if (flag)
					{
						num += 2;
						array[num] = ((num8 > 0) ? (num8 + num4) : num8);
						num8 += num4;
						num9 += num4;
						array[num + 1] = num9;
						this.toc.dirCount++;
					}
					else
					{
						this.toc.filCount++;
					}
					string text3 = text2;
					int num10 = array[num];
					for (i = 0; i < 256; i++)
					{
						if (num10 == 0)
						{
							gamePath = text3;
							text3 = this.resPath + text3;
							break;
						}
						text3 = text3.Insert(0, this.toc.fils[num10].name + '\\');
						num10 = this.toc.fils[num10].dirIdx;
					}
					if (flag)
					{
						text3 += '\\';
					}
					if (this.retrieveFilesInfo)
					{
						if (flag)
						{
							DirectoryInfo directoryInfo = new DirectoryInfo(text3);
							if (!directoryInfo.Exists)
							{
								text = string.Format("Directory '{0}' not found", text3);
								flag2 = true;
							}
						}
						else
						{
							FileInfo fileInfo = new FileInfo(text3);
							if (!fileInfo.Exists)
							{
								text = string.Format("File '{0}' not found", text3);
								flag2 = true;
							}
							else
							{
								num9 = (int)fileInfo.Length;
								this.toc.totalLen += num9;
							}
						}
						if (flag2)
						{
							break;
						}
						if (num3 % num2 == 0)
						{
							if (this.sspbProgress.Value < this.sspbProgress.Maximum)
							{
								this.sspbProgress.Value++;
							}
						}
					}
					MainForm.TOCItemFil item = new MainForm.TOCItemFil(num3, array[num], num8, num9, flag, text2, gamePath, text3);
					this.toc.fils.Add(item);
					this.toc.fils[0].len = this.toc.fils.Count;
					if (flag)
					{
						array[num] = num3;
						flag = false;
					}
					num3++;
				}
				binaryReader.Close();
				fileStream.Close();
			}
			if (this.retrieveFilesInfo)
			{
				this.sspbProgress.Value = 0;
			}
			bool result;
			if (flag2)
			{
				System.Console.WriteLine("error: " + text);
				result = false;
			}
			else
			{
				this.CalcNextFileIds();
				flag2 = this.GenerateTreeView(this.fileNameSort);
				this.rootOpened = true;
				this.LoadInfo(!this.rootOpened);
				result = flag2;
			}
			return result;
		}

		private bool GenerateTOC()
		{
			int num = 0;
			int num2 = 1000000;
			int num3 = num2;
			this.toc = new MainForm.TOCClass(this.resPath);
			this.toc.fils.RemoveRange(1, 5);
			num = this.toc.fils.Count;
			this.GetFilDirInfo(new DirectoryInfo(this.resPath), ref num, ref num3);
			num3 = num2;
			this.toc.fils[0].len -= 5;
			this.toc.fils[1].len -= 5;
			MainForm.TOCItemFil value = this.toc.fils[2];
			this.toc.fils[2] = this.toc.fils[4];
			this.toc.fils[4] = value;
			this.toc.fils[2].TOCIdx = 2;
			this.toc.fils[2].pos = num3;
			value = this.toc.fils[3];
			this.toc.fils[3] = this.toc.fils[4];
			this.toc.fils[4] = value;
			this.toc.fils[3].TOCIdx = 3;
			this.toc.fils[3].pos = num3 + 2;
			value = this.toc.fils[4];
			this.toc.fils[4] = this.toc.fils[5];
			this.toc.fils[5] = value;
			this.toc.fils[4].TOCIdx = 4;
			this.toc.fils[4].pos = num3 + 4;
			this.toc.fils[5].TOCIdx = 5;
			this.toc.fils[5].pos = num3 + 6;
			this.CalcNextFileIds();
			this.toc.fils[2].pos = num3;
			this.GenerateTreeView(this.fileNameSort);
			this.rootOpened = true;
			this.LoadInfo(!this.rootOpened);
			return true;
		}

		private void GetFilDirInfo(DirectoryInfo pDir, ref int itemNum, ref int filePos)
		{
			int num = itemNum - 1;
			DirectoryInfo[] directories = pDir.GetDirectories();
			for (int i = 0; i < directories.Length; i++)
			{
				if (directories[i].Name.ToLower() == "&&systemdata")
				{
					DirectoryInfo directoryInfo = directories[0];
					directories[0] = directories[i];
					directories[i] = directoryInfo;
					break;
				}
			}
			for (int i = 0; i < directories.Length; i++)
			{
				MainForm.TOCItemFil item = new MainForm.TOCItemFil(itemNum, num, num, 0, true, directories[i].Name, directories[i].FullName.Replace(this.resPath, ""), directories[i].FullName);
				this.toc.fils.Add(item);
				itemNum++;
				this.toc.dirCount++;
				this.GetFilDirInfo(directories[i], ref itemNum, ref filePos);
			}
			FileInfo[] files = pDir.GetFiles();
			for (int i = 0; i < files.Length; i++)
			{
				MainForm.TOCItemFil item = new MainForm.TOCItemFil(itemNum, num, filePos, (int)files[i].Length, false, files[i].Name, files[i].FullName.Replace(this.resPath, ""), files[i].FullName);
				this.toc.fils.Add(item);
				this.toc.fils[0].len = this.toc.fils.Count;
				filePos += 2;
				itemNum++;
				this.toc.filCount++;
			}
			this.toc.fils[num].len = itemNum;
		}

		private void CalcNextFileIds()
		{
			this.toc.fils[2].pos = this.toc.fils.Count + 1;
			this.toc.fils[0].nextIdx = 1;
			this.toc.fils[1].nextIdx = 2;
			this.toc.fils[2].nextIdx = 3;
			this.toc.fils[3].nextIdx = 4;
			this.toc.fils[4].nextIdx = 5;
			this.toc.fils[5].nextIdx = 6;
			MainForm.TOCClass tOCClass = (MainForm.TOCClass)this.toc.Clone();
			tOCClass.fils.Sort(tOCClass);
			this.toc.fils[2].pos = 0;
			int[] array = new int[tOCClass.filCount];
			int num = 0;
			for (int i = 0; i < tOCClass.fils.Count - 1; i++)
			{
				if (!tOCClass.fils[i].isDir)
				{
					array[num] = tOCClass.fils[i + 1].TOCIdx;
					num++;
				}
			}
			num = 0;
			for (int i = 0; i < this.toc.fils.Count; i++)
			{
				if (!this.toc.fils[i].isDir)
				{
					this.toc.fils[i].nextIdx = array[num];
					num++;
					if (num == tOCClass.filCount)
					{
						this.toc.endIdx = array[num - 2];
						this.toc.lastIdx = i;
					}
				}
			}
			num = this.toc.fils[2].nextIdx;
			for (int i = 0; i < this.toc.fils.Count - 1; i++)
			{
				if (!this.toc.fils[i].isDir)
				{
					this.toc.fils[num].prevIdx = i;
					for (int j = i + 1; j < this.toc.fils.Count - 1; j++)
					{
						if (!this.toc.fils[j].isDir)
						{
							break;
						}
						i++;
					}
					num = this.toc.fils[i + 1].nextIdx;
				}
			}
			this.toc.fils[this.toc.fils.Count - 1].nextIdx = this.toc.fils.Count - 1;
		}

		private bool GenerateTreeView(bool fileNameSort)
		{
			List<TreeNode> list = new List<TreeNode>();
			this.tvTOC.Nodes.Clear();
			this.tvTOC.BeginUpdate();
			TreeNode treeNode = new TreeNode(this.toc.fils[0].name, 0, 0);
			treeNode.Name = this.toc.fils[0].TOCIdx.ToString();
			treeNode.ToolTipText = this.resPath;
			this.toc.fils[0].node = treeNode;
			list.Add(treeNode);
			this.tvTOC.Nodes.Add(treeNode);
			bool result;
			if (fileNameSort)
			{
				for (int i = 1; i < this.toc.fils.Count; i++)
				{
					if (this.toc.fils[i].isDir)
					{
						int j;
						for (j = 0; j < list.Count; j++)
						{
							if (list[j].Name == this.toc.fils[i].dirIdx.ToString())
							{
								break;
							}
						}
						if (j == list.Count)
						{
							this.tvTOC.Nodes.Clear();
							this.tvTOC.EndUpdate();
							System.Console.WriteLine("error: " + "GenerateTreeView() error: dir2dir not found");
							result = false;
							return result;
						}
						treeNode = list[j];
						TreeNode treeNode2 = new TreeNode(this.toc.fils[i].name, 1, 2);
						treeNode2.Name = this.toc.fils[i].TOCIdx.ToString();
						treeNode2.ToolTipText = this.toc.fils[i].path;
						treeNode2.Tag = i;
						this.toc.fils[i].node = treeNode2;
						list.Add(treeNode2);
						treeNode.Nodes.Add(treeNode2);
					}
					else
					{
						int j;
						for (j = 0; j < list.Count; j++)
						{
							if (list[j].Name == this.toc.fils[i].dirIdx.ToString())
							{
								break;
							}
						}
						if (j == list.Count)
						{
							this.tvTOC.Nodes.Clear();
							this.tvTOC.EndUpdate();
							System.Console.WriteLine("error: " + "GenerateTreeView() error: dir2fil not found");
							result = false;
							return result;
						}
						treeNode = list[j];
						TreeNode treeNode2 = new TreeNode(this.toc.fils[i].name, 3, 3);
						treeNode2.Name = this.toc.fils[i].TOCIdx.ToString();
						treeNode2.ToolTipText = this.toc.fils[i].path;
						treeNode2.Tag = i;
						this.toc.fils[i].node = treeNode2;
						treeNode.Nodes.Add(treeNode2);
					}
				}
			}
			else
			{
				int num = 2;
				for (int i = 1; i < this.toc.fils.Count; i++)
				{
					if (!this.toc.fils[i].isDir)
					{
						TreeNode treeNode2 = new TreeNode(this.toc.fils[num].gamePath, 3, 3);
						treeNode2.Name = this.toc.fils[num].TOCIdx.ToString();
						treeNode2.ToolTipText = this.toc.fils[num].path;
						treeNode2.Tag = num;
						this.toc.fils[num].node = treeNode2;
						treeNode.Nodes.Add(treeNode2);
						num = this.toc.fils[i].nextIdx;
					}
				}
			}
			this.tvTOC.Nodes[0].Expand();
			this.tvTOC.SelectedNode = this.tvTOC.Nodes[0];
			this.tvTOC.EndUpdate();
			this.tvTOC.Enabled = true;
			this.rbSortFileName.Enabled = true;
			this.rbSortAddress.Enabled = true;
			this.tbStartIdx.Enabled = true;
			this.tbEndIdx.Enabled = true;
			this.tvTOC.Focus();
			result = true;
			return result;
		}

		private void Rebuild()
		{
			FileStream fileStream = null;
			BinaryWriter binaryWriter = null;
			FileStream fileStream2 = null;
			BinaryReader binaryReader = null;
			BinaryWriter binaryWriter2 = null;
			MemoryStream memoryStream = null;
			BinaryReader binaryReader2 = null;
			BinaryWriter binaryWriter3 = null;
			bool flag = true;
			bool flag2 = false;
			string errorText = "";
			int m;
			MainForm.ModCB modCB = delegate(int val)
			{
				m = val % this.filesMod;
				int result;
				if (m == 0)
				{
					result = val;
				}
				else
				{
					result = val + (this.filesMod - m);
				}
				return result;
			};
			try
			{
				fileStream = new FileStream(this.imgPath, FileMode.Create, FileAccess.Write, FileShare.None);
				binaryWriter = new BinaryWriter(fileStream, Encoding.Default);
				fileStream2 = new FileStream(this.toc.fils[2].path, FileMode.Open, FileAccess.ReadWrite, FileShare.None);
				binaryReader = new BinaryReader(fileStream2, Encoding.Default);
				memoryStream = new MemoryStream(binaryReader.ReadBytes((int)fileStream2.Length), true);
				binaryReader2 = new BinaryReader(memoryStream, Encoding.Default);
				binaryWriter3 = new BinaryWriter(memoryStream, Encoding.Default);
				memoryStream.Position = 1024L;
				this.toc.fils[2].pos = 0;
				this.toc.fils[3].pos = 9280;
				binaryWriter3.WriteInt32BE(this.toc.fils[3].len);
				memoryStream.Position += 28L;
				int i = modCB(9280 + this.toc.fils[3].len);
				binaryWriter3.WriteInt32BE(i);
				this.toc.fils[4].pos = i;
				i = modCB(i + this.toc.fils[4].len);
				binaryWriter3.WriteInt32BE(i);
				string path = this.toc.fils[5].path;
				this.toc.fils[5].path = Path.GetTempPath() + "game.toc";
				this.toc.fils[5].pos = i;
				this.toc.dataStart = 0;
				byte[] buffer = this.ReGenTOC(this.toc.fils[5].pos, out this.toc.fils[5].len, ref this.toc.dataStart, out this.toc.totalLen);
				if (this.appendImage)
				{
					int num = this.toc.totalLen - this.toc.dataStart;
					this.toc.dataStart = this.maxImageSize - num;
					buffer = this.ReGenTOC(this.toc.fils[5].pos, out this.toc.fils[5].len, ref this.toc.dataStart, out this.toc.totalLen);
				}
				binaryWriter3.WriteInt32BE(this.toc.fils[5].len);
				binaryWriter3.WriteInt32BE(this.toc.fils[5].len);
				memoryStream.Position += 4L;
				binaryWriter3.WriteInt32BE(this.toc.dataStart);
				memoryStream.WriteTo(fileStream);
				if (this.miOptionsModifySystemFiles.Checked)
				{
					fileStream2.Position = 0L;
					memoryStream.WriteTo(fileStream2);
					binaryReader.Close();
					fileStream2.Close();
					fileStream2 = new FileStream(path, FileMode.Create, FileAccess.Write, FileShare.None);
					binaryWriter2 = new BinaryWriter(fileStream2, Encoding.Default);
					binaryWriter2.Write(buffer);
					binaryWriter2.Close();
					fileStream2.Close();
				}
				else
				{
					binaryReader.Close();
					fileStream2.Close();
				}
				binaryReader2.Close();
				binaryWriter3.Close();
				memoryStream.Close();
				binaryReader.Close();
				fileStream2.Close();
				int num2 = (int)fileStream.Position;
				int num3 = 32768;
				int num4 = num3 - num2;
				this.ResetProgressBar(0, 100, 0);
				int num5 = (int)Math.Floor((double)((float)this.toc.totalLen / (float)this.sspbProgress.Maximum));
				num5 = (num5 | num3 - 1) + 1;
				i = (int)Math.Ceiling((double)((float)this.toc.totalLen / (float)num5));
				if (i < 100)
				{
					this.ResetProgressBar(0, i, 0);
				}
				int num6 = (int)Math.Floor((double)((float)this.toc.totalLen / 1000f));
				num6 = (num6 | num3 - 1) + 1;
				this.UpdateActionLabel("Rebuilding…");
				if (this.toc.totalLen > this.maxImageSize || this.toc.totalLen < 0)
				{
					errorText = "The resulting image is too large";
					flag2 = true;
				}
				int index = 3;
				FileInfo fileInfo;
				if (!flag2)
				{
					for (i = 3; i < this.toc.fils.Count; i++)
					{
						if (!this.addressRebuild)
						{
							index = i;
						}
						if (!this.toc.fils[i].isDir)
						{
							fileInfo = new FileInfo(this.toc.fils[index].path);
							if (!fileInfo.Exists)
							{
								errorText = string.Format("File '{0}' not found", fileInfo.FullName);
								flag2 = true;
								break;
							}
							fileStream2 = new FileStream(this.toc.fils[index].path, FileMode.Open, FileAccess.Read, FileShare.Read);
							binaryReader = new BinaryReader(fileStream2, Encoding.Default);
							if (!flag)
							{
								m = this.filesMod - (int)fileStream.Position % this.filesMod;
								for (int j = 0; j < m; j++)
								{
									binaryWriter.Write(0);
								}
								num2 += num3 - num2 % num3;
								m = (int)fileStream.Position;
								byte[] array = new byte[num3];
								for (int j = m; j < this.toc.dataStart; j += num3)
								{
									fileStream.Write(array, 0, num3);
									num2 += num3;
									if (num2 % num6 == 0)
									{
										this.UpdateActionLabel(string.Format("Rebuilding: {0}/{1} bytes written", num2, this.toc.totalLen));
									}
									if (num2 % num5 == 0)
									{
										this.UpdateProgressBar(1);
									}
									if (this.escapePressed && this.ShowMTMBox("Cancel current process?", "Question", MessageBoxButtons.YesNo, MessageBoxIcon.Question, MessageBoxDefaultButton.Button2, DialogResult.Yes))
									{
										this.stopCurrProc = true;
									}
									if (this.stopCurrProc)
									{
										break;
									}
								}
								if (!this.stopCurrProc)
								{
									fileStream.Write(array, 0, this.toc.dataStart % num3);
									num4 = num3;
									num2 = this.toc.dataStart + (num3 - this.toc.dataStart % num3) - num3;
									fileStream.Position = (long)this.toc.dataStart;
									flag = true;
								}
							}
							if (this.stopCurrProc)
							{
								break;
							}
							if (fileStream.Position != (long)this.toc.fils[index].pos)
							{
								m = this.toc.fils[index].pos - (int)fileStream.Position;
								for (int j = 0; j < m; j++)
								{
									binaryWriter.Write(0);
								}
								num4 -= m;
								num2 += m;
							}
							if (num4 < 0)
							{
								errorText = "Oooopps)\r\nPlease mail me info about this image";
								flag2 = true;
								break;
							}
							while (fileStream2.Position < fileStream2.Length)
							{
								byte[] array = binaryReader.ReadBytes(num4);
								int num7 = array.Length;
								num2 += num7;
								if (num7 == num4)
								{
									num4 = num3;
									if (num2 % num6 == 0)
									{
										this.UpdateActionLabel(string.Format("Rebuilding: {0}/{1} bytes written", num2, this.toc.totalLen));
										if (this.stopCurrProc)
										{
											break;
										}
									}
									if (num2 % num5 == 0)
									{
										this.UpdateProgressBar(1);
									}
								}
								else
								{
									num4 -= num7;
								}
								binaryWriter.Write(array);
								if (this.escapePressed && this.ShowMTMBox("Cancel current process?", "Question", MessageBoxButtons.YesNo, MessageBoxIcon.Question, MessageBoxDefaultButton.Button2, DialogResult.Yes))
								{
									this.stopCurrProc = true;
								}
							}
							binaryReader.Close();
							fileStream2.Close();
							if (this.stopCurrProc)
							{
								break;
							}
							if (this.addressRebuild)
							{
								index = this.toc.fils[i].nextIdx;
							}
							if (i == 5)
							{
								flag = false;
							}
						}
					}
				}
				m = this.filesMod - (int)fileStream.Position % this.filesMod;
				for (i = 0; i < m; i++)
				{
					binaryWriter.Write(0);
				}
				fileInfo = new FileInfo(this.toc.fils[5].path);
				if (fileInfo.Exists)
				{
					File.Delete(this.toc.fils[5].path);
					this.toc.fils[5].path = path;
				}
			}
			catch (Exception ex)
			{
				flag2 = true;
				errorText = ex.Message;
			}
			if (!flag2 && !this.stopCurrProc && this.appendImage)
			{
				fileStream.SetLength((long)this.maxImageSize);
			}
			if (binaryWriter != null)
			{
				binaryWriter.Close();
			}
			if (fileStream != null)
			{
				fileStream.Close();
			}
			if (binaryReader != null)
			{
				binaryReader.Close();
			}
			if (binaryWriter2 != null)
			{
				binaryWriter2.Close();
			}
			if (fileStream2 != null)
			{
				fileStream2.Close();
			}
			if (binaryReader2 != null)
			{
				binaryReader2.Close();
			}
			if (binaryWriter3 != null)
			{
				binaryWriter3.Close();
			}
			if (memoryStream != null)
			{
				memoryStream.Close();
			}
			this.ResetControlsRebuild(flag2, errorText);
			this.isRebuilding = false;
			this.stopCurrProc = false;
		}

		private byte[] ReGenTOC(int tocStart, out int tocLen, ref int dataStart, out int totalLen)
		{
			int m;
			MainForm.ModCB modCB = delegate(int val)
			{
				m = val % this.filesMod;
				int result;
				if (m == 0)
				{
					result = val;
				}
				else
				{
					result = val + (this.filesMod - m);
				}
				return result;
			};
			byte[] array = new byte[262144];
			int[] array2 = new int[32768];
			MemoryStream memoryStream = new MemoryStream(array, true);
			BinaryReader binaryReader = new BinaryReader(memoryStream, Encoding.Default);
			BinaryWriter binaryWriter = new BinaryWriter(memoryStream, Encoding.Default);
			int num = 0;
			for (int i = 6; i < this.toc.fils.Count; i++)
			{
				num += this.toc.fils[i].name.Length + 1;
			}
			int num2 = 6;
			int num3 = num2 - 1;
			int num4 = this.toc.fils.Count - num3;
			int num5 = num4 * 12;
			tocLen = num5 + num;
			if (dataStart == 0)
			{
				dataStart = modCB(tocStart + tocLen);
			}
			int num6 = 0;
			binaryWriter.Write(1);
			binaryWriter.Write(0);
			binaryWriter.WriteInt32BE(num4);
			long position = memoryStream.Position;
			int num7 = dataStart;
			int nextIdx = this.toc.fils[5].nextIdx;
			if (!this.addressRebuild)
			{
				for (int i = 6; i < this.toc.fils.Count; i++)
				{
					if (!this.toc.fils[i].isDir)
					{
						num = this.toc.fils[i].len;
						array2[i] = num7;
						num7 = modCB(num7 + num);
					}
				}
			}
			else
			{
				for (int i = 6; i < this.toc.fils.Count; i++)
				{
					if (!this.toc.fils[i].isDir)
					{
						num = this.toc.fils[nextIdx].len;
						array2[nextIdx] = num7;
						num7 = modCB(num7 + num);
						nextIdx = this.toc.fils[i].nextIdx;
					}
				}
			}
			for (int i = 6; i < this.toc.fils.Count; i++)
			{
				memoryStream.Position = position;
				int num8;
				string name;
				if (this.toc.fils[i].isDir)
				{
					num6 = ((num6 & 16777215) | 16777216);
					num8 = ((this.toc.fils[i].pos > 0) ? (this.toc.fils[i].pos - num3) : this.toc.fils[i].pos);
					num = this.toc.fils[i].len - num3;
					name = this.toc.fils[i].name;
				}
				else
				{
					num6 &= 16777215;
					num8 = array2[i];
					this.toc.fils[i].pos = num8;
					num = this.toc.fils[i].len;
					name = this.toc.fils[i].name;
				}
				binaryWriter.WriteInt32BE(num6);
				binaryWriter.WriteInt32BE(num8);
				binaryWriter.WriteInt32BE(num);
				position = memoryStream.Position;
				long position2 = (long)((num6 & 16777215) + num5);
				memoryStream.Position = position2;
				binaryWriter.WriteStringNT(name);
				num6 += name.Length + 1;
			}
			totalLen = num7;
			byte[] array3 = new byte[tocLen];
			Array.Copy(array, array3, tocLen);
			FileStream fileStream = new FileStream(this.toc.fils[5].path, FileMode.Create, FileAccess.Write, FileShare.None);
			fileStream.Write(array3, 0, tocLen);
			fileStream.Close();
			return array3;
		}

		private void ResetProgressBar(int min, int max, int val)
		{
			if (this.statusStrip.InvokeRequired)
			{
				MainForm.ResetProgressBarCB method = new MainForm.ResetProgressBarCB(this.ResetProgressBar);
				base.Invoke(method, new object[]
				{
					min,
					max,
					val
				});
			}
			else
			{
				this.sspbProgress.Minimum = min;
				this.sspbProgress.Maximum = max;
				this.sspbProgress.Value = val;
			}
		}

		private void UpdateProgressBar(int val)
		{
			if (this.statusStrip.InvokeRequired)
			{
				MainForm.UpdateProgressBarCB method = new MainForm.UpdateProgressBarCB(this.UpdateProgressBar);
				base.Invoke(method, new object[]
				{
					val
				});
			}
			else if (val < this.sspbProgress.Maximum)
			{
				if (val == 0)
				{
					this.sspbProgress.Value = val;
				}
				else
				{
					this.sspbProgress.Value += val;
				}
			}
		}

		private void UpdateActionLabel(string text)
		{
			if (this.statusStrip.InvokeRequired)
			{
				MainForm.UpdateActionLabelCB method = new MainForm.UpdateActionLabelCB(this.UpdateActionLabel);
				base.Invoke(method, new object[]
				{
					text
				});
			}
			else
			{
				this.sslblAction.Text = text;
			}
		}

		private void ResetControlsRebuild(bool error, string errorText)
		{
			if (this.statusStrip.InvokeRequired)
			{
				MainForm.ResetControlsCB method = new MainForm.ResetControlsCB(this.ResetControlsRebuild);
				base.Invoke(method, new object[]
				{
					error,
					errorText
				});
			}
			else
			{
				if (errorText != null)
				{
					if (error)
					{
						System.Console.WriteLine("error: " + this, errorText);
					}
					else if (this.stopCurrProc)
					{
						MessageBox.Show(this, "Process cancelled", "Message", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
					}
					else
					{
						System.Console.WriteLine("Success");
					}
				}
				if (!this.fileNameSort && this.canEditTOC)
				{
					this.gbStruct.Text = "Structure (editable)";
				}
				this.miRename.Enabled = true;
				this.miRootOpen.Enabled = true;
				this.miRootSave.Enabled = true;
				this.miRootClose.Enabled = true;
				this.miRootStart.Text = "Rebuild";
				this.miRootStart.Image = Resources.root_run;
				this.sslblAction.Text = "Ready";
				this.miOptions.Enabled = true;
				if (this.bannerLoaded)
				{
					this.cbBDFile.Enabled = true;
					this.btnBDSave.Enabled = true;
				}
				this.sspbProgress.Value = 0;
			}
		}

		protected override void Dispose(bool disposing)
		{
			if (disposing && this.components != null)
			{
				this.components.Dispose();
			}
			base.Dispose(disposing);
		}

		private void InitializeComponent()
		{
			this.components = new Container();
			ComponentResourceManager componentResourceManager = new ComponentResourceManager(typeof(MainForm));
			this.tvTOC = new TreeView();
			this.cmsTVTOC = new ContextMenuStrip(this.components);
			this.miImport = new ToolStripMenuItem();
			this.miExport = new ToolStripMenuItem();
			this.misep1 = new ToolStripSeparator();
			this.miImpFT = new ToolStripMenuItem();
			this.miExpFT = new ToolStripMenuItem();
			this.misep2 = new ToolStripSeparator();
			this.miRename = new ToolStripMenuItem();
			this.miCancel = new ToolStripMenuItem();
			this.imageList = new ImageList(this.components);
			this.gbStruct = new GroupBox();
			this.tbEndIdx = new TextBox();
			this.lblEndIdx = new Label();
			this.tbStartIdx = new TextBox();
			this.lblStartIdx = new Label();
			this.gbSort = new GroupBox();
			this.rbSortAddress = new RadioButton();
			this.rbSortFileName = new RadioButton();
			this.statusStrip = new StatusStrip();
			this.sslblAction = new ToolStripStatusLabel();
			this.sspbProgress = new ToolStripProgressBar();
			this.gbBannerDetails = new GroupBox();
			this.btnBDSave = new Button();
			this.btnBDExport = new Button();
			this.btnBDImport = new Button();
			this.pbBDBanner = new PictureBox();
			this.lblBDBanner = new Label();
			this.tbBDDescription = new TextBox();
			this.lblBDDescription = new Label();
			this.lblBDLanguage = new Label();
			this.cbBDLanguage = new ComboBox();
			this.lblBDFile = new Label();
			this.cbBDFile = new ComboBox();
			this.tbBDLongMaker = new TextBox();
			this.lblBDLongMaker = new Label();
			this.tbBDLongName = new TextBox();
			this.lblBDLongName = new Label();
			this.tbBDShortMaker = new TextBox();
			this.lblBDShortMaker = new Label();
			this.tbBDVersion = new TextBox();
			this.lblBDVersion = new Label();
			this.tbBDShortName = new TextBox();
			this.lblBDShortName = new Label();
			this.menuStrip = new MenuStrip();
			this.miRoot = new ToolStripMenuItem();
			this.miRootOpen = new ToolStripMenuItem();
			this.miRootSave = new ToolStripMenuItem();
			this.miRootClose = new ToolStripMenuItem();
			this.toolStripMenuItem1 = new ToolStripSeparator();
			this.miRootStart = new ToolStripMenuItem();
			this.toolStripMenuItem2 = new ToolStripSeparator();
			this.miRootExit = new ToolStripMenuItem();
			this.miImage = new ToolStripMenuItem();
			this.miImageOpen = new ToolStripMenuItem();
			this.miImageClose = new ToolStripMenuItem();
			this.toolStripSeparator1 = new ToolStripSeparator();
			this.miImageWipeGarbage = new ToolStripMenuItem();
			this.toolStripSeparator2 = new ToolStripSeparator();
			this.miImageExit = new ToolStripMenuItem();
			this.miOptions = new ToolStripMenuItem();
			this.miOptionsModifySystemFiles = new ToolStripMenuItem();
			this.miOptionsDoNotUseGameToc = new ToolStripMenuItem();
			this.miHelp = new ToolStripMenuItem();
			this.miHelpAbout = new ToolStripMenuItem();
			this.gbISODetails = new GroupBox();
			this.tbIDDate = new TextBox();
			this.lblIDDate = new Label();
			this.tbIDDiscID = new TextBox();
			this.lblIDDiskID = new Label();
			this.tbIDMakerCode = new TextBox();
			this.lblIDMakerCode = new Label();
			this.tbIDRegion = new TextBox();
			this.lblIDRegion = new Label();
			this.tbIDGameCode = new TextBox();
			this.lblIDGameCode = new Label();
			this.tbIDName = new TextBox();
			this.lblIDName = new Label();
			this.panel1 = new Panel();
			this.cmsTVTOC.SuspendLayout();
			this.gbStruct.SuspendLayout();
			this.gbSort.SuspendLayout();
			this.statusStrip.SuspendLayout();
			this.gbBannerDetails.SuspendLayout();
			((ISupportInitialize)this.pbBDBanner).BeginInit();
			this.menuStrip.SuspendLayout();
			this.gbISODetails.SuspendLayout();
			this.panel1.SuspendLayout();
			base.SuspendLayout();
			this.tvTOC.ContextMenuStrip = this.cmsTVTOC;
			this.tvTOC.Enabled = false;
			this.tvTOC.ImageIndex = 0;
			this.tvTOC.ImageList = this.imageList;
			this.tvTOC.Location = new Point(9, 71);
			this.tvTOC.Margin = new Padding(6, 3, 6, 3);
			this.tvTOC.Name = "tvTOC";
			this.tvTOC.SelectedImageIndex = 0;
			this.tvTOC.ShowNodeToolTips = true;
			this.tvTOC.Size = new Size(403, 331);
			this.tvTOC.TabIndex = 8;
			this.tvTOC.AfterLabelEdit += new NodeLabelEditEventHandler(this.tvTOC_AfterLabelEdit);
			this.tvTOC.AfterSelect += new TreeViewEventHandler(this.tvTOC_AfterSelect);
			this.tvTOC.MouseDown += new MouseEventHandler(this.tvTOC_MouseDown);
			this.tvTOC.KeyDown += new KeyEventHandler(this.tvTOC_KeyDown);
			this.cmsTVTOC.Items.AddRange(new ToolStripItem[]
			{
				this.miImport,
				this.miExport,
				this.misep1,
				this.miImpFT,
				this.miExpFT,
				this.misep2,
				this.miRename,
				this.miCancel
			});
			this.cmsTVTOC.Name = "cmsTVTOC";
			this.cmsTVTOC.Size = new Size(166, 148);
			this.cmsTVTOC.Opening += new CancelEventHandler(this.cmsTVTOC_Opening);
			this.miImport.Name = "miImport";
			this.miImport.Size = new Size(165, 22);
			this.miImport.Text = "Import…";
			this.miImport.Visible = false;
			this.miImport.Click += new EventHandler(this.miImport_Click);
			this.miExport.Name = "miExport";
			this.miExport.Size = new Size(165, 22);
			this.miExport.Text = "Export…";
			this.miExport.Visible = false;
			this.miExport.Click += new EventHandler(this.miExport_Click);
			this.misep1.Name = "misep1";
			this.misep1.Size = new Size(162, 6);
			this.misep1.Visible = false;
			this.miImpFT.Name = "miImpFT";
			this.miImpFT.Size = new Size(165, 22);
			this.miImpFT.Text = "Import from-to…";
			this.miImpFT.Visible = false;
			this.miImpFT.Click += new EventHandler(this.miImpFT_Click);
			this.miExpFT.Name = "miExpFT";
			this.miExpFT.Size = new Size(165, 22);
			this.miExpFT.Text = "Export from-to…";
			this.miExpFT.Visible = false;
			this.miExpFT.Click += new EventHandler(this.miExpFT_Click);
			this.misep2.Name = "misep2";
			this.misep2.Size = new Size(162, 6);
			this.misep2.Visible = false;
			this.miRename.Name = "miRename";
			this.miRename.Size = new Size(165, 22);
			this.miRename.Text = "Rename";
			this.miRename.Visible = false;
			this.miRename.Click += new EventHandler(this.miRename_Click);
			this.miCancel.Name = "miCancel";
			this.miCancel.Size = new Size(165, 22);
			this.miCancel.Text = "Cancel";
			this.miCancel.Visible = false;
			this.miCancel.Click += new EventHandler(this.miCancel_Click);
			this.imageList.ColorDepth = ColorDepth.Depth8Bit;
			this.imageList.ImageSize = new Size(16, 16);
			this.imageList.TransparentColor = Color.Transparent;
			this.gbStruct.Controls.Add(this.tbEndIdx);
			this.gbStruct.Controls.Add(this.lblEndIdx);
			this.gbStruct.Controls.Add(this.tbStartIdx);
			this.gbStruct.Controls.Add(this.lblStartIdx);
			this.gbStruct.Controls.Add(this.gbSort);
			this.gbStruct.Controls.Add(this.tvTOC);
			this.gbStruct.Location = new Point(458, 11);
			this.gbStruct.Name = "gbStruct";
			this.gbStruct.Size = new Size(421, 442);
			this.gbStruct.TabIndex = 9;
			this.gbStruct.TabStop = false;
			this.gbStruct.Text = "Structure";
			this.tbEndIdx.Enabled = false;
			this.tbEndIdx.Font = new Font("Microsoft Sans Serif", 8.25f);
			this.tbEndIdx.Location = new Point(309, 408);
			this.tbEndIdx.MaxLength = 4;
			this.tbEndIdx.Name = "tbEndIdx";
			this.tbEndIdx.ReadOnly = true;
			this.tbEndIdx.Size = new Size(103, 20);
			this.tbEndIdx.TabIndex = 17;
			this.lblEndIdx.AutoSize = true;
			this.lblEndIdx.Location = new Point(236, 412);
			this.lblEndIdx.Margin = new Padding(3);
			this.lblEndIdx.Name = "lblEndIdx";
			this.lblEndIdx.Size = new Size(57, 13);
			this.lblEndIdx.TabIndex = 16;
			this.lblEndIdx.Text = "End index:";
			this.tbStartIdx.Enabled = false;
			this.tbStartIdx.Font = new Font("Microsoft Sans Serif", 8.25f);
			this.tbStartIdx.Location = new Point(79, 408);
			this.tbStartIdx.MaxLength = 4;
			this.tbStartIdx.Name = "tbStartIdx";
			this.tbStartIdx.ReadOnly = true;
			this.tbStartIdx.Size = new Size(103, 20);
			this.tbStartIdx.TabIndex = 15;
			this.lblStartIdx.AutoSize = true;
			this.lblStartIdx.Location = new Point(6, 412);
			this.lblStartIdx.Margin = new Padding(3);
			this.lblStartIdx.Name = "lblStartIdx";
			this.lblStartIdx.Size = new Size(60, 13);
			this.lblStartIdx.TabIndex = 14;
			this.lblStartIdx.Text = "Start index:";
			this.gbSort.Controls.Add(this.rbSortAddress);
			this.gbSort.Controls.Add(this.rbSortFileName);
			this.gbSort.Location = new Point(9, 19);
			this.gbSort.Name = "gbSort";
			this.gbSort.Size = new Size(403, 45);
			this.gbSort.TabIndex = 9;
			this.gbSort.TabStop = false;
			this.gbSort.Text = "Sort method";
			this.rbSortAddress.AutoSize = true;
			this.rbSortAddress.Enabled = false;
			this.rbSortAddress.Location = new Point(199, 19);
			this.rbSortAddress.Name = "rbSortAddress";
			this.rbSortAddress.Size = new Size(100, 17);
			this.rbSortAddress.TabIndex = 1;
			this.rbSortAddress.Text = "Addresses table";
			this.rbSortAddress.UseVisualStyleBackColor = true;
			this.rbSortAddress.CheckedChanged += new EventHandler(this.rbSortAddress_CheckedChanged);
			this.rbSortFileName.AutoSize = true;
			this.rbSortFileName.Checked = true;
			this.rbSortFileName.Enabled = false;
			this.rbSortFileName.Location = new Point(6, 19);
			this.rbSortFileName.Name = "rbSortFileName";
			this.rbSortFileName.Size = new Size(101, 17);
			this.rbSortFileName.TabIndex = 0;
			this.rbSortFileName.TabStop = true;
			this.rbSortFileName.Text = "File names table";
			this.rbSortFileName.UseVisualStyleBackColor = true;
			this.rbSortFileName.CheckedChanged += new EventHandler(this.rbSortFileName_CheckedChanged);
			this.statusStrip.Items.AddRange(new ToolStripItem[]
			{
				this.sslblAction,
				this.sspbProgress
			});
			this.statusStrip.Location = new Point(0, 487);
			this.statusStrip.Name = "statusStrip";
			this.statusStrip.Size = new Size(886, 22);
			this.statusStrip.SizingGrip = false;
			this.statusStrip.TabIndex = 11;
			this.statusStrip.Text = "Progress";
			this.sslblAction.AutoSize = false;
			this.sslblAction.BorderSides = ToolStripStatusLabelBorderSides.All;
			this.sslblAction.DisplayStyle = ToolStripItemDisplayStyle.Text;
			this.sslblAction.Name = "sslblAction";
			this.sslblAction.Size = new Size(256, 17);
			this.sslblAction.Tag = "Action: ";
			this.sslblAction.Text = "Ready";
			this.sslblAction.TextAlign = ContentAlignment.MiddleLeft;
			this.sspbProgress.Name = "sspbProgress";
			this.sspbProgress.Size = new Size(628, 16);
			this.sspbProgress.Step = 1;
			this.gbBannerDetails.Controls.Add(this.btnBDSave);
			this.gbBannerDetails.Controls.Add(this.btnBDExport);
			this.gbBannerDetails.Controls.Add(this.btnBDImport);
			this.gbBannerDetails.Controls.Add(this.pbBDBanner);
			this.gbBannerDetails.Controls.Add(this.lblBDBanner);
			this.gbBannerDetails.Controls.Add(this.tbBDDescription);
			this.gbBannerDetails.Controls.Add(this.lblBDDescription);
			this.gbBannerDetails.Controls.Add(this.lblBDLanguage);
			this.gbBannerDetails.Controls.Add(this.cbBDLanguage);
			this.gbBannerDetails.Controls.Add(this.lblBDFile);
			this.gbBannerDetails.Controls.Add(this.cbBDFile);
			this.gbBannerDetails.Controls.Add(this.tbBDLongMaker);
			this.gbBannerDetails.Controls.Add(this.lblBDLongMaker);
			this.gbBannerDetails.Controls.Add(this.tbBDLongName);
			this.gbBannerDetails.Controls.Add(this.lblBDLongName);
			this.gbBannerDetails.Controls.Add(this.tbBDShortMaker);
			this.gbBannerDetails.Controls.Add(this.lblBDShortMaker);
			this.gbBannerDetails.Controls.Add(this.tbBDVersion);
			this.gbBannerDetails.Controls.Add(this.lblBDVersion);
			this.gbBannerDetails.Controls.Add(this.tbBDShortName);
			this.gbBannerDetails.Controls.Add(this.lblBDShortName);
			this.gbBannerDetails.Location = new Point(7, 124);
			this.gbBannerDetails.Name = "gbBannerDetails";
			this.gbBannerDetails.Size = new Size(437, 329);
			this.gbBannerDetails.TabIndex = 12;
			this.gbBannerDetails.TabStop = false;
			this.gbBannerDetails.Text = "Banner details";
			this.btnBDSave.Enabled = false;
			this.btnBDSave.Location = new Point(279, 292);
			this.btnBDSave.Name = "btnBDSave";
			this.btnBDSave.Size = new Size(152, 23);
			this.btnBDSave.TabIndex = 32;
			this.btnBDSave.Text = "Save changes";
			this.btnBDSave.UseVisualStyleBackColor = true;
			this.btnBDSave.Click += new EventHandler(this.btnBDSave_Click);
			this.btnBDExport.Enabled = false;
			this.btnBDExport.Location = new Point(279, 263);
			this.btnBDExport.Name = "btnBDExport";
			this.btnBDExport.Size = new Size(75, 23);
			this.btnBDExport.TabIndex = 31;
			this.btnBDExport.Text = "Export…";
			this.btnBDExport.UseVisualStyleBackColor = true;
			this.btnBDExport.Click += new EventHandler(this.btnBDExport_Click);
			this.btnBDImport.Enabled = false;
			this.btnBDImport.Location = new Point(279, 220);
			this.btnBDImport.Name = "btnBDImport";
			this.btnBDImport.Size = new Size(75, 23);
			this.btnBDImport.TabIndex = 30;
			this.btnBDImport.Text = "Import…";
			this.btnBDImport.UseVisualStyleBackColor = true;
			this.btnBDImport.Click += new EventHandler(this.btnBDImport_Click);
			this.pbBDBanner.BackColor = SystemColors.Window;
			this.pbBDBanner.BorderStyle = BorderStyle.FixedSingle;
			this.pbBDBanner.Location = new Point(79, 220);
			this.pbBDBanner.Name = "pbBDBanner";
			this.pbBDBanner.Size = new Size(192, 64);
			this.pbBDBanner.TabIndex = 29;
			this.pbBDBanner.TabStop = false;
			this.pbBDBanner.Paint += new PaintEventHandler(this.pbBDBanner_Paint);
			this.lblBDBanner.AutoSize = true;
			this.lblBDBanner.Location = new Point(5, 224);
			this.lblBDBanner.Margin = new Padding(3);
			this.lblBDBanner.Name = "lblBDBanner";
			this.lblBDBanner.Size = new Size(41, 13);
			this.lblBDBanner.TabIndex = 28;
			this.lblBDBanner.Text = "Banner";
			this.tbBDDescription.Enabled = false;
			this.tbBDDescription.Font = new Font("MS Mincho", 9.25f);
			this.tbBDDescription.Location = new Point(78, 176);
			this.tbBDDescription.MaxLength = 127;
			this.tbBDDescription.Multiline = true;
			this.tbBDDescription.Name = "tbBDDescription";
			this.tbBDDescription.Size = new Size(353, 38);
			this.tbBDDescription.TabIndex = 27;
			this.lblBDDescription.AutoSize = true;
			this.lblBDDescription.Location = new Point(5, 180);
			this.lblBDDescription.Margin = new Padding(3);
			this.lblBDDescription.Name = "lblBDDescription";
			this.lblBDDescription.Size = new Size(63, 13);
			this.lblBDDescription.TabIndex = 26;
			this.lblBDDescription.Text = "Description:";
			this.lblBDLanguage.AutoSize = true;
			this.lblBDLanguage.Location = new Point(248, 48);
			this.lblBDLanguage.Margin = new Padding(3);
			this.lblBDLanguage.Name = "lblBDLanguage";
			this.lblBDLanguage.Size = new Size(58, 13);
			this.lblBDLanguage.TabIndex = 25;
			this.lblBDLanguage.Text = "Language:";
			this.cbBDLanguage.DropDownStyle = ComboBoxStyle.DropDownList;
			this.cbBDLanguage.Enabled = false;
			this.cbBDLanguage.FormattingEnabled = true;
			this.cbBDLanguage.Items.AddRange(new object[]
			{
				"English",
				"German",
				"French",
				"Spanish",
				"Italian",
				"Dutch"
			});
			this.cbBDLanguage.Location = new Point(321, 45);
			this.cbBDLanguage.Name = "cbBDLanguage";
			this.cbBDLanguage.Size = new Size(110, 21);
			this.cbBDLanguage.TabIndex = 24;
			this.cbBDLanguage.SelectedIndexChanged += new EventHandler(this.cbBDLanguage_SelectedIndexChanged);
			this.lblBDFile.AutoSize = true;
			this.lblBDFile.Location = new Point(6, 22);
			this.lblBDFile.Margin = new Padding(3);
			this.lblBDFile.Name = "lblBDFile";
			this.lblBDFile.Size = new Size(26, 13);
			this.lblBDFile.TabIndex = 23;
			this.lblBDFile.Text = "File:";
			this.cbBDFile.DropDownStyle = ComboBoxStyle.DropDownList;
			this.cbBDFile.Enabled = false;
			this.cbBDFile.FormattingEnabled = true;
			this.cbBDFile.Location = new Point(79, 19);
			this.cbBDFile.Name = "cbBDFile";
			this.cbBDFile.Size = new Size(139, 21);
			this.cbBDFile.TabIndex = 22;
			this.cbBDFile.SelectedIndexChanged += new EventHandler(this.cbBDFile_SelectedIndexChanged);
			this.tbBDLongMaker.Enabled = false;
			this.tbBDLongMaker.Font = new Font("MS Mincho", 9.25f);
			this.tbBDLongMaker.Location = new Point(78, 150);
			this.tbBDLongMaker.MaxLength = 63;
			this.tbBDLongMaker.Name = "tbBDLongMaker";
			this.tbBDLongMaker.Size = new Size(353, 20);
			this.tbBDLongMaker.TabIndex = 21;
			this.lblBDLongMaker.AutoSize = true;
			this.lblBDLongMaker.Location = new Point(5, 154);
			this.lblBDLongMaker.Margin = new Padding(3);
			this.lblBDLongMaker.Name = "lblBDLongMaker";
			this.lblBDLongMaker.Size = new Size(66, 13);
			this.lblBDLongMaker.TabIndex = 20;
			this.lblBDLongMaker.Text = "Long maker:";
			this.tbBDLongName.Enabled = false;
			this.tbBDLongName.Font = new Font("MS Mincho", 9.25f);
			this.tbBDLongName.Location = new Point(78, 124);
			this.tbBDLongName.MaxLength = 63;
			this.tbBDLongName.Name = "tbBDLongName";
			this.tbBDLongName.Size = new Size(353, 20);
			this.tbBDLongName.TabIndex = 19;
			this.lblBDLongName.AutoSize = true;
			this.lblBDLongName.Location = new Point(5, 128);
			this.lblBDLongName.Margin = new Padding(3);
			this.lblBDLongName.Name = "lblBDLongName";
			this.lblBDLongName.Size = new Size(63, 13);
			this.lblBDLongName.TabIndex = 18;
			this.lblBDLongName.Text = "Long name:";
			this.tbBDShortMaker.Enabled = false;
			this.tbBDShortMaker.Font = new Font("MS Mincho", 9.25f);
			this.tbBDShortMaker.Location = new Point(78, 98);
			this.tbBDShortMaker.MaxLength = 31;
			this.tbBDShortMaker.Name = "tbBDShortMaker";
			this.tbBDShortMaker.Size = new Size(353, 20);
			this.tbBDShortMaker.TabIndex = 17;
			this.lblBDShortMaker.AutoSize = true;
			this.lblBDShortMaker.Location = new Point(5, 102);
			this.lblBDShortMaker.Margin = new Padding(3);
			this.lblBDShortMaker.Name = "lblBDShortMaker";
			this.lblBDShortMaker.Size = new Size(67, 13);
			this.lblBDShortMaker.TabIndex = 16;
			this.lblBDShortMaker.Text = "Short maker:";
			this.tbBDVersion.Enabled = false;
			this.tbBDVersion.Font = new Font("Microsoft Sans Serif", 8.25f);
			this.tbBDVersion.Location = new Point(78, 46);
			this.tbBDVersion.MaxLength = 4;
			this.tbBDVersion.Name = "tbBDVersion";
			this.tbBDVersion.ReadOnly = true;
			this.tbBDVersion.Size = new Size(140, 20);
			this.tbBDVersion.TabIndex = 13;
			this.lblBDVersion.AutoSize = true;
			this.lblBDVersion.Location = new Point(5, 50);
			this.lblBDVersion.Margin = new Padding(3);
			this.lblBDVersion.Name = "lblBDVersion";
			this.lblBDVersion.Size = new Size(45, 13);
			this.lblBDVersion.TabIndex = 12;
			this.lblBDVersion.Text = "Version:";
			this.tbBDShortName.Enabled = false;
			this.tbBDShortName.Font = new Font("MS Mincho", 9.25f);
			this.tbBDShortName.Location = new Point(78, 72);
			this.tbBDShortName.MaxLength = 31;
			this.tbBDShortName.Name = "tbBDShortName";
			this.tbBDShortName.Size = new Size(353, 20);
			this.tbBDShortName.TabIndex = 1;
			this.lblBDShortName.AutoSize = true;
			this.lblBDShortName.Location = new Point(5, 76);
			this.lblBDShortName.Margin = new Padding(3);
			this.lblBDShortName.Name = "lblBDShortName";
			this.lblBDShortName.Size = new Size(64, 13);
			this.lblBDShortName.TabIndex = 0;
			this.lblBDShortName.Text = "Short name:";
			this.menuStrip.BackColor = SystemColors.Control;
			this.menuStrip.BackgroundImageLayout = ImageLayout.None;
			this.menuStrip.Items.AddRange(new ToolStripItem[]
			{
				this.miRoot,
				this.miImage,
				this.miOptions,
				this.miHelp
			});
			this.menuStrip.Location = new Point(0, 0);
			this.menuStrip.Name = "menuStrip";
			this.menuStrip.Size = new Size(886, 24);
			this.menuStrip.TabIndex = 13;
			this.menuStrip.Text = "menuStrip1";
			this.miRoot.DisplayStyle = ToolStripItemDisplayStyle.Text;
			this.miRoot.DropDownItems.AddRange(new ToolStripItem[]
			{
				this.miRootOpen,
				this.miRootSave,
				this.miRootClose,
				this.toolStripMenuItem1,
				this.miRootStart,
				this.toolStripMenuItem2,
				this.miRootExit
			});
			this.miRoot.Name = "miRoot";
			this.miRoot.Size = new Size(42, 20);
			this.miRoot.Text = "Root";
			this.miRootOpen.Image = Resources.root_open;
			this.miRootOpen.Name = "miRootOpen";
			this.miRootOpen.Size = new Size(120, 22);
			this.miRootOpen.Text = "Open…";
			this.miRootOpen.Click += new EventHandler(this.miRootOpen_Click);
			this.miRootSave.Image = Resources.root_save;
			this.miRootSave.Name = "miRootSave";
			this.miRootSave.Size = new Size(120, 22);
			this.miRootSave.Text = "Save…";
			this.miRootSave.Click += new EventHandler(this.miRootSave_Click);
			this.miRootClose.Enabled = false;
			this.miRootClose.Image = Resources.root_close;
			this.miRootClose.Name = "miRootClose";
			this.miRootClose.Size = new Size(120, 22);
			this.miRootClose.Text = "Close";
			this.miRootClose.Click += new EventHandler(this.miRootClose_Click);
			this.toolStripMenuItem1.Name = "toolStripMenuItem1";
			this.toolStripMenuItem1.Size = new Size(117, 6);
			this.miRootStart.Enabled = false;
			this.miRootStart.Image = Resources.root_run;
			this.miRootStart.Name = "miRootStart";
			this.miRootStart.Size = new Size(120, 22);
			this.miRootStart.Text = "Rebuild";
			this.miRootStart.Click += new EventHandler(this.miRootStart_Click);
			this.toolStripMenuItem2.Name = "toolStripMenuItem2";
			this.toolStripMenuItem2.Size = new Size(117, 6);
			this.miRootExit.Image = Resources.exit;
			this.miRootExit.Name = "miRootExit";
			this.miRootExit.Size = new Size(120, 22);
			this.miRootExit.Text = "Exit";
			this.miRootExit.Click += new EventHandler(this.miRootExit_Click);
			this.miImage.DisplayStyle = ToolStripItemDisplayStyle.Text;
			this.miImage.DropDownItems.AddRange(new ToolStripItem[]
			{
				this.miImageOpen,
				this.miImageClose,
				this.toolStripSeparator1,
				this.miImageWipeGarbage,
				this.toolStripSeparator2,
				this.miImageExit
			});
			this.miImage.Name = "miImage";
			this.miImage.Size = new Size(49, 20);
			this.miImage.Text = "Image";
			this.miImageOpen.Image = (Image)componentResourceManager.GetObject("miImageOpen.Image");
			this.miImageOpen.Name = "miImageOpen";
			this.miImageOpen.Size = new Size(152, 22);
			this.miImageOpen.Text = "Open…";
			this.miImageOpen.Click += new EventHandler(this.miImageOpen_Click);
			this.miImageClose.Enabled = false;
			this.miImageClose.Image = (Image)componentResourceManager.GetObject("miImageClose.Image");
			this.miImageClose.Name = "miImageClose";
			this.miImageClose.Size = new Size(152, 22);
			this.miImageClose.Text = "Close";
			this.miImageClose.Click += new EventHandler(this.miImageClose_Click);
			this.toolStripSeparator1.Name = "toolStripSeparator1";
			this.toolStripSeparator1.Size = new Size(149, 6);
			this.miImageWipeGarbage.Enabled = false;
			this.miImageWipeGarbage.Image = Resources.wipe;
			this.miImageWipeGarbage.Name = "miImageWipeGarbage";
			this.miImageWipeGarbage.Size = new Size(152, 22);
			this.miImageWipeGarbage.Text = "Wipe garbage";
			this.miImageWipeGarbage.Click += new EventHandler(this.miImageWipeGarbage_Click);
			this.toolStripSeparator2.Name = "toolStripSeparator2";
			this.toolStripSeparator2.Size = new Size(149, 6);
			this.miImageExit.Image = Resources.exit;
			this.miImageExit.Name = "miImageExit";
			this.miImageExit.Size = new Size(152, 22);
			this.miImageExit.Text = "Exit";
			this.miImageExit.Click += new EventHandler(this.miImageExit_Click);
			this.miOptions.DisplayStyle = ToolStripItemDisplayStyle.Text;
			this.miOptions.DropDownItems.AddRange(new ToolStripItem[]
			{
				this.miOptionsModifySystemFiles,
				this.miOptionsDoNotUseGameToc
			});
			this.miOptions.Name = "miOptions";
			this.miOptions.Size = new Size(56, 20);
			this.miOptions.Text = "Options";
			this.miOptionsModifySystemFiles.CheckOnClick = true;
			this.miOptionsModifySystemFiles.Name = "miOptionsModifySystemFiles";
			this.miOptionsModifySystemFiles.Size = new Size(189, 22);
			this.miOptionsModifySystemFiles.Text = "Modify system files";
			this.miOptionsDoNotUseGameToc.CheckOnClick = true;
			this.miOptionsDoNotUseGameToc.Name = "miOptionsDoNotUseGameToc";
			this.miOptionsDoNotUseGameToc.Size = new Size(189, 22);
			this.miOptionsDoNotUseGameToc.Text = "Do not use 'game.toc'";
			this.miHelp.DisplayStyle = ToolStripItemDisplayStyle.Text;
			this.miHelp.DropDownItems.AddRange(new ToolStripItem[]
			{
				this.miHelpAbout
			});
			this.miHelp.Name = "miHelp";
			this.miHelp.Size = new Size(40, 20);
			this.miHelp.Text = "Help";
			this.miHelpAbout.Image = Resources.about;
			this.miHelpAbout.Name = "miHelpAbout";
			this.miHelpAbout.Size = new Size(114, 22);
			this.miHelpAbout.Text = "About";
			this.miHelpAbout.Click += new EventHandler(this.miHelpAbout_Click);
			this.gbISODetails.Controls.Add(this.tbIDDate);
			this.gbISODetails.Controls.Add(this.lblIDDate);
			this.gbISODetails.Controls.Add(this.tbIDDiscID);
			this.gbISODetails.Controls.Add(this.lblIDDiskID);
			this.gbISODetails.Controls.Add(this.tbIDMakerCode);
			this.gbISODetails.Controls.Add(this.lblIDMakerCode);
			this.gbISODetails.Controls.Add(this.tbIDRegion);
			this.gbISODetails.Controls.Add(this.lblIDRegion);
			this.gbISODetails.Controls.Add(this.tbIDGameCode);
			this.gbISODetails.Controls.Add(this.lblIDGameCode);
			this.gbISODetails.Controls.Add(this.tbIDName);
			this.gbISODetails.Controls.Add(this.lblIDName);
			this.gbISODetails.Location = new Point(7, 11);
			this.gbISODetails.Name = "gbISODetails";
			this.gbISODetails.Size = new Size(437, 103);
			this.gbISODetails.TabIndex = 10;
			this.gbISODetails.TabStop = false;
			this.gbISODetails.Text = "Image details";
			this.tbIDDate.BackColor = SystemColors.Control;
			this.tbIDDate.Enabled = false;
			this.tbIDDate.Location = new Point(321, 71);
			this.tbIDDate.Name = "tbIDDate";
			this.tbIDDate.ReadOnly = true;
			this.tbIDDate.Size = new Size(110, 20);
			this.tbIDDate.TabIndex = 15;
			this.lblIDDate.AutoSize = true;
			this.lblIDDate.Location = new Point(248, 75);
			this.lblIDDate.Margin = new Padding(3);
			this.lblIDDate.Name = "lblIDDate";
			this.lblIDDate.Size = new Size(33, 13);
			this.lblIDDate.TabIndex = 14;
			this.lblIDDate.Text = "Date:";
			this.tbIDDiscID.BackColor = SystemColors.Control;
			this.tbIDDiscID.Enabled = false;
			this.tbIDDiscID.Location = new Point(389, 19);
			this.tbIDDiscID.Name = "tbIDDiscID";
			this.tbIDDiscID.ReadOnly = true;
			this.tbIDDiscID.Size = new Size(42, 20);
			this.tbIDDiscID.TabIndex = 13;
			this.lblIDDiskID.AutoSize = true;
			this.lblIDDiskID.Location = new Point(316, 23);
			this.lblIDDiskID.Margin = new Padding(3);
			this.lblIDDiskID.Name = "lblIDDiskID";
			this.lblIDDiskID.Size = new Size(45, 13);
			this.lblIDDiskID.TabIndex = 12;
			this.lblIDDiskID.Text = "Disc ID:";
			this.tbIDMakerCode.BackColor = SystemColors.Control;
			this.tbIDMakerCode.Enabled = false;
			this.tbIDMakerCode.Location = new Point(79, 71);
			this.tbIDMakerCode.Name = "tbIDMakerCode";
			this.tbIDMakerCode.ReadOnly = true;
			this.tbIDMakerCode.Size = new Size(139, 20);
			this.tbIDMakerCode.TabIndex = 7;
			this.lblIDMakerCode.AutoSize = true;
			this.lblIDMakerCode.Location = new Point(6, 75);
			this.lblIDMakerCode.Margin = new Padding(3);
			this.lblIDMakerCode.Name = "lblIDMakerCode";
			this.lblIDMakerCode.Size = new Size(67, 13);
			this.lblIDMakerCode.TabIndex = 6;
			this.lblIDMakerCode.Text = "Maker code:";
			this.tbIDRegion.BackColor = SystemColors.Control;
			this.tbIDRegion.Enabled = false;
			this.tbIDRegion.Location = new Point(321, 45);
			this.tbIDRegion.Name = "tbIDRegion";
			this.tbIDRegion.ReadOnly = true;
			this.tbIDRegion.Size = new Size(110, 20);
			this.tbIDRegion.TabIndex = 5;
			this.lblIDRegion.AutoSize = true;
			this.lblIDRegion.Location = new Point(248, 48);
			this.lblIDRegion.Margin = new Padding(3);
			this.lblIDRegion.Name = "lblIDRegion";
			this.lblIDRegion.Size = new Size(44, 13);
			this.lblIDRegion.TabIndex = 4;
			this.lblIDRegion.Text = "Region:";
			this.tbIDGameCode.BackColor = SystemColors.Control;
			this.tbIDGameCode.Enabled = false;
			this.tbIDGameCode.Location = new Point(79, 45);
			this.tbIDGameCode.Name = "tbIDGameCode";
			this.tbIDGameCode.ReadOnly = true;
			this.tbIDGameCode.Size = new Size(139, 20);
			this.tbIDGameCode.TabIndex = 3;
			this.lblIDGameCode.AutoSize = true;
			this.lblIDGameCode.Location = new Point(6, 49);
			this.lblIDGameCode.Margin = new Padding(3);
			this.lblIDGameCode.Name = "lblIDGameCode";
			this.lblIDGameCode.Size = new Size(65, 13);
			this.lblIDGameCode.TabIndex = 2;
			this.lblIDGameCode.Text = "Game code:";
			this.tbIDName.BackColor = SystemColors.Control;
			this.tbIDName.Enabled = false;
			this.tbIDName.Location = new Point(79, 19);
			this.tbIDName.Name = "tbIDName";
			this.tbIDName.ReadOnly = true;
			this.tbIDName.Size = new Size(218, 20);
			this.tbIDName.TabIndex = 1;
			this.lblIDName.AutoSize = true;
			this.lblIDName.Location = new Point(6, 23);
			this.lblIDName.Margin = new Padding(3);
			this.lblIDName.Name = "lblIDName";
			this.lblIDName.Size = new Size(38, 13);
			this.lblIDName.TabIndex = 0;
			this.lblIDName.Text = "Name:";
			this.panel1.BorderStyle = BorderStyle.Fixed3D;
			this.panel1.Controls.Add(this.gbStruct);
			this.panel1.Controls.Add(this.gbISODetails);
			this.panel1.Controls.Add(this.gbBannerDetails);
			this.panel1.Location = new Point(-2, 27);
			this.panel1.Name = "panel1";
			this.panel1.Size = new Size(892, 482);
			this.panel1.TabIndex = 14;
			base.AutoScaleDimensions = new SizeF(6f, 13f);
			base.AutoScaleMode = AutoScaleMode.Font;
			base.ClientSize = new Size(886, 509);
			base.Controls.Add(this.statusStrip);
			base.Controls.Add(this.menuStrip);
			base.Controls.Add(this.panel1);
			base.FormBorderStyle = FormBorderStyle.FixedSingle;
			base.KeyPreview = true;
			base.MainMenuStrip = this.menuStrip;
			base.MaximizeBox = false;
			base.Name = "MainForm";
			this.Text = "GCRebuilder v1.0";
			base.Load += new EventHandler(this.MainForm_Load);
			base.KeyDown += new KeyEventHandler(this.MainForm_KeyDown);
			this.cmsTVTOC.ResumeLayout(false);
			this.gbStruct.ResumeLayout(false);
			this.gbStruct.PerformLayout();
			this.gbSort.ResumeLayout(false);
			this.gbSort.PerformLayout();
			this.statusStrip.ResumeLayout(false);
			this.statusStrip.PerformLayout();
			this.gbBannerDetails.ResumeLayout(false);
			this.gbBannerDetails.PerformLayout();
			((ISupportInitialize)this.pbBDBanner).EndInit();
			this.menuStrip.ResumeLayout(false);
			this.menuStrip.PerformLayout();
			this.gbISODetails.ResumeLayout(false);
			this.gbISODetails.PerformLayout();
			this.panel1.ResumeLayout(false);
			base.ResumeLayout(false);
			base.PerformLayout();
		}

		private void LoadInfo(bool image)
		{
			this.LoadISOInfo(image);
			this.GetBanners(image);
		}

		private void LoadISOInfo(bool image)
		{
			string path = image ? this.imgPath : this.toc.fils[2].path;
			FileStream fileStream = new FileStream(path, FileMode.Open, FileAccess.Read, FileShare.Read);
			BinaryReader binaryReader = new BinaryReader(fileStream, Encoding.Default);
			byte[] array = binaryReader.ReadBytes(4);
			this.tbIDGameCode.Text = SIOExtensions.ToStringC(Encoding.Default.GetChars(array));
			string text = Convert.ToString(Encoding.Default.GetChars(new byte[]
			{
				array[3]
			})[0]).ToLower();
			if (text != null)
			{
				if (text == "e")
				{
					this.tbIDRegion.Text = "USA/NTSC";
					this.region = 'u';
					goto IL_125;
				}
				if (text == "j")
				{
					this.tbIDRegion.Text = "JAP/NTSC";
					this.region = 'j';
					goto IL_125;
				}
				if (text == "p")
				{
					this.tbIDRegion.Text = "EUR/PAL";
					this.region = 'e';
					goto IL_125;
				}
			}
			this.tbIDRegion.Text = "UNK";
			this.region = 'n';
			IL_125:
			array = binaryReader.ReadBytes(2);
			this.tbIDMakerCode.Text = SIOExtensions.ToStringC(Encoding.Default.GetChars(array));
			byte b = binaryReader.ReadByte();
			this.tbIDDiscID.Text = string.Format("0x{0:x2}", b);
			fileStream.Position += 25L;
			this.tbIDName.Text = binaryReader.ReadStringNT();
			binaryReader.Close();
			fileStream.Close();
			path = (image ? this.imgPath : this.toc.fils[3].path);
			fileStream = new FileStream(path, FileMode.Open, FileAccess.Read, FileShare.Read);
			binaryReader = new BinaryReader(fileStream, Encoding.Default);
			if (image)
			{
				fileStream.Position = (long)this.toc.fils[3].pos;
			}
			this.tbIDDate.Text = binaryReader.ReadStringNT();
			binaryReader.Close();
			fileStream.Close();
			this.tbIDName.Enabled = true;
			this.tbIDDiscID.Enabled = true;
			this.tbIDGameCode.Enabled = true;
			this.tbIDRegion.Enabled = true;
			this.tbIDMakerCode.Enabled = true;
			this.tbIDDate.Enabled = true;
		}

		private void GetBanners(bool image)
		{
			int num = 0;
			string value = "opening";
			string value2 = ".bnr";
			string text = "";
			this.cbBDFile.Items.Clear();
			for (int i = 0; i < this.toc.fils.Count; i++)
			{
				if (!this.toc.fils[i].isDir && this.toc.fils[i].name.IndexOf(value) == 0 && this.toc.fils[i].name.LastIndexOf(value2) == this.toc.fils[i].name.Length - 4)
				{
					this.cbBDFile.Items.Add(this.toc.fils[i].name);
					text += string.Format("x{0:d2}{1:d6}", num, i);
					num++;
				}
			}
			if (num > 0)
			{
				this.cbBDFile.Tag = text;
				this.cbBDFile.Enabled = true;
				this.cbBDFile.SelectedIndex = 0;
			}
		}

		private void LoadBannerInfo(int fIdx, bool image)
		{
			byte[] array = new byte[4];
			bool flag = false;
			string text = (string)this.cbBDFile.Tag;
			fIdx = text.IndexOf(string.Format("x{0:d2}", fIdx));
			if (fIdx == -1)
			{
				System.Console.WriteLine("error: " + "Banner index not found");
			}
			else
			{
				fIdx = Convert.ToInt32(text.Substring(fIdx + 3, 6));
				if (!image)
				{
					FileInfo fileInfo = new FileInfo(this.toc.fils[fIdx].path);
					if (!fileInfo.Exists)
					{
						System.Console.WriteLine("error: " + string.Format("Banner '{0}' not found", this.toc.fils[fIdx].path));
						return;
					}
				}
				if (this.bnr != null)
				{
					this.bnrr.Close();
					this.bnrw.Close();
					this.bnr.Close();
				}
				string path = image ? this.imgPath : this.toc.fils[fIdx].path;
				FileStream fileStream = new FileStream(path, FileMode.Open, FileAccess.Read, FileShare.Read);
				BinaryReader binaryReader = new BinaryReader(fileStream, Encoding.Default);
				if (image && fileStream.Length < (long)(this.toc.fils[fIdx].pos + this.toc.fils[fIdx].len))
				{
					System.Console.WriteLine("error: " + string.Format("Banner '{0}' not found", this.toc.fils[fIdx].path));
				}
				else
				{
					if (image)
					{
						fileStream.Position = (long)this.toc.fils[fIdx].pos;
					}
					this.bnr = new MemoryStream(binaryReader.ReadBytes(this.toc.fils[fIdx].len));
					this.bnrr = new BinaryReader(this.bnr, Encoding.Default);
					this.bnrw = new BinaryWriter(this.bnr, Encoding.Default);
					binaryReader.Close();
					fileStream.Close();
					this.bnr.Read(array, 0, 4);
					this.tbBDVersion.Text = SIOExtensions.ToStringC(Encoding.Default.GetChars(array));
					if (this.tbBDVersion.Text.Length < 3)
					{
						flag = true;
					}
					if (!flag && this.tbBDVersion.Text.ToLower().Substring(0, 3) != "bnr")
					{
						flag = true;
					}
					if (flag)
					{
						this.bannerLoaded = false;
						this.cbBDLanguage.Enabled = false;
						this.tbBDVersion.Text = "";
						this.tbBDVersion.Enabled = false;
						this.tbBDShortName.Text = "";
						this.tbBDShortName.Enabled = false;
						this.tbBDShortMaker.Text = "";
						this.tbBDShortMaker.Enabled = false;
						this.tbBDLongName.Text = "";
						this.tbBDLongName.Enabled = false;
						this.tbBDLongMaker.Text = "";
						this.tbBDLongMaker.Enabled = false;
						this.tbBDDescription.Text = "";
						this.tbBDDescription.Enabled = false;
						this.btnBDImport.Enabled = false;
						this.btnBDExport.Enabled = false;
						this.btnBDSave.Enabled = false;
						this.img = null;
						this.pbBDBanner.Invalidate();
						System.Console.WriteLine("error: " + "This is not banner file");
					}
					else
					{
						this.tbBDVersion.Enabled = true;
						if (this.region == 'j')
						{
							this.bannerEnc = Encoding.GetEncoding(932);
						}
						else
						{
							this.bannerEnc = Encoding.GetEncoding(1252);
						}
						if (this.cbBDLanguage.Items.IndexOf("Japan") == 0)
						{
							this.cbBDLanguage.Items.RemoveAt(0);
						}
						if (this.region == 'j')
						{
							this.cbBDLanguage.Items.Insert(0, "Japan");
						}
						if (array[3] == 50)
						{
							if (this.cbBDLanguage.Items.IndexOf("Japan") == 0)
							{
								this.cbBDLanguage.Items.RemoveAt(0);
							}
							this.bannerEnc = Encoding.GetEncoding(1252);
							this.cbBDLanguage.Enabled = true;
						}
						else
						{
							this.cbBDLanguage.Enabled = false;
							this.LoadBannerLang(0);
						}
						if (this.cbBDLanguage.SelectedIndex == 0)
						{
							this.LoadBannerLang(0);
						}
						else
						{
							this.cbBDLanguage.SelectedIndex = 0;
						}
						this.LoadBannerImage();
					}
				}
			}
		}

		private void LoadBannerLang(int lIdx)
		{
			if (this.bnr != null)
			{
				this.bannerLoaded = false;
				int num = 6176 + lIdx * 320;
				this.bnr.Position = (long)num;
				byte[] bytes = this.bnrr.ReadBytes(32);
				this.tbBDShortName.Text = SIOExtensions.ToStringC(this.bannerEnc.GetChars(bytes));
				bytes = this.bnrr.ReadBytes(32);
				this.tbBDShortMaker.Text = SIOExtensions.ToStringC(this.bannerEnc.GetChars(bytes));
				bytes = this.bnrr.ReadBytes(64);
				this.tbBDLongName.Text = SIOExtensions.ToStringC(this.bannerEnc.GetChars(bytes));
				bytes = this.bnrr.ReadBytes(64);
				this.tbBDLongMaker.Text = SIOExtensions.ToStringC(this.bannerEnc.GetChars(bytes));
				bytes = this.bnrr.ReadBytes(128);
				this.tbBDDescription.Text = SIOExtensions.ToStringC(this.bannerEnc.GetChars(bytes));
				this.tbBDShortName.Enabled = true;
				this.tbBDShortMaker.Enabled = true;
				this.tbBDLongName.Enabled = true;
				this.tbBDLongMaker.Enabled = true;
				this.tbBDDescription.Enabled = true;
				this.bannerLoaded = true;
			}
		}

		private void LoadBannerImage()
		{
			int num = 4;
			int num2 = 4;
			int num3 = num * num2;
			int num4 = 0;
			int num5 = -1;
			int num6 = 24;
			int num7 = 8;
			int num8 = num6 * num7;
			int num9 = 0;
			int num10 = -1;
			int num11 = 54;
			int num12 = num7 * num;
			byte[] array = new byte[num11 + 6144];
			int num13 = num6 * num * 2;
			byte[] array2 = new byte[num13];
			int num14 = num13 * num7 * num2 + num11;
			Array.Copy(Resources.bmp15, array, Resources.bmp15.Length);
			this.bnr.Position = 32L;
			for (int i = 0; i < num8; i++)
			{
				if (i % num6 == 0)
				{
					num9 = 0;
					num10++;
				}
				int num15 = num9 * num * 2 + num10 * num2 * num13 + num11;
				for (int j = 0; j < num3; j++)
				{
					if (j % num == 0)
					{
						num4 = 0;
						num5++;
						if (num5 > 0)
						{
							num15 += num13;
						}
					}
					int num16 = this.bnrr.ReadInt16BE();
					if ((num16 & 32768) != 32768)
					{
						byte b = (byte)(num16 >> 12 & 15);
						byte b2 = (byte)(num16 >> 8 & 15);
						byte b3 = (byte)(num16 >> 4 & 15);
						byte b4 = (byte)(num16 & 15);
						num16 = ((int)b2 << 11 | (int)b3 << 6 | (int)b4 << 1);
					}
					else
					{
						num16 &= 32767;
					}
					array[num15 + num4 * 2] = (byte)(num16 & 255);
					array[num15 + num4 * 2 + 1] = (byte)(num16 >> 8);
					num4++;
				}
				num5 = -1;
				num9++;
			}
			for (int i = num12 / 2; i < num12; i++)
			{
				Array.Copy(array, i * num13 + num11, array2, 0, num13);
				Array.Copy(array, (num12 - i - 1) * num13 + num11, array, i * num13 + num11, num13);
				Array.Copy(array2, 0, array, (num12 - i - 1) * num13 + num11, num13);
			}
			this.img = Image.FromStream(new MemoryStream(array));
			this.btnBDImport.Enabled = true;
			this.btnBDExport.Enabled = true;
			this.btnBDSave.Enabled = true;
			this.pbBDBanner.Invalidate();
		}

		private void SaveBannerInfo(int fIdx, bool image)
		{
			FileStream fileStream = null;
			string text = (string)this.cbBDFile.Tag;
			fIdx = text.IndexOf(string.Format("x{0:d2}", fIdx));
			if (fIdx == -1)
			{
				System.Console.WriteLine("error: " + "Banner index not found");
			}
			else
			{
				fIdx = Convert.ToInt32(text.Substring(fIdx + 3, 6));
				if (!image)
				{
					FileInfo fileInfo = new FileInfo(this.toc.fils[fIdx].path);
					if (!fileInfo.Exists)
					{
						System.Console.WriteLine("error: " + string.Format("Banner '{0}' not found", this.toc.fils[fIdx].path));
						return;
					}
				}
				string path = image ? this.imgPath : this.toc.fils[fIdx].path;
				try
				{
					fileStream = new FileStream(path, FileMode.Open, FileAccess.Write, FileShare.None);
					if (image)
					{
						fileStream.Position = (long)this.toc.fils[fIdx].pos;
					}
					this.bnr.WriteTo(fileStream);
				}
				catch (Exception ex)
				{
					System.Console.WriteLine("error: " + this, ex.Message);
				}
				if (fileStream != null)
				{
					fileStream.Close();
				}
			}
		}

		private void SaveBannerLang(int lIdx)
		{
			if (this.bannerLoaded)
			{
				if (this.bnr != null)
				{
					int num = 6176 + lIdx * 320;
					this.bnr.Position = (long)num;
					this.bnrw.WriteStringNT(this.bannerEnc, this.tbBDShortName.Text, 32);
					this.bnrw.WriteStringNT(this.bannerEnc, this.tbBDShortMaker.Text, 32);
					this.bnrw.WriteStringNT(this.bannerEnc, this.tbBDLongName.Text, 64);
					this.bnrw.WriteStringNT(this.bannerEnc, this.tbBDLongMaker.Text, 64);
					this.bnrw.WriteStringNT(this.bannerEnc, this.tbBDDescription.Text, 128);
				}
			}
		}

		private void SaveBannerImage(string path)
		{
			int num = 4;
			int num2 = 4;
			int num3 = num * num2;
			int num4 = 0;
			int num5 = -1;
			int num6 = 24;
			int num7 = 8;
			int num8 = num6 * num7;
			int num9 = 0;
			int num10 = -1;
			int num11 = 54;
			int num12 = num7 * num;
			int num13 = num6 * num * 2;
			byte[] array = new byte[num13];
			int num14 = num13 * num7 * num2 + num11;
			FileStream fileStream = new FileStream(path, FileMode.Open, FileAccess.Read, FileShare.Read);
			BinaryReader binaryReader = new BinaryReader(fileStream, Encoding.Default);
			byte[] array2 = binaryReader.ReadBytes((int)fileStream.Length);
			binaryReader.Close();
			fileStream.Close();
			int i = ((int)array2[1] << 8) + (int)array2[0];
			if (i != 19778)
			{
				System.Console.WriteLine("error: " + "Not BMP");
			}
			else
			{
				i = ((int)array2[29] << 8) + (int)array2[28];
				int j = ((int)array2[31] << 8) + (int)array2[30];
				if (i != 16 || j != 0)
				{
					System.Console.WriteLine("error: " + "Wrong BMP");
				}
				else
				{
					i = ((int)array2[21] << 24) + ((int)array2[20] << 16) + ((int)array2[19] << 8) + (int)array2[18];
					j = ((int)array2[25] << 24) + ((int)array2[24] << 16) + ((int)array2[23] << 8) + (int)array2[22];
					if (i != 96 || j != 32)
					{
						System.Console.WriteLine("error: " + "Wrong width or height. Must be 96x32");
					}
					else
					{
						for (i = num12 / 2; i < num12; i++)
						{
							Array.Copy(array2, i * num13 + num11, array, 0, num13);
							Array.Copy(array2, (num12 - i - 1) * num13 + num11, array2, i * num13 + num11, num13);
							Array.Copy(array, 0, array2, (num12 - i - 1) * num13 + num11, num13);
						}
						this.bnr.Position = 32L;
						for (i = 0; i < num8; i++)
						{
							if (i % num6 == 0)
							{
								num9 = 0;
								num10++;
							}
							int num15 = num9 * num * 2 + num10 * num2 * num13 + num11;
							for (j = 0; j < num3; j++)
							{
								if (j % num == 0)
								{
									num4 = 0;
									num5++;
									if (num5 > 0)
									{
										num15 += num13;
									}
								}
								int num16 = (int)array2[num15 + num4 * 2];
								if (this.ignoreBannerAlpha)
								{
									num16 |= 32768;
								}
								num16 += (int)array2[num15 + num4 * 2 + 1] << 8;
								this.bnrw.WriteInt16BE(num16);
								num4++;
							}
							num5 = -1;
							num9++;
						}
						this.LoadBannerImage();
					}
				}
			}
		}
	}
}

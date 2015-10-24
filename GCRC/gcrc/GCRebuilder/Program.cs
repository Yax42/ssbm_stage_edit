using System;
using System.Windows.Forms;

namespace GCRebuilder
{
	internal static class Program
	{
		[STAThread]
		private static void Main(string[] args)
		{
			MainForm.args = args;
			//MainForm.args = new[] {"ssbm.iso", "GrTFc.dat", "my_res.dat"};
#if true
			var mf = new MainForm();
			mf.RunCommand();
#else
			Application.EnableVisualStyles();
			Application.SetCompatibleTextRenderingDefault(false);
			Application.Run(new MainForm());
#endif
		}
	}
}

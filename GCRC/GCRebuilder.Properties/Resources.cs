using System;
using System.CodeDom.Compiler;
using System.ComponentModel;
using System.Diagnostics;
using System.Drawing;
using System.Globalization;
using System.Resources;
using System.Runtime.CompilerServices;

namespace GCRebuilder.Properties
{
	[GeneratedCode("System.Resources.Tools.StronglyTypedResourceBuilder", "2.0.0.0"), DebuggerNonUserCode, CompilerGenerated]
	internal class Resources
	{
		private static ResourceManager resourceMan;

		private static CultureInfo resourceCulture;

		[EditorBrowsable(EditorBrowsableState.Advanced)]
		internal static ResourceManager ResourceManager
		{
			get
			{
				if (object.ReferenceEquals(Resources.resourceMan, null))
				{
					ResourceManager resourceManager = new ResourceManager("GCRebuilder.Properties.Resources", typeof(Resources).Assembly);
					Resources.resourceMan = resourceManager;
				}
				return Resources.resourceMan;
			}
		}

		[EditorBrowsable(EditorBrowsableState.Advanced)]
		internal static CultureInfo Culture
		{
			get
			{
				return Resources.resourceCulture;
			}
			set
			{
				Resources.resourceCulture = value;
			}
		}

		internal static Bitmap about
		{
			get
			{
				object @object = Resources.ResourceManager.GetObject("about", Resources.resourceCulture);
				return (Bitmap)@object;
			}
		}

		internal static Bitmap blank
		{
			get
			{
				object @object = Resources.ResourceManager.GetObject("blank", Resources.resourceCulture);
				return (Bitmap)@object;
			}
		}

		internal static byte[] bmp15
		{
			get
			{
				object @object = Resources.ResourceManager.GetObject("bmp15", Resources.resourceCulture);
				return (byte[])@object;
			}
		}

		internal static Bitmap dir_cl
		{
			get
			{
				object @object = Resources.ResourceManager.GetObject("dir_cl", Resources.resourceCulture);
				return (Bitmap)@object;
			}
		}

		internal static Bitmap dir_op
		{
			get
			{
				object @object = Resources.ResourceManager.GetObject("dir_op", Resources.resourceCulture);
				return (Bitmap)@object;
			}
		}

		internal static Bitmap down
		{
			get
			{
				object @object = Resources.ResourceManager.GetObject("down", Resources.resourceCulture);
				return (Bitmap)@object;
			}
		}

		internal static Bitmap exit
		{
			get
			{
				object @object = Resources.ResourceManager.GetObject("exit", Resources.resourceCulture);
				return (Bitmap)@object;
			}
		}

		internal static Bitmap fil
		{
			get
			{
				object @object = Resources.ResourceManager.GetObject("fil", Resources.resourceCulture);
				return (Bitmap)@object;
			}
		}

		internal static Icon ico
		{
			get
			{
				object @object = Resources.ResourceManager.GetObject("ico", Resources.resourceCulture);
				return (Icon)@object;
			}
		}

		internal static Bitmap img_close
		{
			get
			{
				object @object = Resources.ResourceManager.GetObject("img_close", Resources.resourceCulture);
				return (Bitmap)@object;
			}
		}

		internal static Bitmap img_open
		{
			get
			{
				object @object = Resources.ResourceManager.GetObject("img_open", Resources.resourceCulture);
				return (Bitmap)@object;
			}
		}

		internal static Bitmap root
		{
			get
			{
				object @object = Resources.ResourceManager.GetObject("root", Resources.resourceCulture);
				return (Bitmap)@object;
			}
		}

		internal static Bitmap root_close
		{
			get
			{
				object @object = Resources.ResourceManager.GetObject("root_close", Resources.resourceCulture);
				return (Bitmap)@object;
			}
		}

		internal static Bitmap root_open
		{
			get
			{
				object @object = Resources.ResourceManager.GetObject("root_open", Resources.resourceCulture);
				return (Bitmap)@object;
			}
		}

		internal static Bitmap root_run
		{
			get
			{
				object @object = Resources.ResourceManager.GetObject("root_run", Resources.resourceCulture);
				return (Bitmap)@object;
			}
		}

		internal static Bitmap root_save
		{
			get
			{
				object @object = Resources.ResourceManager.GetObject("root_save", Resources.resourceCulture);
				return (Bitmap)@object;
			}
		}

		internal static Bitmap root_stop
		{
			get
			{
				object @object = Resources.ResourceManager.GetObject("root_stop", Resources.resourceCulture);
				return (Bitmap)@object;
			}
		}

		internal static Bitmap up
		{
			get
			{
				object @object = Resources.ResourceManager.GetObject("up", Resources.resourceCulture);
				return (Bitmap)@object;
			}
		}

		internal static Bitmap wipe
		{
			get
			{
				object @object = Resources.ResourceManager.GetObject("wipe", Resources.resourceCulture);
				return (Bitmap)@object;
			}
		}

		internal Resources()
		{
		}
	}
}

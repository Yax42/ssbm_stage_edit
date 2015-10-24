using System;
using System.IO;
using System.Text;

namespace System.Runtime.CompilerServices
{
	[AttributeUsage(AttributeTargets.Assembly | AttributeTargets.Class
		 | AttributeTargets.Method)]
	public sealed class ExtensionAttribute : Attribute { }
}
public static class SIOExtensions
{
	private static int resI;

	private static int resH;

	private static string resS;

	private static int i;

	private static byte b;

	private static byte[] bb;

	public static int ReadInt32BE(this BinaryReader br)
	{
		SIOExtensions.i = (int)br.ReadByte();
		SIOExtensions.resI = SIOExtensions.i << 24;
		SIOExtensions.i = (int)br.ReadByte();
		SIOExtensions.resI += SIOExtensions.i << 16;
		SIOExtensions.i = (int)br.ReadByte();
		SIOExtensions.resI += SIOExtensions.i << 8;
		SIOExtensions.i = (int)br.ReadByte();
		SIOExtensions.resI += SIOExtensions.i;
		return SIOExtensions.resI;
	}

	public static void WriteInt32BE(this BinaryWriter bw, int val)
	{
		SIOExtensions.b = (byte)(val >> 24 & 255);
		bw.Write(SIOExtensions.b);
		SIOExtensions.b = (byte)(val >> 16 & 255);
		bw.Write(SIOExtensions.b);
		SIOExtensions.b = (byte)(val >> 8 & 255);
		bw.Write(SIOExtensions.b);
		SIOExtensions.b = (byte)(val & 255);
		bw.Write(SIOExtensions.b);
	}

	public static int ReadInt16BE(this BinaryReader br)
	{
		SIOExtensions.i = (int)br.ReadByte();
		SIOExtensions.resH = SIOExtensions.i << 8;
		SIOExtensions.i = (int)br.ReadByte();
		SIOExtensions.resH += SIOExtensions.i;
		return SIOExtensions.resH;
	}

	public static void WriteInt16BE(this BinaryWriter bw, int val)
	{
		SIOExtensions.b = (byte)(val >> 8 & 255);
		bw.Write(SIOExtensions.b);
		SIOExtensions.b = (byte)(val & 255);
		bw.Write(SIOExtensions.b);
	}

	public static string ReadStringNT(this BinaryReader br)
	{
		SIOExtensions.resS = "";
		SIOExtensions.b = br.ReadByte();
		while (SIOExtensions.b != 0)
		{
			SIOExtensions.resS += Encoding.Default.GetChars(new byte[]
			{
				SIOExtensions.b
			})[0];
			SIOExtensions.b = br.ReadByte();
		}
		return SIOExtensions.resS;
	}

	public static void WriteStringNT(this BinaryWriter bw, string s)
	{
		SIOExtensions.resI = s.Length;
		SIOExtensions.i = 0;
		while (SIOExtensions.i < SIOExtensions.resI)
		{
			SIOExtensions.b = Encoding.Default.GetBytes(new char[]
			{
				s[SIOExtensions.i]
			})[0];
			bw.Write(SIOExtensions.b);
			SIOExtensions.i++;
		}
		bw.Write(0);
	}

	public static void WriteStringNT(this BinaryWriter bw, Encoding enc, string s, int maxLen)
	{
		SIOExtensions.bb = enc.GetBytes(s.Replace("\r\n", "\n"));
		SIOExtensions.resI = SIOExtensions.bb.Length;
		SIOExtensions.i = 0;
		while (SIOExtensions.i < SIOExtensions.resI)
		{
			bw.Write(SIOExtensions.bb[SIOExtensions.i]);
			if (SIOExtensions.i == maxLen - 1)
			{
				SIOExtensions.resI = SIOExtensions.i;
				break;
			}
			SIOExtensions.i++;
		}
		SIOExtensions.i = SIOExtensions.resI;
		while (SIOExtensions.i < maxLen)
		{
			bw.Write(0);
			SIOExtensions.i++;
		}
	}

	public static string ToStringC(char[] chars)
	{
		SIOExtensions.resS = "";
		SIOExtensions.resH = chars.Length;
		for (int i = 0; i < SIOExtensions.resH; i++)
		{
			if (chars[i] == '\n')
			{
				SIOExtensions.resS += '\r';
			}
			SIOExtensions.resS += chars[i];
		}
		return SIOExtensions.resS;
	}
}

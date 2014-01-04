#pragma once

#include <iostream>
#include <bitset>
#include <sstream>

typedef unsigned short Ushort;
typedef unsigned char Byte;


class Math
{
public:
	template <typename T>
	static int	sgn(T val)
	{
	    return (val > T(0)) - (val < T(0));
	}

	template <typename T>
	static T	abs(T val)
	{
	    return (val > T(0)) ? val : -val;
	}

	template <typename T>
	static T	pow(T val, int time)
	{
		T	result = 1;
		for (int i = 0; i < time; i++)
			result *= val;
		return (result);
	}

	template <typename T>
	static int	smallest(T v1, T v2)
	{
	    return (v1 < v2) ? v1 : v2;
	}
	
	template <typename T>
	static int	biggest(T v1, T v2)
	{
	    return (v1 > v2) ? v1 : v2;
	}

	template <typename T>
	static std::string	binStr(T v)
	{
		union
		{
			T x;
			Byte y[sizeof(T)];
		}f;

		f.x = v;

		std::string	str = std::bitset<8>(f.y[0]).to_string();
		for (int i = 1; i < sizeof(T); i++)
		{
			str +=".";
			str += std::bitset<8>(f.y[i]).to_string();
		}
		return (str);
		
	}

	template <class T, class V>
	static std::string		toStr(V v, int id)
	{
		const int		nb = sizeof(V) / sizeof(T);
		union
		{
			V x;
			T y[nb];
		} f;
		f.x = v;
		std::stringstream	stream;

		stream << f.y[id];
		std::string str;
		stream >> str;
		return (str);
	}
private:
	Math();
	~Math();
};
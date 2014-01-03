#pragma once

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

private:
	Math();
	~Math();
};
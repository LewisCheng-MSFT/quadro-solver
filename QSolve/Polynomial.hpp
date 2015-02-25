#pragma once

#include "Config.hpp"

template <typename T>
class Polynomial
{
private:
	T coef_[3];

public:
	Polynomial<T>(void)
	{
		Polynomial<T>(0, 0, 0);
	}

	Polynomial<T>(T c0, T c1, T c2)
	{
		coef_[0] = c0;
		coef_[1] = c1;
		coef_[2] = c2;
	}

	Polynomial<T>(const Polynomial<T> &other)
	{
		CopyFrom(other);
	}

	~Polynomial<T>(void) { }

	T &operator[](int index) { return coef_[index]; }

	T operator[](int index) const { return coef_[index]; }

	const Polynomial<T> &operator=(const Polynomial<T> &other)
	{
		CopyFrom(other);
		return *this;
	}

	const Polynomial<T> &operator+=(const Polynomial<T> &rhs)
	{
		coef_[0] += rhs[0];
		coef_[1] += rhs[1];
		coef_[2] += rhs[2];

		return *this;
	}

	const Polynomial<T> &operator-=(const Polynomial<T> &rhs)
	{
		coef_[0] -= rhs[0];
		coef_[1] -= rhs[1];
		coef_[2] -= rhs[2];

		return *this;
	}

	const Polynomial<T> &operator*=(const Polynomial<T> &rhs)
	{
		// ×ö³Ë·¨
		T temp[3];
		temp[0] = coef_[0] * rhs[0];
		temp[1] = coef_[1] * rhs[0] + coef_[0] * rhs[1];
		temp[2] = coef_[2] * rhs[0] + coef_[0] * rhs[2] + coef_[1] * rhs[1];
	
		// ¸³Öµ
		coef_[0] = temp[0];
		coef_[1] = temp[1];
		coef_[2] = temp[2];

		return *this;
	}

	const Polynomial<T> &operator*=(T rhs)
	{
		coef_[0] *= rhs;
		coef_[1] *= rhs;
		coef_[2] *= rhs;

		return *this;
	}

	const Polynomial<T> &operator/=(const Polynomial<T> &rhs)
	{
		if (rhs[1] != 0 || rhs[2] != 0)
			throw std::exception("divisor not constant");
		coef_[0] /= rhs[0];
		coef_[1] /= rhs[0];
		coef_[2] /= rhs[0];

		return *this;
	}

	const Polynomial<T> &operator^=(int power)
	{
		Polynomial<T> multiplier = *this;

		*this = GetConst(1);
		for (int i = 0; i < power; ++i)
			(*this) *= multiplier;

		return *this;
	}
	
	static const Polynomial<T> &GetX()
	{
		static Polynomial<T> px(0, 1, 0);
		return px;
	}

	static const Polynomial<T> &GetConst(T c)
	{
		static Polynomial<T> pc;
		pc[0] = c;
		return pc;
	}

private:
	void CopyFrom(const Polynomial<T> &other)
	{
		coef_[0] = other[0];
		coef_[1] = other[1];
		coef_[2] = other[2];
	}
};

template <typename T>
std::ostream &operator<<(std::ostream &os, const Polynomial<T> &p)
{
	os << p[2] << "x^2 + " << p[1] << "x + " << p[0];
	return os;
}
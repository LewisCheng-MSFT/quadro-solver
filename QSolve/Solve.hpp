#pragma once

#include "Config.hpp"

template <typename T, bool V> struct SolveWrapper;

template <typename T>
struct SolveWrapper<T, false>
{
	static void Solve(const std::string &expr)
	{
		throw new std::exception("T should be arithmetic type");
	}
};

template <typename T>
struct SolveWrapper<T, true>
{
	static void Solve(const std::string &expr)
	{
		// �ʷ�����
		Lex<T> lex;
		lex.Scan(expr);
		
		// �﷨���������
		Parser<T> parser(lex);
		Polynomial<T> equation = parser.Parse();
		std::cout << "equation: " << equation << " = 0" << std::endl;

		// �ⷽ��
		T a = equation[2];
		T b = equation[1];
		T c = equation[0];

		if (a == 0)
		{ // һ�η���
			std::cout << "solution: x = " << -c / b << std::endl;
		}
		else
		{ // ���η���
			T delta = b * b - 4 * a * c;
			if (delta < 0)
			{
				throw new std::exception("no solution");
			}
			else
			{
				// ��ΪT���������ͣ�����һ����ת����double���͡�
				T x1 = (-b + sqrt((double)delta)) / (2 * a);
				T x2 = (-b - sqrt((double)delta)) / (2 * a);
				std::cout << "solution: x1 = " << x1 << ", x2 = " << x2 << std::endl;
			}
		}
	}
};

template <typename T>
void Solve(const std::string &expr)
{
	SolveWrapper<T, std::tr1::is_arithmetic<T>::value>::Solve(expr);
}
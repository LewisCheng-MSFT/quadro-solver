// qsolver.cpp : 定义控制台应用程序的入口点。

#include "stdafx.h"
#include "Config.hpp"

int _tmain(int argc, _TCHAR* argv[])
{
	std::vector<std::string> expressions;
	expressions.push_back("x+x^2+2(3/4+1)+x(x-1))=x"); // 有语法错误
	expressions.push_back("6+x^2-x(3/6-x+1)=54x+9x^2-7");
	expressions.push_back("x^2-1x+-6=0");
	expressions.push_back("x^2+2x+6=0.5x(x+5)+2^2*3");
	expressions.push_back("x=-3");
	expressions.push_back("x^2+5x+=0"); // 有语法错误
	
	for (int i = 0; i < (int)expressions.size(); ++i)
	{
		try
		{
			Solve<double>(expressions[i]);
		}
		catch (std::exception *e)
		{
			std::cout << "error: " << e->what() << std::endl;
		}
		std::cout << std::endl;
	}
	
	return 0;
}
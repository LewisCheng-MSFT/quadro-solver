#pragma once

#include "Config.hpp"

template <typename T>
class Parser
{
private:
	Lex<T> &lex_;

public:
	Parser<T>(Lex<T> &lex)
		: lex_(lex) { }

	~Parser<T>(void) { }

	Polynomial<T> Parse()
	{
		Polynomial<T> result = ParsePolynomial();

		Token<T> eof = lex_.NextToken();;
		if (!lex_.Empty() || eof.GetType() != Token<T>::TOK_EOF)
			throw new std::exception("unexpected token");

		return result;
	}

private:
	Polynomial<T> ParsePolynomial()
	{
		Polynomial<T> opr1 = ParseTerm();
		Polynomial<T> opr2;
		Token<T> opt;

		while (true)
		{
			opt = lex_.NextToken();
			switch (opt.GetType())
			{
			case Token<T>::TOK_PLUS:
				opr2 = ParseTerm();
				opr1 += opr2;
				continue;

			case Token<T>::TOK_MINUS:
				opr2 = ParseTerm();
				opr1 -= opr2;
				continue;

			default:
				lex_.PutBack(opt);
				break;
			}
			break;
		}

		return opr1;
	}

	Polynomial<T> ParseTerm()
	{
		Polynomial<T> opr1 = ParseFactor();
		Polynomial<T> opr2;
		Token<T> opt;

		while (true)
		{
			opt = lex_.NextToken();
			switch (opt.GetType())
			{
			case Token<T>::TOK_MULTIPLY:
				opr2 = ParseFactor();
				opr1 *= opr2;
				continue;

			case Token<T>::TOK_DIVIDE:
				opr2 = ParseFactor();
				opr1 /= opr2;
				continue;

			case Token<T>::TOK_NUMBER:
			case Token<T>::TOK_X:
			case Token<T>::TOK_LPAREN:
				lex_.PutBack(opt);
				opr2 = ParseFactor();
				opr1 *= opr2;
				continue;

			default:
				lex_.PutBack(opt);
				break;
			}
			break;
		}

		return opr1;
	}

	Polynomial<T> ParseFactor()
	{
		Polynomial<T> result;

		Token<T> first = lex_.NextToken();
		Token<T> look_ahead;

		switch (first.GetType())
		{
		case Token<T>::TOK_PLUS:
			result = ParseXorNumber();
			break;

		case Token<T>::TOK_MINUS:
			result = ParseXorNumber();
			result *= -1;
			break;

		case Token<T>::TOK_LPAREN:
			result = ParsePolynomial();
			look_ahead = lex_.NextToken();
			if (look_ahead.GetType() != Token<T>::TOK_RPAREN)
				throw new std::exception("parenthesis mismatch");
			break;

		case Token<T>::TOK_NUMBER:
		case Token<T>::TOK_X:
			lex_.PutBack(first);
			result = ParseXorNumber();
			break;

		default:
			throw new std::exception("unexpected token");
			break;
		}

		return result;
	}

	Polynomial<T> ParseXorNumber()
	{
		Polynomial<T> result;

		Token<T> first = lex_.NextToken();

		if (first.GetType() == Token<T>::TOK_X)
			result = Polynomial<T>::GetX();
		else if (first.GetType() == Token<T>::TOK_NUMBER)
			result = Polynomial<T>::GetConst(first.GetValue());
		else
			throw new std::exception("operand expected");

		Token<T> look_ahead = lex_.NextToken();
		if (look_ahead.GetType() == Token<T>::TOK_POWER)
		{ // –Ë“™º∆À„≥À√›
			Token<T> number = lex_.NextToken();
			if (number.GetType() != Token<T>::TOK_NUMBER)
				throw new std::exception("power must be number");
			result ^= (int)number.GetValue();
		}
		else
		{
			lex_.PutBack(look_ahead);
		}

		return result;
	}
};
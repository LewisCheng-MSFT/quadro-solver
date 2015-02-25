#pragma once

#include "Config.hpp"

template <typename T>
class Lex
{
private:
	std::deque<Token<T> > token_stream_;
	std::string text_;
	int text_index_;
	bool equal_encountered_;

public:
	Lex<T>(void) { }

	~Lex<T>(void) { }

	void Scan(const std::string &text)
	{
		// 添加文法结束符
		text_ = text + "$";
		text_index_ = 0;
		equal_encountered_ = false;

		Token<T> current;
		while (true)
		{
			current = ScanSingleToken();
			if (current.GetType() == Token<T>::TOK_EQUAL)
			{ // '=' 变成 '-('
				// 多余=号
				if (equal_encountered_)
					throw new std::exception("more than two operator =");
				equal_encountered_ = true;
				token_stream_.push_back(Token<T>::TOK_MINUS);
				token_stream_.push_back(Token<T>::TOK_LPAREN);
			}
			else if (current.GetType() == Token<T>::TOK_EOF)
			{
				if (equal_encountered_)
					token_stream_.push_back(Token<T>::TOK_RPAREN);
				token_stream_.push_back(Token<T>::TOK_EOF);
				break;
			}
			else if (current.GetType() == Token<T>::TOK_NULL)
			{
				throw new std::exception("illegal character");
			}
			else
			{
				token_stream_.push_back(current);
			}
		}

		// 没有=号
		if (!equal_encountered_)
			throw new std::exception("operator = not found");
	}

	bool Empty() { return token_stream_.empty(); }

	Token<T> NextToken()
	{
		if (token_stream_.empty())
			throw new std::exception("token expected");
		Token<T> result = token_stream_.front();
		token_stream_.pop_front();
		return result;
	}

	void PutBack(const Token<T> &token)
	{
		token_stream_.push_front(token);
	}

private:
	T ScanNumber()
	{
		T value = 0;

		char ch = text_[text_index_];
		while (isdigit(ch))
		{
			value *= 10;
			value += ch - '0';
			ch = text_[++text_index_];
		}
		if (ch == '.')
		{
			// 匹配'.'
			ch = text_[++text_index_];

			T divisor = 1;
			while (isdigit(ch))
			{
				value *= 10;
				value += ch - '0';
				divisor *= 10;
				ch = text_[++text_index_];
			}
			value /= divisor;
		}

		return value;
	}

	Token<T> ScanSingleToken()
	{
		Token<T> result;

		char ch = text_[text_index_];
		// 跳过空白字符
		while (!isgraph(ch))
			ch = text_[++text_index_];

		switch (ch)
		{
		case '$':
			result.SetType(Token<T>::TOK_EOF);
			++text_index_;
			break;

		case '(':
			result.SetType(Token<T>::TOK_LPAREN);
			++text_index_;
			break;

		case ')':
			result.SetType(Token<T>::TOK_RPAREN);
			++text_index_;
			break;

		case '+':
			result.SetType(Token<T>::TOK_PLUS);
			++text_index_;
			break;

		case '-':
			result.SetType(Token<T>::TOK_MINUS);
			++text_index_;
			break;

		case '*':
			result.SetType(Token<T>::TOK_MULTIPLY);
			++text_index_;
			break;

		case '/':
			result.SetType(Token<T>::TOK_DIVIDE);
			++text_index_;
			break;

		case '^':
			result.SetType(Token<T>::TOK_POWER);
			++text_index_;
			break;

		case '=':
			result.SetType(Token<T>::TOK_EQUAL);
			++text_index_;
			break;

		case 'x':
		case 'X':
			result.SetType(Token<T>::TOK_X);
			++text_index_;
			break;

		default:
			if (isdigit(ch))
			{
				result.SetType(Token<T>::TOK_NUMBER);
				result.SetValue(ScanNumber());
			}
			else
			{ // 什么都不匹配，非法字符。
				result.SetType(Token<T>::TOK_NULL);
			}
			break;
		}

		return result;
	}
};


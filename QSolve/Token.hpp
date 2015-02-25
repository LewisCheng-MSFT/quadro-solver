#pragma once

#include "Config.hpp"

template <typename T>
class Token
{
public:
	enum {
		TOK_NULL,
		TOK_LPAREN,
		TOK_RPAREN,
		TOK_PLUS,
		TOK_MINUS,
		TOK_MULTIPLY,
		TOK_DIVIDE,
		TOK_POWER,
		TOK_EQUAL,
		TOK_X,
		TOK_NUMBER,
		TOK_EOF
	};

private:
	int type_;
	T value_;

public:
	Token<T>(void)
		: type_(TOK_NULL), value_(.0) { }

	Token<T>(int type)
		: type_(type), value_(.0) { }

	Token<T>(int type, T value)
		: type_(type), value_(value) { }

	~Token<T>(void) { }

	void SetType(int type) { type_ = type; }

	int GetType() const { return type_; }

	void SetValue(T value) { value_ = value; }

	T GetValue() const { return value_; }
};

template <typename T>
std::ostream &operator<<(std::ostream &os, const Token<T> &token)
{
	os << "token: " << token.GetType() << ", " << token.GetValue();
	return os;
}
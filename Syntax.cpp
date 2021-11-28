#include "Syntax.h"
#include<regex>

using namespace std;

typeE Syntax::getType(const char c) throw(SyntaxException)
{
	if (operators.count(c) != 0) // if c is in operators keys
		return typeE::OPERATOR;

	if (parentheses.count(c) != 0)	// if c is in parentheses set
	{
		if (open_parentheses.count(c) != 0)
			return typeE::OPEN_PARENTHESES;
		else
			return typeE::CLOSE_PARENTESES;
	}

	if ((int(c) >= int('a') && int(c) <= int('z')) || (int(c) >= int('A') && int(c) <= int('Z')))	// if c is a-z or A-Z
		return typeE::ID_OPERAND;

	if (int(c) >= int('0') && int(c) <= int('9'))	// if c is 0-9
		return typeE::NUM_OPERAND;
}

void Syntax::checkSyntax(const string& infix_expression) throw(SyntaxException)
{
	// check syntax using regex
	string id = "[a-zA-Z]";	// single char
	string num = "[1-9][0-9]*";	// may be multiple digits, first != 0
	string op = "[+^*/-]";

	// expressions with id operands
	// <id><op><id> | '('<id><op><id>')'
	string basic_expr = "((" + id + op + id + ")|(" + "\\(" + id + op + id + "\\)))";	
	string continue_expr = "(" + op + id + ")*";
	// (<op><id>)*(<op>'('<op><id>(<op><id>)*<op><id>')')*
	continue_expr = "(" + continue_expr + "(" + op + "\\(" + id + continue_expr + "(" + op + id + "\\)))*" + continue_expr + ")*";

	string id_expr = basic_expr + continue_expr;

	// expressions with num operands
	basic_expr = "((" + num + op + num + ")|(" + "\\(" + num + op + num + "\\)))";
	continue_expr = "(" + op + num + ")*";
	continue_expr = "(" + continue_expr + "(" + op + "\\(" + num + continue_expr + "(" + op + num + "\\)))*" + continue_expr + ")*";

	string num_expr = basic_expr + continue_expr;

	// valid expression contains only id operands or only num operands
	string expr = id_expr + "|" + num_expr;

	regex reg(expr);
	smatch base_match;

	// if expression doesn't match regex
	if (!regex_match(infix_expression, base_match, reg))
		throw SyntaxException();
}

// check if char other is a matching parentheses of char c
bool Syntax::match(const char& c, const char& other) throw(SyntaxException)
{
	// can't apply method if c is not a parentheses
	if (getType(c) != OPEN_PARENTHESES && getType(c) != CLOSE_PARENTESES)
		throw SyntaxException("Invalid matching for non-parentheses type " + string(1,c));

	if (matches.at(c) == other)
		return true;
	return false;
}

// get association of op
int Syntax::getAssociation(const char& c) throw(SyntaxException)
{
	// can't apply method if c is not an op
	if (getType(c) != OPERATOR)
		throw SyntaxException("Invalid accosiation for non-operator type " + string(1, c));

	if (c == '^')	// only ^ is from right to left
		return RL;
	return LR;
}

// check which operation has higher priority and return it. if equal return NULL ('\0')
char Syntax::higherPriority(const char& c, const char& other) throw(SyntaxException)
{
	if (getType(c) != OPERATOR || getType(other) != OPERATOR)
		throw SyntaxException("Invalid priority comparison for at least one non-operator: " + string(1, c) + " or " + string(1, other));

	if (operators.at(c) > operators.at(other))
		return c;
	if (operators.at(c) < operators.at(other))
		return other;
	return NULL;
}
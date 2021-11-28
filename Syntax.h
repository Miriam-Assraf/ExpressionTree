#include<iostream>
#include<string>
#include<unordered_map>
#include<unordered_set>

// define constants for operation association (LR=left to right, RL=right to left)
#define LR 0
#define RL 1

using namespace std;

static const unordered_map<char, int> operators{ {'+',0 }, {'-',0}, {'*',1}, {'/',1}, {'^',2} };	// map operators to priority
static const unordered_set<char> parentheses{ '(','[','{',')',']','}' };
static const unordered_set<char> open_parentheses{ '(','[','{' };

static const unordered_map<char, char> matches{ {'(',')'},{'[',']'},{'{','}'},{')','('},{']','['},{'}','{'} };	// map parentheses for matching

enum typeE { ID_OPERAND, NUM_OPERAND, OPERATOR, OPEN_PARENTHESES, CLOSE_PARENTESES };	// valid char types

class Syntax
{
private:
	Syntax() {};

public:
	static void checkSyntax(const string& infix_expression);
	static typeE getType(const char c);
	static bool match(const char& c, const char& other);
	static int getAssociation(const char& c);
	static char higherPriority(const char& c, const char& other);
};

class SyntaxException
{
private:
	string msg;
public:
	SyntaxException() {};
	SyntaxException(const string& msg) : msg(msg) {};
	virtual void errMsg() const 
	{ 
		if (msg.empty())
			cout << "Invalid syntax." << endl;
		else
		{
			cout << "Invalid syntax:\t";
			cout << msg << endl;
		}
	};
};
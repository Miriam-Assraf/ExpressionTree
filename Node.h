#include<string>
#include "Syntax.h"

using namespace std;

class ExpressionTree;

class Node
{
private:
	string val;
	typeE type;
	Node* parent;
	Node* left;
	Node* right;

	Node(const Node& other) : val(other.val), type(other.type), parent(other.parent), left(other.left), right(other.right) {};

public:
	friend class ExpressionTree;
	Node(const string& val) : val(val), left(nullptr), right(nullptr), parent(nullptr)
	{
		type = Syntax::getType(val[0]);
	};

	const string getVal() const { return val; };
	const Node* getLeft() const { return left; };
	const Node* getRight() const { return right; };
};
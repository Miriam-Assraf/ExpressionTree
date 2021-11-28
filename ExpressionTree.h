#include<string>
#include<vector>
#include<stack>

using namespace std;

enum orderE {INORDER, PREORDER, POSTORDER};	// traversal orders

class Node;

class ExpressionTree
{
private:
	Node* root;
	bool postfix;
	ExpressionTree();

	vector<string> toPostfix(const string& infix_expression);
	vector<string> toPrefix(const string& infix_expression);
	void addChar(const char c, vector<string>& newExpression, stack<char>& s);

	Node* createTree(const vector<string>& expression);
	void addNode(Node& node, stack<Node*>& s);
	void inorderTraversal(ostream& os, const Node* root) const;
	void preorderTraversal(const Node* root) const;
	void postorderTraversal(const Node* root) const;

public:
	ExpressionTree(const char* infix_expression, bool postfix=true);	// create postfix tree by default, if false create prefix tree
	const Node* getTree() const { return root; };

	void print(const orderE& order) const;	// print by chosen format (by traversal in given order)
	friend ostream& operator<<(ostream& os, const ExpressionTree tree);	// ostream to print infix format
};
#include<iostream>
#include<string>
#include<stack>
#include <algorithm>
#include "ExpressionTree.h"
#include "Node.h"

using namespace std;

ExpressionTree::ExpressionTree(const char* infix_expression, bool postfix) : postfix(postfix)
{
	if (infix_expression == nullptr)
	{
		root = nullptr;
		return;
	}

	Syntax::checkSyntax(infix_expression);

	vector<string> expression;
	string infix_str(infix_expression);

	if (postfix)
		expression = toPostfix(infix_str);
	else
		expression = toPrefix(infix_str);

	root = createTree(expression);
}

vector<string> ExpressionTree::toPostfix(const string& infix_expression)
{
	vector<string> postfixExpression;
	stack<char> s;

	// from left to right
	for (int i = 0; i < infix_expression.size(); ++i)
	{	
		string var;
		if (Syntax::getType(infix_expression[i]) == ID_OPERAND)
		{
			var.push_back(infix_expression[i]);
			postfixExpression.push_back(var);
		}
		else if (Syntax::getType(infix_expression[i]) == NUM_OPERAND)
		{
			while (Syntax::getType(infix_expression[i]) == NUM_OPERAND)
			{
				var.push_back(infix_expression[i]);
				++i;
			}
			postfixExpression.push_back(var);
			addChar(infix_expression[i], postfixExpression, s);
		}

		else
			addChar(infix_expression[i], postfixExpression, s);
	}

	while(!s.empty())
	{
		string var;
		var.push_back(s.top());
		postfixExpression.push_back(var);
		s.pop();
	}
	
	return postfixExpression;
}

vector<string> ExpressionTree::toPrefix(const string& infix_expression)
{
	vector<string> prefixExpression;
	stack<char> s;

	// from left to right
	for (int i = infix_expression.size()-1; i >= 0; --i)
	{
		string var;
		if (Syntax::getType(infix_expression[i]) == ID_OPERAND)
		{
			var.push_back(infix_expression[i]);
			prefixExpression.push_back(var);
		}
		else if (Syntax::getType(infix_expression[i]) == NUM_OPERAND)
		{
			while (Syntax::getType(infix_expression[i]) == NUM_OPERAND)
			{
				var.push_back(infix_expression[i]);
				--i;
			}
			prefixExpression.push_back(var);
			addChar(infix_expression[i], prefixExpression, s);
		}

		else
			addChar(infix_expression[i], prefixExpression, s);
	}

	while (!s.empty())
	{
		string var;
		var.push_back(s.top());
		prefixExpression.push_back(var);
		s.pop();
	}

	reverse(prefixExpression.begin(), prefixExpression.end());
	
	return prefixExpression;
}

void ExpressionTree::addChar(const char c, vector<string>& newExpression, stack<char>& s)
{
	typeE type = Syntax::getType(c);

	if (type == OPEN_PARENTHESES)
	{
		if (postfix)
			s.push(c);
		else
		{
			if (s.empty())  // stopped because stack is empty- match not found
				throw "Invalid syntax.";

			while (!s.empty() && !Syntax::match(c, s.top()))
			{
				newExpression.push_back(string(1, s.top()));
				s.pop();
			}
			s.pop();
		}
	}

	else if (type == CLOSE_PARENTESES)
	{
		if (postfix)
		{
			if (s.empty())  // stopped because stack is empty- match not found
				throw "Invalid syntax.";

			while (!s.empty() && !Syntax::match(c, s.top()))
			{
				newExpression.push_back(string(1, s.top()));
				s.pop();
			}
			s.pop();
		}
		else
			s.push(c);
	}

	else if (type == OPERATOR)
	{
		if (!s.empty())
		{
			typeE topOpType = Syntax::getType(s.top());

			if (topOpType != OPEN_PARENTHESES && topOpType != CLOSE_PARENTESES)
			{
				char higher_priority_op = Syntax::higherPriority(c, s.top());

				while (!s.empty() && topOpType != OPEN_PARENTHESES && topOpType != CLOSE_PARENTESES && higher_priority_op == s.top())  // top of stack with higher priority
				{
					newExpression.push_back(string(1, s.top()));
					s.pop();
					if (!s.empty())
					{
						higher_priority_op = Syntax::higherPriority(c, s.top());
						topOpType = Syntax::getType(s.top());
					}
				}

				if (!s.empty() && topOpType != OPEN_PARENTHESES && topOpType != CLOSE_PARENTESES)	// c has equal or higher preference of top operator
				{
					if (higher_priority_op == c)	// c has higher
						s.push(c);

					else // both with same priority
					{
						if (Syntax::getAssociation(c) == LR)
						{
							if (postfix)
							{
								newExpression.push_back(string(1, s.top()));
								s.pop();
								addChar(c, newExpression, s);  // recurively call add
							}
							else
								s.push(c);
						}
						else  // c associative is RL
						{
							if (postfix)
								s.push(c);
							else
							{
								newExpression.push_back(string(1, s.top()));
								s.pop();
								addChar(c, newExpression, s);  // recurively call add
							}
						}

					}
				}

			}
		}

		if (s.empty() || Syntax::getType(s.top()) == OPEN_PARENTHESES || Syntax::getType(s.top()) == CLOSE_PARENTESES)
			s.push(c);
	}
}

Node* ExpressionTree::createTree(const vector<string>& expression)
{
	stack<Node*> s;

	for (int i = 0; i < expression.size(); ++i)
	{
		Node* node = new Node(expression[i]);
		addNode(*node, s);
	}

	while (s.size()>1)
	{
		Node* subTree = s.top();
		s.pop();

		if (s.top()->left == nullptr)  // always build first the left subtree
			s.top()->left = subTree;
		else
			s.top()->right = subTree;

		subTree->parent = s.top();
	}

	Node* root = s.top();
	s.pop();
	return root;
}

void ExpressionTree::addNode(Node& node, stack<Node*>& s)
{
	if (postfix)
	{
		if (node.type == ID_OPERAND || node.type == NUM_OPERAND)
			s.push(&node);
		else
		{
			Node* node1 = s.top();
			s.pop();
			Node* node2 = s.top();
			s.pop();
			node.right = node1;
			node.left = node2;

			node1->parent = &node;
			node2->parent = &node;

			s.push(&node);
		}
	}

	else
	{
		if (node.type == OPERATOR)
			s.push(&node);

		else
		{
			Node* subTree = s.top();
			s.pop();

			// iteratively search for node doesn't have at least one child
			while (subTree->left != nullptr && subTree->right != nullptr)
			{
				Node* tmp = subTree;
				subTree = s.top();
				s.pop();

				if (subTree->left == nullptr)  // always build first the left subtree
					subTree->left = tmp;
				else
					subTree->right = tmp;

				tmp->parent = subTree;
			}

			// at least one child doesn't exist
			if (subTree->left == nullptr)  // always build first the left subtree
				subTree->left = &node;
			else
				subTree->right = &node;

			node.parent = subTree;
			s.push(subTree);
		}
	}
}

void ExpressionTree::preorderTraversal(const Node* root) const
{
	if (root == nullptr)  // base case - leaves
		return;

	cout << root->val;
	preorderTraversal(root->left);
	preorderTraversal(root->right);
}

void ExpressionTree::postorderTraversal(const Node* root) const
{
	if (root == nullptr)  // base case - leaves
		return;

	postorderTraversal(root->left);
	postorderTraversal(root->right);
	cout << root->val;
}

void ExpressionTree::print(const orderE& order) const
{
	switch (order)
	{
	case INORDER:
		cout << *this;
		break;
	case PREORDER:
		preorderTraversal(root);
		break;
	case POSTORDER:
		postorderTraversal(root);
		break;
	default:
		cout << "Invalid order input." << endl;
	}
}

void ExpressionTree::inorderTraversal(ostream& os, const Node* root) const
{
	if (root == nullptr)  // base case - leaves
		return;

	inorderTraversal(os, root->left);
	os << root->val;
	inorderTraversal(os, root->right);
}

ostream& operator<<(ostream& os, const ExpressionTree tree)
{
	tree.inorderTraversal(os, tree.root);
	return os;
}

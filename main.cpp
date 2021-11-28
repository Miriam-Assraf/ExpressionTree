#include<stdio.h>
#include<iostream>
#include<string>
#include "ExpressionTree.h"
#include "Syntax.h"

using namespace std;

void createTree(const char* expression, bool postfix=true);

int main()
{
	createTree("K+L-M*N+(O^P)*W/U/V*T+Q^J^A");
	createTree("(12+4)*2^3/2");
	createTree("a+b*(c/d)^e", false);
	createTree("ab1+d*)");

	return 0;
}

void createTree(const char* expression, bool postfix)
{
	try
	{
		ExpressionTree tree(expression, postfix);
		cout << "Infix expression: " << tree << endl;

		cout << "Prefix expression: ";
		tree.print(PREORDER);
		cout << '\n';

		cout << "Postfix expression: ";
		tree.print(POSTORDER);
		cout << '\n';
		cout << "---------------------------------------------------------" << endl;
	}
	catch (SyntaxException& e)
	{
		e.errMsg();
	}
}
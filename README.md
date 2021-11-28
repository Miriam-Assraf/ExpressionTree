# ExpressionTree
Syntax: </br>
•	Supports infix expressions with single char or numbers (sequences of digits) operands.</br>
•	Supports +,-,\*,/,^ operands</br>
•	Supports (),[],{} parentheses</br></br>
Examples:</br>
•	A+B\*(C/D)^e is valid</br>
•	20/2+(11-3) is valid</br>
•	A+2\*B is not valid</br>
•	AB+D is not valid</br>
•	A(C+d is not valid</br></br>

Syntax of infix expression is checked using regex:</br>
•	<id> :: [a-zA-Z]	(a single char)</br>
•	<num> :: [1-9][0-9]\*	(at least one digit)</br>
•	<op> :: [+^\*/-]</br>
•	<expr> :: <id_expr> | <num_expr></br>
•	<id_expr> :: <base_expr> <continue_expr> | ‘(‘ <base_expr> ‘)’ <continue_expr></br>
•	< base_expr> :: \<id> \<op> \<id></br>
•	<continue_expr> :: (\<op> \<id>)\* (\<op> ‘(‘ \<id> (\<op> \<id>)* \<op> \<id> ‘)’ )\* <continue_expr></br>
•	<num_expr> defined the same as <id_expr> but with \<num> instead of \<id></br></br>

Expression Tree:</br>
•	Generates expression tree for a given infix expression by converting it to postfix expression (by default) or prefix expression (if postfix Boolean is set to false)</br>
•	Supports operator<<, which print infix expression (inorder traversal of expression tree)</br>
•	Supports print function by passing enum type as argument for INORDER,PREORDER,POSTORDER and prints values by traversing expression tree in required order</br></br>

Expression Tree generation:</br>
•	Conversion of infix expression to postfix, or to prefix if defining postfix=false:</br>
Iterate infix expression (forward to postfix or backwards to prefix)</br>
&emsp;o	If char is an operand- push it to stack (for number operand loop until not a number operand, because a number may contain</br>&emsp;&ensp; more than one digit, and push the sequence of digits as a string)</br>
&emsp;o	If char is open parentheses- for postfix push it to stack, for prefix push top element of stack to the new expression and pop it</br>&emsp;&ensp; from stack until a matching parentheses is found</br>
&emsp;o	If char is closing parentheses- for postfix push top element of stack to the new expression and pop it from stack </br>&emsp;&ensp; until a matching parentheses is found, for prefix push it to stack</br>
&emsp;o	If operand- compare priority with operator on top of stack</br>
&emsp;&emsp;*	While operator on top of stack has higher priority- keep pushing it to the new expression </br>&emsp;&emsp;&ensp; and pop it from stack (until stack is empty or has equal/lower propriety to current char operator)</br>
&emsp;&emsp;*	If current operator has higher priority or stack is empty or top of stack is a parentheses- push to stack</br>
&emsp;&emsp;*	Else (both have same priority)- check current  operator associative</br>
&emsp;&emsp;*	If left-to-right- for postfix keep pushing it to the new expression and pop it from stack </br>&emsp;&emsp;&ensp; (recursively try to add current operator), for prefix push it to stack</br>
&emsp;&emsp;*	If right-to-left- for postfix push it to stack, for prefix keep pushing it to the new expression and pop it from stack </br>&emsp;&emsp;&ensp; (recursively try to add current operator)</br>
&emsp;o	If prefix- reverse expression</br></br>

•	Generate tree using postfix/prefix expression:</br>
Iterate over expression</br>
&emsp;o	For postfix expression:</br>
&emsp;&emsp;*	If operand- push it to stack</br>
&emsp;&emsp;*	If operator- define it’s right child the node on top of stack and pop it, and define it’s left child the new top of stack </br>&emsp;&ensp; and pop it too, then push the operator (subtree) to stack</br>
&emsp;o	For prefix expression:</br>
&emsp;&emsp;*	If operator- push it to stack</br>
&emsp;&emsp;*	If operand- get top of stack as root of a new subtree</br>
&emsp;&emsp;*	While root has both children iteratively search for a node that doesn’t have at least one child- </br>&emsp;&emsp;&ensp; update new top of stack as root, pop it, and add previous root to it</br>
&emsp;&emsp;*	Root is missing at least one child- if left is null, define as left child (build left subtree first), else define as right child</br>
&emsp;o	Finally while stack is not empty- keep adding subtrees iteratively</br>

As we can see, building an expression tree of a postfix expression requires a linear iteration over the postfix expression,</br> while building of a prefix expression also requires merging subtrees, where at most we’ll have iterative calls for merging as the number of operator in expression.</br>
Therefore the complexity is:</br>
•	Using postfix expression – O(n), where n is the length of the postfix expression</br>
•	Using prefix expression – O(n*m), where n is the length and m is the number the operators of the prefix expression</br>


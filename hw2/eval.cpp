#include <iostream>
#include <string>
#include <stack>
#include <cassert>
using namespace std;

bool convert(string infix, string& postfix);

// Evaluates a boolean expression
// Postcondition: If infix is a syntactically valid infix boolean
//   expression, then postfix is set to the postfix form of that
//   expression, result is set to the value of the expression (where
//   in that expression, each digit k represents element k of the
//   values array), and the function returns zero.  If infix is not a
//   syntactically valid expression, the function returns 1.  (In that
//   case, postfix may or may not be changed, but result must be
//   unchanged.)
int evaluate(string infix, const bool values[], string& postfix, bool& result)
{
	if ( convert(infix, postfix) == false ) //infix expression is not syntactically valid
		return 1;
	stack<bool> operandstack;
	for (int i = 0; i < postfix.size(); i++)
	{
		if (postfix[i] >= '0' && postfix[i] <= '9') //if character is a digit
			operandstack.push(values[postfix[i]-'0']); //push the actual true/false value represented by digit
		else if (postfix[i] == '!')
		{
			bool operand = operandstack.top();
			operandstack.pop();
			operandstack.push(!operand); //push the opposite bool
		}
		else //character is a binary operator
		{
			bool operand2 = operandstack.top();
			operandstack.pop();
			bool operand1 = operandstack.top();
			operandstack.pop();
			//apply operation
			if (postfix[i] == '&')
				operandstack.push(operand1 && operand2);
			else if (postfix[i] == '|')
				operandstack.push(operand1 || operand2);
		}
	}
	result = operandstack.top();
	return 0;
}

bool convert(string infix, string& postfix)
{
	postfix = "";
	stack<char> operatorstack;
	char lastchar = '*'; //initialize it to a random char first
	for (int i = 0; i <infix.size(); i++)
	{
		switch (infix[i])
		{
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				if (isdigit(lastchar)) //invalid syntax: two digits in a row
					return false;
				postfix += infix[i];
				break;
			case '(':
				if (isdigit(lastchar)) //invalid syntax: must have operator before parentheses
					return false;
				operatorstack.push(infix[i]);
				break;
			case ')':
				if (!isdigit(lastchar) && lastchar != ')' ) //invalid syntax: last char before end parentheses must be digit or ')'
					return false;
				while ( !operatorstack.empty() && operatorstack.top() != '(' )
				{
					postfix += operatorstack.top();
					operatorstack.pop();
				}
				if ( operatorstack.empty() ) //invalid syntax: exited loop because empty stack, so matching '(' not found
					return false;
				operatorstack.pop();
				break;
			case '!':
				if (isdigit(lastchar)) //invalid syntax: '!' cannot follow a digit
					return false;
				operatorstack.push(infix[i]);
				break;
			case ' ': //in case of spaces, skip to process next char
				continue;
			default: //binary operator case
				if (infix[i] != '&' && infix[i] != '|') //invalid syntax: not a valid character
					return false;
				if ( !isdigit(lastchar) && lastchar != ')' ) //invalid syntax: binary operator must follow digit or ')'
					return false;
				if (!operatorstack.empty() && operatorstack.top() == '!')
				{
					postfix += operatorstack.top();
					operatorstack.pop();
				}
				while ( !operatorstack.empty() && operatorstack.top() != '(' 
					//check precedence of character <= precedence of stack top
					&& (infix[i] == '|' || operatorstack.top() == '&') )
				{
					postfix += operatorstack.top();
					operatorstack.pop();
				}
				operatorstack.push(infix[i]);
				break;
		}
		lastchar = infix[i];
	}
	if ( !isdigit(lastchar) && lastchar != ')' ) //invalid syntax: string must end with digit or ')', can't end with operator
		return false;
	while ( !operatorstack.empty() )
	{
		if (operatorstack.top() == '(' ) //invalid syntax: will only have '(' in final stack if there was no matching end parenthesis ')' found earlier
			return false;
		postfix += operatorstack.top();
		operatorstack.pop();
	}
	return true;
}

/*
 int main()
        {
            bool ba[10] = {
              //  0      1      2      3      4      5      6      7      8      9
                true,  true,  true,  false, false, false, true,  false, true,  false
            };
            string pf;
            bool answer;
			assert(evaluate("2", ba, pf, answer) == 0 && pf == "2" && answer);
			assert(evaluate("(3)", ba, pf, answer) == 0 && pf == "3" && !answer);
			assert(evaluate("2&(3)", ba, pf, answer) == 0 && pf == "23&" && !answer);
			assert(evaluate("0 & !9", ba, pf, answer) == 0 && pf == "09!&" && answer);
			assert(evaluate("!(7|8)", ba, pf, answer) == 0 && pf == "78|!" && !answer);
			assert(evaluate("!7|8", ba, pf, answer) == 0 && pf == "7!8|" && answer);
			assert(evaluate("6|4&5", ba, pf, answer) == 0 && pf == "645&|" && answer);
			assert(evaluate("1&!(9|1&1|9) | !!!(9&1&9)", ba, pf, answer) == 0 && pf == "1911&|9|!&91&9&!!!|" && answer);
			assert(evaluate("8|&", ba, pf, answer) == 1);
			assert(evaluate("!|7", ba, pf, answer) == 1);
			assert(evaluate("&7|!5", ba, pf, answer) == 1);
			assert(evaluate("8&3|", ba, pf, answer) == 1);
			assert(evaluate("8&3!", ba, pf, answer) == 1);
			assert(evaluate("10", ba, pf, answer) == 1);
			assert(evaluate("7$", ba, pf, answer) == 1);
			assert(evaluate("56", ba, pf, answer) == 1);
			assert(evaluate("7&()", ba, pf, answer) == 1);
			assert(evaluate("7&(|)", ba, pf, answer) == 1);
			assert(evaluate("7&(2|)", ba, pf, answer) == 1);
			assert(evaluate("7&(|2)", ba, pf, answer) == 1);
			assert(evaluate("7&2)|5", ba, pf, answer) == 1);
			assert(evaluate("3|(7&2|5", ba, pf, answer) == 1);
			assert(evaluate("8&3!5", ba, pf, answer) == 1);


            assert(evaluate("2| 3", ba, pf, answer) == 0  &&  pf == "23|" &&  answer);
            assert(evaluate("8|", ba, pf, answer) == 1);
            assert(evaluate("4 5", ba, pf, answer) == 1);
            assert(evaluate("01", ba, pf, answer) == 1);
            assert(evaluate("()", ba, pf, answer) == 1);
            assert(evaluate("2(9|8)", ba, pf, answer) == 1);
            assert(evaluate("2(&8)", ba, pf, answer) == 1);
            assert(evaluate("(6&(7|7)", ba, pf, answer) == 1);
            assert(evaluate("4  |  !3 & (0&3) ", ba, pf, answer) == 0
                                   &&  pf == "43!03&&|"  &&  !answer);
            assert(evaluate("", ba, pf, answer) == 1);
            assert(evaluate(" 9  ", ba, pf, answer) == 0  &&  pf == "9"  &&  !answer);
            ba[2] = false;
            ba[9] = true;
            assert(evaluate("((9))", ba, pf, answer) == 0  &&  pf == "9"  &&  answer);
            assert(evaluate("2| 3", ba, pf, answer) == 0  &&  pf == "23|" &&  !answer);
            cout << "Passed all tests" << endl;
        }
*/
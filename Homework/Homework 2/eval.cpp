
#include <stack>
#include <iostream>
#include <cassert>
#include <string>
#include <cctype>

using namespace std;

bool convertToPostfix(string infix, string& postfix);
bool evaluatePostfix(const string& postfix, const bool values[]);
bool precedenceIsGreater(char first, char second);
bool isOperator(char c);

int evaluate(string infix, const bool values[], string& postfix, bool& result)
{
    if(!convertToPostfix(infix, postfix))
        return 1;
    result = evaluatePostfix(postfix, values);
    return 0;
}

bool convertToPostfix(string infix, string& postfix)
{
    if(infix == "")
        return false;
    string pf = "";
    stack<char> operators;
    string infix1 = ""; int open = 0; int closed = 0;
    for(int i = 0; i < infix.length(); i++)
    {
        if(infix[i] != ' ')
            infix1 += infix[i];
        if(infix[i] == '(')
            open++;
        if(infix[i] == ')')
            closed++;
    }
    if(open != closed)
        return false;
    for(int i = 0; i < infix1.length(); i++)
    {
        char c = infix1[i];
        char last = '\0';
        if(i != 0)
            last = infix1[i - 1];
        switch(c)
        {
            case '(' : case '!':
                if(i == infix1.length() - 1 || isdigit(last))
                    return false;
                operators.push(c);
                break;
            case ')' :
                if((!isdigit(last) && last != ')') || i == 0 || isOperator(last))
                    return false;
                while(!operators.empty() && operators.top() != '(')
                {
                    pf += operators.top();
                    operators.pop();
                }
                if(operators.empty())
                    return false;
                operators.pop();
                break;
            case '&' : case '|' :
                if(i == infix1.length() - 1 || (last != ')' && !isdigit(last)) || i == 0)
                    return false;
                while(!operators.empty() && operators.top() != '('
                      && precedenceIsGreater(operators.top(), c))
                {
                    pf += operators.top();
                    operators.pop();
                }
                operators.push(c);
                break;
            default :
                if(isdigit(last))
                    return false;
                if(isdigit(c))
                    pf += c;
                else
                    return false;
                break;
        }
    }
    while(!operators.empty())
    {
        pf += operators.top();
        operators.pop();
    }
    postfix = pf;
    return true;
}

bool evaluatePostfix(const string& postfix, const bool values[])
{
    stack<bool> operands;
    for(int i = 0; i < postfix.length(); i++)
    {
        char o1; char o2;
        char c = postfix[i];
        if(isdigit(c))
        {
            int test = c - '0';
            operands.push(values[test]); // This might have to change
        }
        else if(c == '!')
        {
            o1 = operands.top();
            operands.pop();
            operands.push(!o1);
        }
        else
        {
            o2 = operands.top();
            operands.pop();
            o1 = operands.top();
            operands.pop();
            if(c == '|')
                operands.push(o1 || o2);
            if(c == '&')
                operands.push(o1 && o2);
        }
    }
    
    return operands.top();
}

bool precedenceIsGreater(char first, char second)
{
    if(first == '!' && second != '!')
        return true;
    else if(first == '!' && second == '!')
        return false;
    if(first == '&')
    {
        if(second == '|')
            return true;
        if(second == '&')
            return false;
        if(second == '!')
            return false;
    }
    if(first == '|' && second == '|')
        return true;
    if(first == '|' && second != '|')
        return false;
    return false;
} // Check operators of equal precedence

bool isOperator(char c)
{
    return (c == '!' || c == '&' || c == '|');
}

#include <iostream>
#include <string>
using namespace std;

#define CAP 200

struct StringStack
{
    string items[CAP];
    int topIndex = -1;

    void push(const string &val) { items[++topIndex] = val; }
    string pop() { 
        return items[topIndex--]; 
    }
    string top() { 
        return items[topIndex]; 
    }
    bool empty() { 
        return topIndex < 0; 
    }
};

bool checkFunc(const string &token)
{
    return token == "sqrt" || token == "sin" || token == "cos" || token == "tan" || token == "pm";
}

bool checkOp(const string &token)
{
    return token == "+" || token == "-" || token == "*" || token == "/" || token == "^";
}

int getPriority(const string &op)
{
    if (op == "^")
        return 3;
    if (op == "*" || op == "/")
        return 2;
    if (op == "+" || op == "-")
        return 1;
    return 0;
}

int parseTokens(const string &rawInput, string tokenList[])
{
    int total = 0;
    string temp = "";
    int len = rawInput.length();

    for (int idx = 0; idx <= len; idx++)
    {
        if (idx == len || rawInput[idx] == ' ')
        {
            if (!temp.empty())
            {
                tokenList[total++] = temp;
                temp = "";
            }
        }
        else
        {
            temp += rawInput[idx];
        }
    }
    return total;
}

int buildPostfix(string inputTokens[], int count, string output[])
{
    StringStack stk;
    int outIdx = 0;

    for (int i = 0; i < count; i++)
    {
        string cur = inputTokens[i];

        if (cur != "(" && cur != ")" && !checkOp(cur) && !checkFunc(cur))
        {
            output[outIdx++] = cur;
        }
        else if (checkFunc(cur) || cur == "(")
        {
            stk.push(cur);
        }
        else if (cur == ")")
        {
            while (!stk.empty() && stk.top() != "(")
            {
                output[outIdx++] = stk.pop();
            }
            if (!stk.empty())
                stk.pop();
            while (!stk.empty() && checkFunc(stk.top()))
            {
                output[outIdx++] = stk.pop();
            }
        }
        else if (checkOp(cur))
        {
            while (!stk.empty() && stk.top() != "(" &&
                   (checkFunc(stk.top()) || getPriority(stk.top()) >= getPriority(cur)))
            {
                output[outIdx++] = stk.pop();
            }
            stk.push(cur);
        }
    }

    while (!stk.empty())
    {
        output[outIdx++] = stk.pop();
    }

    return outIdx;
}

int buildPrefix(string inputTokens[], int count, string output[])
{
    string reversed[CAP];
    for (int i = 0; i < count; i++)
    {
        string cur = inputTokens[count - 1 - i];
        if (cur == "(")
            cur = ")";
        else if (cur == ")")
            cur = "(";
        reversed[i] = cur;
    }

    StringStack stk;
    string tempBuffer[CAP];
    int bufIdx = 0;

    for (int i = 0; i < count; i++)
    {
        string cur = reversed[i];

        if (cur != "(" && cur != ")" && !checkOp(cur) && !checkFunc(cur))
        {
            tempBuffer[bufIdx++] = cur;
        }
        else if (checkFunc(cur) || cur == "(")
        {
            stk.push(cur);
        }
        else if (cur == ")")
        {
            while (!stk.empty() && stk.top() != "(")
            {
                tempBuffer[bufIdx++] = stk.pop();
            }
            if (!stk.empty())
                stk.pop();
            while (!stk.empty() && checkFunc(stk.top()))
            {
                tempBuffer[bufIdx++] = stk.pop();
            }
        }
        else if (checkOp(cur))
        {
            while (!stk.empty() && stk.top() != "(" &&
                   (checkFunc(stk.top()) || getPriority(stk.top()) > getPriority(cur)))
            {
                tempBuffer[bufIdx++] = stk.pop();
            }
            stk.push(cur);
        }
    }

    while (!stk.empty())
    {
        tempBuffer[bufIdx++] = stk.pop();
    }

    for (int i = 0; i < bufIdx; i++)
    {
        output[i] = tempBuffer[bufIdx - 1 - i];
    }

    return bufIdx;
}

void displaySequence(string arr[], int size)
{
    for (int i = 0; i < size; i++)
    {
        cout << arr[i] << (i == size - 1 ? "" : " ");
    }
    cout << "\n";
}

void executeConversion(const string &header, const string &expr)
{
    string arrTokens[CAP], arrPostfix[CAP], arrPrefix[CAP];

    int tokenCount = parseTokens(expr, arrTokens);
    int postCount = buildPostfix(arrTokens, tokenCount, arrPostfix);
    int preCount = buildPrefix(arrTokens, tokenCount, arrPrefix);

    cout << "\n=== " << header << " ===" << endl;
    cout << "Infix RHS : " << expr << endl;
    cout << "Postfix   : ";
    displaySequence(arrPostfix, postCount);
    cout << "Prefix    : ";
    displaySequence(arrPrefix, preCount);
}

int main()
{
    cout << "Converting RHS of trigonometric identities to Postfix / Prefix" << endl;
    cout << "(x/a/b are angle variables, pm = +/- , sqrt = square root)" << endl;

    executeConversion(
        "Half-Angle Formula: tan(x/2) RHS",
        "pm ( sqrt ( ( 1 - cos x ) / ( 1 + cos x ) ) )");

    executeConversion(
        "Pythagorean Identity: sin^2(x) RHS",
        "1 - ( cos x ) ^ 2");

    executeConversion(
        "Product-to-Sum Identity: sin(a)*sin(b) RHS",
        "( 1 / 2 ) * ( cos ( a - b ) - cos ( a + b ) )");

    return 0;
}
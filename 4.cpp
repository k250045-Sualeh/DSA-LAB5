#include <iostream>
#include <string>
using namespace std;

#define LIMIT 20
#define BUFFER_SIZE 100

double parseToDouble(const string &text) {
    double total = 0.0;
    double polarity = 1.0;
    size_t pos = 0;

    if (pos < text.length() && (text[pos] == '+' || text[pos] == '-')) {
        if (text[pos] == '-') polarity = -1.0;
        pos++;
    }

    while (pos < text.length() && text[pos] >= '0' && text[pos] <= '9') {
        total = total * 10.0 + (text[pos] - '0');
        pos++;
    }

    if (pos < text.length() && text[pos] == '.') {
        pos++;
        double weight = 0.1;
        while (pos < text.length() && text[pos] >= '0' && text[pos] <= '9') {
            total += (text[pos] - '0') * weight;
            weight *= 0.1;
            pos++;
        }
    }

    return total * polarity;
}


string formatDouble(double val) {
    long long integerPart = val; 
    double decimalPart = val - integerPart;
    if (decimalPart < 0) decimalPart = -decimalPart;

    string output = "";
    if (val < 0 && integerPart == 0) output += "-";
    output += to_string(integerPart);

    if (decimalPart > 0.00001) {
        output += ".";
        for (int k = 0; k < 4; k++) {
            decimalPart *= 10;
            int digit = decimalPart; 
            output += to_string(digit);
            decimalPart -= digit;
        }
    }
    return output;
}

int splitTokens(const string &expr, string tokenArray[]) {
    int totalTokens = 0;
    size_t index = 0;
    size_t length = expr.length();

    while (index < length) {
        char ch = expr[index];

        if (ch == ' ') {
            index++;
            continue;
        }

        if ((ch >= '0' && ch <= '9') || ch == '.') {
            string numberStr = "";
            while (index < length && ((expr[index] >= '0' && expr[index] <= '9') || expr[index] == '.')) {
                numberStr += expr[index];
                index++;
            }
            tokenArray[totalTokens++] = numberStr;
        }
        else if (ch == 'x' || ch == 'X') {
            tokenArray[totalTokens++] = "x";
            index++;
        }
        else if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '^' || ch == '(' || ch == ')') {
            if (ch == '-' && (totalTokens == 0 || tokenArray[totalTokens - 1] == "(" ||
                              tokenArray[totalTokens - 1] == "+" || tokenArray[totalTokens - 1] == "-" ||
                              tokenArray[totalTokens - 1] == "*" || tokenArray[totalTokens - 1] == "/" ||
                              tokenArray[totalTokens - 1] == "^")) {
                tokenArray[totalTokens++] = "0";
            }
            tokenArray[totalTokens++] = string(1, ch);
            index++;
        }
        else {
            index++;
        }
    }
    return totalTokens;
}

bool checkOperator(const string &token) {
    return token == "+" || token == "-" || token == "*" || token == "/" || token == "^";
}

int fetchPrecedence(const string &op) {
    if (op == "^") return 3;
    if (op == "*" || op == "/") return 2;
    if (op == "+" || op == "-") return 1;
    return 0;
}

struct OperatorStack {
    string elements[BUFFER_SIZE];
    int topIndex = -1;

    void push(const string &str) { elements[++topIndex] = str; }
    string pop() {
        return elements[topIndex--]; 
    }
    string top() {
        return elements[topIndex]; 
    }
    bool empty() {
        return topIndex == -1; 
    }
};

int convertToPostfix(string inputTokens[], int count, string outputTokens[]) {
    OperatorStack opStk;
    int outCount = 0;

    for (int i = 0; i < count; i++) {
        string t = inputTokens[i];

        if (t != "(" && t != ")" && !checkOperator(t)) {
            outputTokens[outCount++] = t;
        } else if (t == "(") {
            opStk.push(t);
        } else if (t == ")") {
            while (!opStk.empty() && opStk.top() != "(") {
                outputTokens[outCount++] = opStk.pop();
            }
            if (!opStk.empty()) opStk.pop();
        } else if (checkOperator(t)) {
            while (!opStk.empty() && opStk.top() != "(" &&
                   fetchPrecedence(opStk.top()) >= fetchPrecedence(t)) {
                outputTokens[outCount++] = opStk.pop();
            }
            opStk.push(t);
        }
    }

    while (!opStk.empty()) outputTokens[outCount++] = opStk.pop();
    return outCount;
}

double computePower(double base, double exponent) {
    long long expVal = exponent; 
    double accum = 1.0;
    for (long long i = 0; i < expVal; i++) accum *= base;
    return accum;
}

double computePostfix(string postfixTokens[], int size, double valX) {
    double evalStack[BUFFER_SIZE];
    int top = -1;

    for (int i = 0; i < size; i++) {
        string t = postfixTokens[i];

        if (t == "x") {
            evalStack[++top] = valX;
        } else if (checkOperator(t)) {
            double rightOperand = evalStack[top--];
            double leftOperand = evalStack[top--];
            double outcome = 0.0;
            if (t == "+") outcome = leftOperand + rightOperand;
            else if (t == "-") outcome = leftOperand - rightOperand;
            else if (t == "*") outcome = leftOperand * rightOperand;
            else if (t == "/") outcome = leftOperand / rightOperand;
            else if (t == "^") outcome = computePower(leftOperand, rightOperand);
            evalStack[++top] = outcome;
        } else {
            evalStack[++top] = parseToDouble(t);
        }
    }

    return evalStack[top];
}

bool checkCondition(const string &condStr, double valX) {
    string symbol = "";
    size_t matchPos = string::npos;
    size_t symbolLen = 0;

    for (size_t i = 0; i + 1 < condStr.length(); i++) {
        string dual = condStr.substr(i, 2);
        if (dual == ">=" || dual == "<=" || dual == "==" || dual == "!=") {
            symbol = dual;
            matchPos = i;
            symbolLen = 2;
            break;
        }
    }

    if (symbol.empty()) {
        for (size_t i = 0; i < condStr.length(); i++) {
            if (condStr[i] == '>' || condStr[i] == '<') {
                symbol = string(1, condStr[i]);
                matchPos = i;
                symbolLen = 1;
                break;
            }
        }
    }

    if (matchPos == string::npos) {
        cout << "Invalid condition format: " << condStr << endl;
        return false;
    }

    string rightSide = condStr.substr(matchPos + symbolLen);
    size_t offset = 0;
    while (offset < rightSide.length() && rightSide[offset] == ' ') offset++;
    rightSide = rightSide.substr(offset);

    double targetVal = parseToDouble(rightSide);

    if (symbol == ">=") return valX >= targetVal;
    if (symbol == "<=") return valX <= targetVal;
    if (symbol == "==") return valX == targetVal;
    if (symbol == "!=") return valX != targetVal;
    if (symbol == ">")  return valX > targetVal;
    if (symbol == "<")  return valX < targetVal;

    return false;
}

int main() {
    int pieceCount;
    string exprList[LIMIT];
    string condList[LIMIT];

    cout << "Enter number of piecewise expressions: ";
    cin >> pieceCount;
    cin.ignore();

    if (pieceCount <= 0 || pieceCount > LIMIT) {
        cout << "Invalid number of pieces." << endl;
        return 0;
    }

    for (int idx = 0; idx < pieceCount; idx++) {
        cout << "Enter Expression " << (idx + 1) << " (in terms of x): ";
        getline(cin, exprList[idx]);
        cout << "Enter Condition " << (idx + 1) << " (e.g. x >= 0): ";
        getline(cin, condList[idx]);
    }

    double inputX;
    cout << "Enter x: ";
    cin >> inputX;

    int activeIdx = -1;
    for (int idx = 0; idx < pieceCount; idx++) {
        if (checkCondition(condList[idx], inputX)) {
            activeIdx = idx;
            break;
        }
    }

    if (activeIdx == -1) {
        cout << "\nNo condition satisfied for x = " << inputX << endl;
        return 0;
    }

    string parsedTokens[BUFFER_SIZE], postfixResult[BUFFER_SIZE];
    int tokenLen = splitTokens(exprList[activeIdx], parsedTokens);
    int postfixLen = convertToPostfix(parsedTokens, tokenLen, postfixResult);

    double finalAnswer = computePostfix(postfixResult, postfixLen, inputX);

    cout << "\nThe program selects:" << endl;
    cout << "Condition   : " << condList[activeIdx] << endl;
    cout << "Expression  : " << exprList[activeIdx] << endl;

    cout << "Postfix     : ";
    for (int i = 0; i < postfixLen; i++) {
        cout << postfixResult[i] << (i == postfixLen - 1 ? "" : " ");
    }
    cout << endl;

    cout << "f(" << formatDouble(inputX) << ") = " << finalAnswer << endl;

    return 0;
}
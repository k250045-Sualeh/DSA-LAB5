#include <iostream>
#include <string>
using namespace std;

#define MAX_ELEMENTS 500

struct CustomStack {
    string storage[MAX_ELEMENTS];
    int topPos = -1;
    void push(const string &val) { storage[++topPos] = val; }
    string pop() { 
        return storage[topPos--]; 
    }
    string peek() { 
        return storage[topPos]; 
    }
    bool empty() { 
        return topPos < 0; 
    }
};

int getPrecedence(const string &symbol) {
    if (symbol == "+") return 1;
    return 0;
}

bool checkIsOp(const string &token) {
    return token == "+";
}

int convertToPostfix(string inputTokens[], int length, string output[]) {
    CustomStack stk;
    int outIdx = 0;

    for (int i = 0; i < length; i++) {
        string curToken = inputTokens[i];
        if (!checkIsOp(curToken)) {
            output[outIdx++] = curToken;
        } else {
            while (!stk.empty() && getPrecedence(stk.peek()) >= getPrecedence(curToken)) {
                output[outIdx++] = stk.pop();
            }
            stk.push(curToken);
        }
    }
    while (!stk.empty()) output[outIdx++] = stk.pop();
    return outIdx;
}

int convertToPrefix(string inputTokens[], int length, string output[]) {
    string reversedTokens[MAX_ELEMENTS];
    for (int i = 0; i < length; i++) {
        reversedTokens[i] = inputTokens[length - 1 - i];
    }

    CustomStack stk;
    string buffer[MAX_ELEMENTS];
    int bufIdx = 0;

    for (int i = 0; i < length; i++) {
        string curToken = reversedTokens[i];
        if (!checkIsOp(curToken)) {
            buffer[bufIdx++] = curToken;
        } else {
            while (!stk.empty() && getPrecedence(stk.peek()) > getPrecedence(curToken)) {
                buffer[bufIdx++] = stk.pop();
            }
            stk.push(curToken);
        }
    }
    while (!stk.empty()) buffer[bufIdx++] = stk.pop();

    for (int i = 0; i < bufIdx; i++) {
        output[i] = buffer[bufIdx - 1 - i];
    }
    return bufIdx;
}

int main() {
    int count;
    cout << "Enter number of terms (n): ";
    cin >> count;

    if (count <= 0) {
        cout << "Invalid number of terms." << endl;
        return 0;
    }

    string termList[MAX_ELEMENTS];
    for (int idx = 1; idx <= count; idx++) {
        int termVal = 3 * idx - 2;
        termList[idx - 1] = to_string(termVal);
    }

    string allTokens[MAX_ELEMENTS];
    int totalTokens = 0;
    for (int idx = 0; idx < count; idx++) {
        allTokens[totalTokens++] = termList[idx];
        if (idx != count - 1) allTokens[totalTokens++] = "+";
    }

    string postfixResult[MAX_ELEMENTS], prefixResult[MAX_ELEMENTS];
    int pfSize = convertToPostfix(allTokens, totalTokens, postfixResult);
    int prSize = convertToPrefix(allTokens, totalTokens, prefixResult);

    cout << "\nGeneral Form : 1 + 4 + 7 + ... + (3n - 2)" << endl;
    cout << "For n = " << count << ":\n" << endl;

    cout << "Series  : ";
    for (int i = 0; i < count; i++) {
        cout << termList[i] << (i != count - 1 ? " + " : "");
    }
    cout << endl;

    cout << "Postfix : ";
    for (int i = 0; i < pfSize; i++) {
        cout << postfixResult[i] << (i != pfSize - 1 ? " " : "");
    }
    cout << endl;

    cout << "Prefix  : ";
    for (int i = 0; i < prSize; i++) {
        cout << prefixResult[i] << (i != prSize - 1 ? " " : "");
    }
    cout << endl;

    return 0;
}
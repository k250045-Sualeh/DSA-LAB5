#include <iostream>
using namespace std;
 
int main() {
    const int MAX_SIZE = 100;
    int stackArr[MAX_SIZE];
    int top = -1;
    int n;
 
    cout << "Enter number of elements: ";
    cin >> n;
 
    if (n <= 0 || n > MAX_SIZE) {
        cout << "Invalid number of elements (must be between 1 and " << MAX_SIZE << ")." << endl;
        return 0;
    }

    cout << "Enter elements one by one (first = bottom, last = top):" << endl;
    for (int i = 0; i < n; i++) {
        int val;
        cout << "Element " << (i + 1) << ": ";
        cin >> val;
        top++;
        stackArr[top] = val;
    }

    cout << "\nStack contents (Top -> Bottom): ";
    for (int i = top; i >= 0; i--) {
        cout << stackArr[i];
        if (i != 0) cout << " -> ";
    }
    cout << endl;
 
    int maxVal = stackArr[0];
    int minVal = stackArr[0];
    for (int i = 1; i <= top; i++) {
        if (stackArr[i] > maxVal) maxVal = stackArr[i];
        if (stackArr[i] < minVal) minVal = stackArr[i];
    }
 
    cout << "\nMaximum value in stack : " << maxVal << endl;
    cout << "Minimum value in stack : " << minVal << endl;
 
    if (n % 2 != 0) {
        int midIndex = n / 2;
        cout << "Middle element        : " << stackArr[midIndex] << endl;
    } else {
        int mid1 = (n / 2) - 1;
        int mid2 = n / 2;
        cout << "Middle elements        : " << stackArr[mid1]
             << " and " << stackArr[mid2] << endl;
    }
 
    return 0;
}
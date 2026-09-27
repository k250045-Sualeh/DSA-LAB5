#include <iostream>
using namespace std;

const int MAX_CAPACITY = 100;

struct LinkedNode {
    int value;
    LinkedNode* nextPtr;
};

LinkedNode* stackHead = nullptr;
LinkedNode* stackTail = nullptr;
int elementCount = 0;

void appendToStack(int newValue) {
    LinkedNode* newNode = new LinkedNode();
    newNode->value = newValue;
    newNode->nextPtr = nullptr;

    if (stackHead == nullptr) {
        stackHead = newNode;
        stackTail = newNode;
    } else {
        stackTail->nextPtr = newNode;
        stackTail = newNode;
    }
    elementCount++;
}

void printStackContents() {
    LinkedNode* current = stackHead;
    cout << "(Bottom -> Top): ";
    while (current != nullptr) {
        cout << current->value;
        if (current->nextPtr != nullptr) cout << " -> ";
        current = current->nextPtr;
    }
    cout << endl;
}

void arrangeStackInOrder() {
    for (LinkedNode* outer = stackHead; outer != nullptr; outer = outer->nextPtr) {
        LinkedNode* minPosition = outer;
        for (LinkedNode* inner = outer->nextPtr; inner != nullptr; inner = inner->nextPtr) {
            if (inner->value < minPosition->value) {
                minPosition = inner;
            }
        }
        if (minPosition != outer) {
            int tempValue = outer->value;
            outer->value = minPosition->value;
            minPosition->value = tempValue;
        }
    }
}

struct CircularBuffer {
    int dataArray[MAX_CAPACITY];
    int frontIndex;
    int rearIndex;
    int bufferSize;
    int currentItems;
};

void initializeBuffer(CircularBuffer &buffer, int maxSize) {
    buffer.frontIndex = 0;
    buffer.rearIndex = -1;
    buffer.bufferSize = maxSize;
    buffer.currentItems = 0;
}

bool isBufferFull(CircularBuffer &buffer) {
    return buffer.currentItems == buffer.bufferSize;
}

void addToBuffer(CircularBuffer &buffer, int newItem) {
    if (isBufferFull(buffer)) {
        cout << "Buffer is full, cannot add " << newItem << endl;
        return;
    }
    buffer.rearIndex = (buffer.rearIndex + 1) % buffer.bufferSize;
    buffer.dataArray[buffer.rearIndex] = newItem;
    buffer.currentItems++;
}

void printBufferContents(CircularBuffer &buffer) {
    if (buffer.currentItems == 0) {
        cout << "Buffer is empty." << endl;
        return;
    }
    cout << "Buffer Contents (Front -> Rear): ";
    int currentPos = buffer.frontIndex;
    for (int i = 0; i < buffer.currentItems; i++) {
        cout << buffer.dataArray[currentPos];
        if (i != buffer.currentItems - 1) cout << " -> ";
        currentPos = (currentPos + 1) % buffer.bufferSize;
    }
    cout << endl;
}

int main() {
    int firstStackSize, secondStackSize;
    int firstArray[MAX_CAPACITY], secondArray[MAX_CAPACITY];

    cout << "Enter elements for Primary Collection (size): ";
    cin >> firstStackSize;
    cout << "Enter " << firstStackSize << " elements (bottom to top):" << endl;
    for (int i = 0; i < firstStackSize; i++) {
        cout << "Position " << (i + 1) << ": ";
        cin >> firstArray[i];
    }

    cout << "\nEnter elements for Secondary Collection (size): ";
    cin >> secondStackSize;
    cout << "Enter " << secondStackSize << " elements (bottom to top):" << endl;
    for (int i = 0; i < secondStackSize; i++) {
        cout << "Position " << (i + 1) << ": ";
        cin >> secondArray[i];
    }

    int firstIndex = 0, secondIndex = 0;
    while (firstIndex < firstStackSize && secondIndex < secondStackSize) {
        appendToStack(firstArray[firstIndex++]);
        appendToStack(secondArray[secondIndex++]);
    }
    while (firstIndex < firstStackSize) {
        appendToStack(firstArray[firstIndex++]);
    }
    while (secondIndex < secondStackSize) {
        appendToStack(secondArray[secondIndex++]);
    }

    cout << "\nCombined Collection (alternating merge, linked list):" << endl;
    printStackContents();

    arrangeStackInOrder();
    cout << "\nSorted Collection (ascending order):" << endl;
    printStackContents();

    CircularBuffer circularStorage;
    initializeBuffer(circularStorage, elementCount);

    LinkedNode* currentNode = stackHead;
    while (currentNode != nullptr) {
        addToBuffer(circularStorage, currentNode->value);
        currentNode = currentNode->nextPtr;
    }

    cout << "\nCircular Buffer from sorted elements:" << endl;
    printBufferContents(circularStorage);

    return 0;
}
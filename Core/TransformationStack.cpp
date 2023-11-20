#include "TransformationStack.h"

using namespace std;


void TransformationStack::push(const Matrix4& matrix) {
    if(stack.size() == 0) {
        stack.push(matrix);
    }
    else {
        stack.push(stack.top() * matrix);
    }
}

void TransformationStack::pop() {
    stack.pop();
}

Matrix4 TransformationStack::top() const {
    return stack.top();
}

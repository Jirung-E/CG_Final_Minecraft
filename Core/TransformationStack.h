#pragma once

#include <stack>

#include "../Math/Math.h"


// 부모-자식 관계가 있는 오브젝트에서, 자식의 변환상태를 알아내기 위해 사용하는 스택
// 부모의 변환행렬부터 차례대로 곱하면서 넣는다.
class TransformationStack {
private:
    std::stack<Matrix4> stack;

public:
    void push(const Matrix4& matrix);
    void pop();
    Matrix4 top() const;
};

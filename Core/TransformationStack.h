#pragma once

#include <stack>

#include "../Math/Math.h"


// �θ�-�ڽ� ���谡 �ִ� ������Ʈ����, �ڽ��� ��ȯ���¸� �˾Ƴ��� ���� ����ϴ� ����
// �θ��� ��ȯ��ĺ��� ���ʴ�� ���ϸ鼭 �ִ´�.
class TransformationStack {
private:
    std::stack<Matrix4> stack;

public:
    void push(const Matrix4& matrix);
    void pop();
    Matrix4 top() const;
};

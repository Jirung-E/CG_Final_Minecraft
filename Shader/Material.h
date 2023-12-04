#pragma once

#include "../Util/Util.h"


class Material {
public:
    ColorRGB base_color;
    float shininess;
    float reflectivity;     // 0 ~ 1        // �ſﰰ�� ���̴���

public:
    Material(const ColorRGB& base_color, float shininess, float reflectivity);
    Material(const Material& material, const ColorRGB& base_color);

public:
    static const Material basic;
    static const Material matte;
    static const Material metal;
    static const Material base;
};

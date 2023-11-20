#pragma once

#include "../Util/Util.h"


class Material {
public:
    ColorRGB base_color;
    float shininess;
    float reflectivity;     // 0 ~ 1        // 거울같이 보이는지

public:
    static const Material basic;
    static const Material matte;
    static const Material metal;
};

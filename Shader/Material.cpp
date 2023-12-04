#include "Material.h"


Material::Material(const ColorRGB& base_color, float shininess, float reflectivity) : base_color { base_color }, shininess { shininess }, reflectivity { reflectivity } {

}

Material::Material(const Material& material, const ColorRGB& base_color) : Material { base_color, material.shininess, material.reflectivity } {

}


const Material Material::basic { RGB_Gray, 32.0f, 0.5f };
const Material Material::matte { RGB_Gray, 1.0f, 0.0f };
const Material Material::metal { RGB_Gray, 256.0f, 0.95f };
const Material Material::base { RGB_White, 1.0f, 0.0f };

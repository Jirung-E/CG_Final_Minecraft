#version 330 core

#define MAX_LIGHTS 32

in vec3 frag_pos;
in vec3 frag_normal;
in vec2 frag_texcoord;
out vec4 frag_color;

struct Light {
    vec3 position;
    vec3 color;
    float intensity;
    float ambient;
};

struct Material {
    vec4 color;
    float shininess;
    float reflectivity;
};

uniform vec3 cam_pos;
uniform int num_lights;
uniform Light lights[MAX_LIGHTS];
uniform Material material;
uniform sampler2D out_texture;
uniform bool use_texture;

void main(void) {
    vec3 material_color = material.color.rgb * material.color.a;

    if(num_lights == 0) {
        if(use_texture) {
            frag_color = texture(out_texture, frag_texcoord);
        }
        else {
            frag_color = vec4(material_color, 1.0);
        }
        return;
    }

    vec3 result = vec3(0, 0, 0);

    for(int i=0; i<num_lights; ++i) {
        Light light = lights[i];
        vec3 light_color = light.color * light.intensity;

        vec3 ambient = light.ambient * light_color * (1 - material.reflectivity*0.9);

        vec3 normal = normalize(frag_normal);
        vec3 light_dir = normalize(light.position - frag_pos);

        float diffuse_light = max(dot(normal, light_dir), 0.0);
        diffuse_light = pow(diffuse_light, 1+material.reflectivity);
        vec3 diffuse = diffuse_light * light_color;

        vec3 view_dir = normalize(cam_pos - frag_pos);
        vec3 reflect_dir = reflect(-light_dir, normal);
        float specular_light = max(dot(view_dir, reflect_dir), 0.0);
        specular_light = pow(specular_light, material.shininess) * material.reflectivity;
        vec3 specular = specular_light * light_color;

        result += (ambient + diffuse) * material_color + specular;
    }

    frag_color = vec4(result, 1.0);
    if(use_texture) {
        frag_color = texture(out_texture, frag_texcoord) * frag_color;
    }
}

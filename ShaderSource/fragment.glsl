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
    float c1;
    float c2;
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
    vec4 result = vec4(material_color, 1);

    if(use_texture) {
        result = texture(out_texture, frag_texcoord);
        result.rgb *= material_color;
    }

    if(result == vec4(0, 0, 0, 1)) {
        discard;
    }

    if(num_lights == 0) {
        frag_color = result;
        return;
    }

    frag_color = vec4(0, 0, 0, 1);

    float default_ambient = 0.1;

    for(int i=0; i<num_lights; ++i) {
        Light light = lights[i];
        float distance = length(light.position - frag_pos);
        float attenuation = 1.0 / (1.0 + light.c1 * distance + light.c2 * distance * distance);
        frag_color += vec4(default_ambient, default_ambient, default_ambient, 1) * result;
        if(attenuation < 0.01) {
            continue;
        }
        vec3 light_color = light.color * light.intensity;
        light_color *= attenuation;

        vec3 ambient = light.ambient * light_color;

        vec3 normal = normalize(frag_normal);
        vec3 light_dir = normalize(light.position - frag_pos);

        float diffuse_light = max(dot(normal, light_dir) * (1-material.reflectivity*0.5), 0.0);
        vec3 diffuse = diffuse_light * light_color;

        vec3 view_dir = normalize(cam_pos - frag_pos);
        vec3 reflect_dir = reflect(-light_dir, normal);
        float specular_light = max(dot(view_dir, reflect_dir), 0.0);
        specular_light = pow(specular_light, material.shininess) * material.reflectivity;
        vec3 specular = specular_light * light_color;

        vec3 ad = ambient + diffuse;
        if(length(ad) > 2.0) {          // sqrt(3) ¹Ý¿Ã¸² -> 2
            ad = normalize(ad) * 2;
        }
        frag_color += vec4(ad, 1) * result + vec4(specular, 1);
    }
}

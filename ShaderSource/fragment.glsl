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
uniform float render_distance;
uniform vec3 fog_color;

void main(void) {
    vec3 material_color = material.color.rgb * material.color.a;
    vec4 result = vec4(material_color, 1);

    if(use_texture) {
        vec4 tex_color = texture(out_texture, frag_texcoord);
        result = tex_color;
        result.rgb *= material_color;
    }

    if(result.a == 0) {
        discard;
    }

    if(num_lights == 0) {
        frag_color = result;
        float distance = length(frag_pos - cam_pos);
        if(distance > render_distance-1) {
            float t = (render_distance-distance) / 1;
            frag_color = mix(frag_color, vec4(fog_color, 1), 1-t);
        }
        return;
    }

    float default_ambient = 0.1;
    frag_color = vec4(default_ambient, default_ambient, default_ambient, 1) * result;

    for(int i=0; i<num_lights; ++i) {
        Light light = lights[i];
        float distance = length(light.position - frag_pos);
        float attenuation = 1.0 / (1.0 + light.c1 * distance + light.c2 * distance * distance);
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
        if(frag_color.r > result.r*2) {
            frag_color.r = result.r*2;
        }
        if(frag_color.g > result.g*2) {
            frag_color.g = result.g*2;
        }
        if(frag_color.b > result.b*2) {
            frag_color.b = result.b*2;
        }
    }
    float distance = length(frag_pos - cam_pos);
    if(distance > render_distance-10) {
        float t = (render_distance-distance) / 10;
        frag_color = mix(frag_color, vec4(fog_color, 1), 1-t);
    }
}

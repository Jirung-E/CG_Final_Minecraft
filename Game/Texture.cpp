#include "Texture.h"

#include <gl/glew.h>
#include <gl/freeglut.h>

#define STB_IMAGE_IMPLEMENTATION
#include "../Util/stb_image.h"

using namespace std;


unordered_map<string, Texture> Texture::textures;


Texture::Texture() : id { 0 } {

}

Texture::Texture(unsigned int id) : id { id } {

}


unsigned int Texture::getID() const {
    return id;
}


Texture& Texture::get(const string& path) {
    if (textures.find(path) == textures.end()) {
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        int width;
        int height;
        int number_of_channels;
        stbi_set_flip_vertically_on_load(true);
        unsigned char* data = stbi_load(path.c_str(), &width, &height, &number_of_channels, 0);

        glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glBindTexture(GL_TEXTURE_2D, texture);

        stbi_image_free(data);

        textures[path] = Texture { texture };
    }

    return textures[path];
}

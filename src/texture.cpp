#include <vector>
#include "texture.h"

Texture::Texture()
        : Width(0), Height(0), Internal_Format(GL_RGB), Image_Format(GL_RGB),
          Wrap_S(GL_REPEAT), Wrap_T(GL_REPEAT), Filter_Min(GL_LINEAR),
          Filter_Max(GL_LINEAR)
{
    glGenTextures(1, &this->ID);
}

void Texture::Generate(GLuint width, GLuint height, unsigned char *data)
{
    this->Width = 256;
    this->Height = 256;

    auto image = std::vector<unsigned char>{ 255, 255, 255, 255 };
    // Create Texture
    glBindTexture(GL_TEXTURE_2D, this->ID);
    //glTexImage2D(GL_TEXTURE_2D, 0, this->Internal_Format, width, height, 0, this->Image_Format, GL_UNSIGNED_BYTE, data);
    glTexImage2D(GL_TEXTURE_2D, 0, this->Internal_Format, 1, 1, 0,
                 this->Image_Format, GL_UNSIGNED_BYTE,
                 image.data());

    // Set Texture wrap and filter modes
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->Wrap_S);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->Wrap_T);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->Filter_Min);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->Filter_Max);
    // Unbind texture
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Bind() const
{
    glBindTexture(GL_TEXTURE_2D, this->ID);
}
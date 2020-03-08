#include "Texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace MGLlib {

Texture::Texture(const std::string& path)
    : m_RendererID(0), m_FilePath(path), m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_BPP(0) 
{
    // flips the texture up side down
    // opengl expects bottomleft as 0,0
    // needed for png
    stbi_set_flip_vertically_on_load(1);
    m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4);

    GLCall(glGenTextures(1, &m_RendererID));
    // todo: add 3D image support ?
    GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID)); // bind

    // these need to be specified
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, 
        GL_RGBA8,               // format inside the GPU
        m_Width, m_Height,      // size of the texture
        0,                      // border width
        GL_RGBA,                // format we're giving to the GPU
        GL_UNSIGNED_BYTE,       // size of the rgba components
        m_LocalBuffer));        // can be given later if data is not yet ready
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));    // unbind

    // not specifically needed if we want to change it later
    if(m_LocalBuffer){
        stbi_image_free(m_LocalBuffer);
    }
}
Texture::~Texture(){
    GLCall(glDeleteTextures(1, &m_RendererID));
}


void Texture::Bind(unsigned int slot) const{
    GLCall(glActiveTexture(GL_TEXTURE0 + slot));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

void Texture::Unbind() const{
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

} // namespace
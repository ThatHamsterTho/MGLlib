#ifndef RENDERERHEADERGUARD
#define RENDERERHEADERGUARD

#include "Debugger.hpp"

#include "VertexArray.hpp"
#include "IndexBuffer.hpp"
#include "shader.hpp"

class Renderer {
    private:
    public:
        Renderer();
        ~Renderer();
        void Clear(unsigned int ClearBufferBit = GL_COLOR_BUFFER_BIT);
        void Draw(const VertexArray *va, const IndexBuffer *ib, const Shader *shader) const;
};

#endif
#ifndef RENDERERHEADERGUARD
#define RENDERERHEADERGUARD

#include "Debugger.hpp"

#include "VertexArrayObject.hpp"
#include "IndexBufferObject.hpp"
#include "Shader.hpp"

class Renderer {
    private:
    public:
        Renderer();
        ~Renderer();
        void Clear(unsigned int ClearBufferBit = GL_COLOR_BUFFER_BIT);
        void Draw(const VertexArrayObject *va, const IndexBufferObject *ib, const Shader *shader) const;
};

#endif
#include "GenericShape.h"

#include "Debugger.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "UtilFunc.hpp"

namespace MGLlib {

// non member functions
    void SetColorDefault(Shader* shader, ColorNDC RGBA){
        shader->SetUniform4f("u_Color", RGBA.R, RGBA.G, RGBA.B, RGBA.A);
    }

// static shape data
    const GenericShape::ShapeMapItem GenericShape::ShapeMap[__SHAPETYPECOUNT] = {
        // type                 // Name                         // GL_TYPE          // base IBO         // base, extends with
        {MGL_CUSTOM,            "[MGL_CUSTOM]",                 GL_TRIANGLES,       {0},                1, 1},
        {MGL_POINTS,            "[MGL_POINTS]",                 GL_POINTS,          {0},                1, 1},
        {MGL_LINES,             "[MGL_LINES]",                  GL_LINES,           {0, 1},             2, 2},
        {MGL_LINE_STRIP,        "[MGL_LINE_STRIP]",             GL_LINE_STRIP,      {0},                1, 1},
        {MGL_LINE_LOOP,         "[MGL_LINE_LOOP]",              GL_LINE_LOOP,       {0},                1, 1},
        {MGL_TRIANGLES,         "[MGL_TRIANGLES]",              GL_TRIANGLES,       {0, 1, 2},          3, 3},
        {MGL_TRIANGLE_STRIP,    "[MGL_TRIANGLE_STRIP]",         GL_TRIANGLE_STRIP,  {0, 1, 2},          3, 1},
        {MGL_TRIANGLE_FAN,      "[MGL_TRIANGLE_FAN | POLYGON]", GL_TRIANGLE_FAN,    {0, 1, 2},          3, 2},
        {MGL_QUADS,             "[MGL_QUADS]",                  GL_TRIANGLES,       {0, 1, 2, 2, 3, 0}, 4, 4},
        {MGL_QUAD_STRIP,        "[MGL_QUAD_STRIP]",             GL_TRIANGLES,       {0, 2, 3, 3, 1, 0}, 4, 2}
    };

    bool GenericShape::v_UseDefaultShader = true;

    std::vector<unsigned int> GenericShape::GenerateIndexBuffer(ShapeType ST, unsigned int VertexCount){
        if(VertexCount < ShapeMap[ST].baseCount){
            pWARNING("Not enough Vertexes for Shapetype: %s, given: %d, needed: %d", getName().c_str(), VertexCount, ShapeMap[ST].baseCount);
        }
        if((VertexCount - ShapeMap[ST].baseCount) % ShapeMap[ST].extendCount){
            pWARNING("Vertex count does not match shapetype: %s, given: %d, needed: %d, extends with: %d", getName().c_str(), VertexCount, ShapeMap[ST].baseCount, ShapeMap[ST].extendCount);
        }
        // creating index buffer
        std::vector<unsigned int> IndexBuffer;
        if(ST < MGL_TRIANGLE_FAN || ST >= __SHAPETYPECOUNT){
            for(unsigned int i = 0; i < VertexCount; i++){
                IndexBuffer.push_back(i);
            }
        }
        else{
            // is a square type
            unsigned int Loopcount;
            if(VertexCount < ShapeMap[ST].baseCount){
                Loopcount = 0;
            }
            else{
                Loopcount = (VertexCount - ShapeMap[ST].baseCount) / ShapeMap[ST].extendCount + 1;
            }
            for(unsigned int i = 0; i < Loopcount; i++){
                for(unsigned int j = 0; j < ShapeMap[ST].IndexBufferBase.size(); j++){
                    IndexBuffer.push_back(ShapeMap[ST].IndexBufferBase[j] + i * ShapeMap[ST].extendCount);
                }
            }
        }
        return IndexBuffer;
    }

    void GenericShape::GenerateGAS(Shader* shader, ShapeType ST, std::vector<float> VertexData, std::vector<unsigned int> VertexLayout){
        this->ST = ST;
        // error checking
        if((ST >= __SHAPETYPECOUNT) || (ST < 0)){
            pERROR("Unsupported Shapetype did you mean [MGL_CUSTOM]?");
        }
        VertexLength = 0;
        for(unsigned int i = 0; i < VertexLayout.size(); i++){
            VertexLength += VertexLayout[i];
        }
        // checking passed vertex count
        VertexCount = VertexData.size()/VertexLength;
        std::vector<unsigned int> IndexBuffer = GenerateIndexBuffer(ST, VertexCount);

        float* VBOtemp = Util::VecToArray<float>(VertexData);
        unsigned int* IBOtemp = Util::VecToArray<unsigned int>(IndexBuffer);
        unsigned int* VBOlayouttemp = Util::VecToArray<unsigned int>(VertexLayout);
        GAShape = new GenericAbstractShape<float>(
            shader, 
            VBOtemp, VertexData.size() * sizeof(float), 
            VBOlayouttemp, VertexLayout.size(),
            IBOtemp, IndexBuffer.size(), 
            ShapeMap[ST].GLtype
        );
        delete VBOtemp;
        delete IBOtemp;
        delete VBOlayouttemp;
    }

// constructor
    GenericShape::GenericShape(Shader* shader, ShapeType ST, std::vector<float> VertexData, std::vector<unsigned int> VertexLayout){
        GenerateGAS(shader, ST, VertexData, VertexLayout);
    }
    GenericShape::GenericShape(Shader* shader, ShapeType ST){
        GAShape = new GenericAbstractShape<float>(shader, ShapeMap[ST].GLtype);
    }
// destructor
    GenericShape::~GenericShape(){
        delete GAShape;
    }

// member functions

    void GenericShape::Draw(void){
        //printf("%d\n", use_texture);
        if(use_texture){
            if(CHtexture_on_render){
                this->GAShape->BindTexture();
            }
        }
        else{
            this->GAShape->DisableTexture();
        }
        if(CHcolor_on_render){
            SetColorNDC(this->color);
        }
        this->GAShape->Draw();
    }

    void GenericShape::Scale(float x, float y, float z){
        GAShape->GetShader()->Bind();
        this->GAShape->SetScale(x, y, z);
    }
    void GenericShape::SetColorNDC(ColorNDC RGBA){
        GAShape->GetShader()->Bind();
        this->color.R = RGBA.R;
        this->color.G = RGBA.G;
        this->color.B = RGBA.B;
        this->color.A = RGBA.A;

        this->SCF(this->GAShape->GetShader(), RGBA);
    }

    void GenericShape::SetColor(Color RGBA){
        ColorNDC RGBANDC;
        RGBANDC.R = (float)RGBA.R / 255.0f;
        RGBANDC.G = (float)RGBA.G / 255.0f;;
        RGBANDC.B = (float)RGBA.B / 255.0f;;
        RGBANDC.A = (float)RGBA.A / 255.0f;;
        SetColorNDC({RGBANDC.R, RGBANDC.G, RGBANDC.B, RGBANDC.A});
    }

// texture methods
    void GenericShape::SetTexture(Texture* texture, int slot, bool UseDefaultShader){
        this->GAShape->SetTexture(texture, slot, UseDefaultShader);
        use_texture = true;
    }
    void GenericShape::UseDefaultShader(bool v){
        this->v_UseDefaultShader = v;
    }
    void GenericShape::DisableTexture(){
        use_texture = false;
    }
    void GenericShape::EnableTexture(){
        use_texture = true;
    }
    
// position methods
    void GenericShape::SetPosition(glm::vec3 position){
        this->Position = position;
        this->ModelMat = glm::translate(glm::mat4(1.0f), Position);
    }
    glm::vec3 GenericShape::GetPosition(void){return this->Position;}
    glm::mat4 GenericShape::GetModelMat(void){return this->ModelMat;}
    void GenericShape::SetModelMat(glm::mat4 modelmatrix){this->ModelMat = modelmatrix;}

// data methods
    void GenericShape::SetIndexBuffer(std::vector<unsigned int> IndexBuffer){
        unsigned int *IndexBufferArray = new unsigned int[IndexBuffer.size()];
        for(unsigned int i = 0; i < IndexBuffer.size(); i++){
            IndexBufferArray[i] = IndexBuffer[i];
        }
        GAShape->SetIndexBuffer(IndexBufferArray, IndexBuffer.size());
        delete [] IndexBufferArray;
    }
    void GenericShape::SetVertexData(std::vector<float> VertexData, std::vector<unsigned int> VertexLayout){
        unsigned int *VertexLayoutArray = new unsigned int [VertexLayout.size()];
        VertexLength = 0;
        for(unsigned int i = 0; i < VertexLayout.size(); i++){
            VertexLength += VertexLayout[i];
        }
        GAShape->SetVertexLayout(VertexLayoutArray, VertexLayout.size());
        delete [] VertexLayoutArray;
        
        unsigned int *VertexBufferArray = new unsigned int[VertexData.size()];
        if((VertexData.size() % VertexLength)){
            #ifdef VBLAYOUTWARNING
                pWARNING("[WARNING]: Vertex Data does not match given layout of %d values", VertexLength);
            #endif
        }
        for(unsigned int i = 0; i < VertexData.size(); i++){
            VertexBufferArray[i] = VertexData[i];
        }
        GAShape->SetVertexBuffer(VertexBufferArray, sizeof(VertexBufferArray));
        delete [] VertexBufferArray;
    }

// getters
    std::string GenericShape::getName(){
        if(ST >= __SHAPETYPECOUNT){
            return "[MGL_ERR_TYPE]";
        }
        return this->ShapeMap[ST].ShapeName;
    }

    Shader* GenericShape::GetShader(void){
        return this->GAShape->GetShader();
    }
}


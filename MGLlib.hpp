#include <GL/glew.h>		// sudo apt-get install libglew-dev
#include <GLFW/glfw3.h>	// sudo apt-get install libglfw3-dev
#include <glm/glm.hpp>	// sudo apt-get install libglm-dev

//inc/Core/UtilFunc.hpp : 

#ifndef UTILFUNCHEADERGUARD
#define UTILFUNCHEADERGUARD

#include <random>
#include <cstdlib>
#include <ctime>

namespace MGLlib { 
namespace Util {

float toNDC(double in, double max);
float toPixel(double in, double max);
float fastsquare(float n);

// normalized color struct
typedef union __RGBA__ {
	struct {
		float r, g, b, a;
	};
	float *c;
} RGBA;

void quickrandomsetseed(void);
int GetQuickRandom(int min, int max); 

class RandomGenerator {
	private:
		std::mt19937 rng;
		uint32_t seed_val;

	public:
		RandomGenerator();
		~RandomGenerator();

		template <typename T>
		T GetRandom(T min, T max);
};


}} // namespace
#endif
//inc/Core/Debug/Debugger.hpp : 

#ifndef DEBUGGERHEADERGUARD
#define DEBUGGERHEADERGUARD

//#include DefineDebug.h"


#include <cassert>

namespace MGLlib { 

// Use (void) to silent unused warnings.
#define assertm(exp, msg) assert(((void)msg, exp))
#define GLCall(x) GLClearError();\
	x;\
	GLLogCall(#x, __FILE__, __LINE__)

void GLClearError();
void GLLogCall(const char* function, const char* file, int line);

} // namespace
#endif
//inc/Core/Debug/DefineDebug.h : 

#ifndef DEFINEDEBUGGERHEADERGUARD
#define DEFINEDEBUGGERHEADERGUARD

#include <iostream>
#define SHOWDEBUG
#define SHOWINFO
#define SHOWWARNING
//#define SHOWLOGSOURCE

#define SHADERWARNINGS

#define INDEXBUFFERWARNING
#define VBLAYOUTWARNING

#define CAMERAWARNINGS


#define SHADERDEBUG

#define SHOWGLVERSION
#define SHOWMSAA
#define SHOWINITINFO


#ifdef SHOWDEBUG
#define LOG(stream, type, format, ...){\
	fprintf(stream, "%-10s ", type);\
	fprintf(stream, format, ##__VA_ARGS__);\
}
#else
#define LOG(stream, type, format, ...)
#endif

#define pERROR(format, ...){\
	fprintf(stderr, "%-10s ", "[ERROR]:");\
	fprintf(stderr, format, ##__VA_ARGS__);\
	fprintf(stderr, "\n|- %-7s file: \"%s\" ; function: \"%s\" : line: %d\n", "in:", __FILE__, __FUNCTION__, __LINE__);\
}

#ifdef SHOWWARNING
	#ifdef SHOWLOGSOURCE
		#define pWARNING(format, ...){\
			LOG(stderr, "[WARNING]:", format, ##__VA_ARGS__);\
			fprintf(stderr, "\n|- %-7s file: \"%s\" ; function: \"%s\" : line: %d\n", "in:", __FILE__, __FUNCTION__, __LINE__);\
		}
		#else
		#define pWARNING(format, ...){\
			LOG(stderr, "[WARNING]:", format, ##__VA_ARGS__);\
			fprintf(stderr, "\n");\
		}
		#endif
	#else
	#define pWARNING(format, ...)
#endif

#ifdef SHOWINFO
#define pINFO(format, ...){\
	LOG(stdout, "[INFO]:", format, ##__VA_ARGS__);\
	printf("\n");\
}
#else
#define pINFO(format, ...)
#endif

#endif
//inc/Core/Primitives/IndexBufferObject.hpp : 

#ifndef IBOHEADERGUARD
#define IBOHEADERGUARD

namespace MGLlib {
namespace Primitives {
	class IndexBufferObject{
	private:
		unsigned int m_RendererID;
		unsigned int m_Count;

	public:
		IndexBufferObject(const unsigned int* data, unsigned int count);
		~IndexBufferObject();

		void Bind() const;
		void UnBind() const;

		inline unsigned int GetCount() const { return m_Count; }
	};
}
}
#endif
//inc/Core/Primitives/VertexBufferLayout.hpp : 

#ifndef VBOLAYOUTHEADERGUARD
#define VBOLAYOUTHEADERGUARD

//#include Debugger.hpp"

#include <vector>

namespace MGLlib { 
namespace Primitives { 

struct VertexBufferElement {
	unsigned int type;
	unsigned int count;
	unsigned char normalized;

	static unsigned int GetSizeOfType(unsigned int type){
		switch (type){
			case GL_BYTE:			return 1;
			case GL_SHORT:			return 2;
			case GL_INT:			return 4;
			case GL_FLOAT: 			return 4;
			case GL_DOUBLE:			return 8;
			case GL_UNSIGNED_BYTE: 	return 1;
			case GL_UNSIGNED_SHORT:	return 2;
			case GL_UNSIGNED_INT: 	return 4;
			
		}
		pERROR("unsupported buffer element type");
		return 0;
	}
};

class VertexBufferLayout{
private:
	std::vector<VertexBufferElement> m_Elements;
	unsigned int m_Stride;

public:
 	VertexBufferLayout();
 	~VertexBufferLayout();

	template<typename T>
	void Push(unsigned int count);

	inline const std::vector<VertexBufferElement> GetElements() const { return m_Elements; }
	inline unsigned int GetStride() const { return m_Stride; }
};


}} // namespace
#endif
//inc/Core/Primitives/Shader.hpp : 

#ifndef SHADER_HPP
#define SHADER_HPP

#include <unordered_map>
#include <string>

#include <glm/glm.hpp>

namespace MGLlib { 

class Shader 
{
private:   
    unsigned int m_RendererID;
    std::string m_filepath[3];
    std::unordered_map<std::string, int> m_uniformLocationCache;

    enum __ShaderType {
        SINGLE,
        Fragment,
        Vertex,
        ShaderCount
    };

    std::string ShaderType[ShaderCount-1] = {"Fragment", "Vertex"};
    
    struct ShaderProgramSource
    {
        std::string VertexSource;
        std::string FragmentSource;
    };

    // used for multiple shader files
    std::string ReadShaderFile(enum __ShaderType T);
    // used for single shader file
    ShaderProgramSource ParseShader();
    // compiles the shader
    unsigned int CompileShader(unsigned int type, const std::string& ShaderCode);
    // links the shader files
    void LinkShaders(unsigned int VertexShaderID, unsigned int FragmentShaderID);

    int GetUniformLocation(const std::string& name);

public:
    Shader(const char * vertex_file_path,const char * fragment_file_path);
    Shader(const char* shader_file_path);
    ~Shader();

    void Bind() const;
    void UnBind() const;

    // set uniforms
    void SetUniform1f(const std::string& name, float value);
    void SetUniform1i(const std::string& name, int value);
    void SetUniform1ui(const std::string& name, unsigned int value);
    void SetUniform1fv(const std::string& name, int count, const float *value);
    void SetUniform1iv(const std::string& name, int count, const int *value);
    void SetUniform1uiv(const std::string& name, int count, const unsigned int *value);
    void SetUniform2f(const std::string& name, float v0, float v1);
    void SetUniform2i(const std::string& name, int v0, int v1);
    void SetUniform2ui(const std::string& name, unsigned int v0, unsigned int v1);
    void SetUniform2fv(const std::string& name, int count, const float *value);
    void SetUniform2iv(const std::string& name, int count, const int *value);
    void SetUniform2uiv(const std::string& name, int count, const unsigned int *value);
    void SetUniform3f(const std::string& name, float v0, float v1, float v2);
    void SetUniform3i(const std::string& name, int v0, int v1, int v2);
    void SetUniform3ui(const std::string& name, unsigned int v0, unsigned int v1, unsigned int v2);
    void SetUniform3fv(const std::string& name, int count, const float *value);
    void SetUniform3iv(const std::string& name, int count, const int *value);
    void SetUniform3uiv(const std::string& name, int count, const unsigned int *value);
    void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
    void SetUniform4i(const std::string& name, int v0, int v1, int v2, int v3);
    void SetUniform4ui(const std::string& name, unsigned int v0, unsigned int v1, unsigned int v2, unsigned int v3);
    void SetUniform4fv(const std::string& name, int count, const float *value);
    void SetUniform4iv(const std::string& name, int count, const int *value);
    void SetUniform4uiv(const std::string& name, int count, const unsigned int *value);

    void SetuniformMat4f(const std::string& name, glm::mat4 matrix);
};

} // namespace
#endif

//inc/Core/Primitives/VertexArrayObject.hpp : 

#ifndef VAOHEADERGUARD
#define VAOHEADERGUARD

//#include VertexBufferObject.hpp"
//#include VertexBufferLayout.hpp"

namespace MGLlib { 
namespace Primitives { 

class VertexArrayObject{
private:
	unsigned int m_RendererID;
public:
	VertexArrayObject();
	~VertexArrayObject();

	void AddBuffer(const VertexBufferObject* vb, const VertexBufferLayout* layout);

	void Bind() const;
	void UnBind() const;
};

}} // namespace

#endif
//inc/Core/Primitives/VertexBufferObject.hpp : 

#ifndef VBOHEADERGUARD
#define VBOHEADERGUARD

namespace MGLlib { 
namespace Primitives { 

class VertexBufferObject{
private:
	unsigned int m_RendererID;

public:
	VertexBufferObject(const void* data, unsigned int size);
 	~VertexBufferObject();

	void Bind() const;
	void UnBind() const;

};

}} // namespace
#endif
//inc/Core/Primitives/Texture.hpp : 

#ifndef TEXTUREHEADERGUARD
#define TEXTUREHEADERGUARD

#include <string>
//#include Debugger.hpp"

namespace MGLlib {

class Texture
{
private:
	unsigned int m_RendererID;
	std::string m_FilePath;
	unsigned char* m_LocalBuffer;
	int m_Width, m_Height, m_BPP;
public:
 	Texture(const std::string& path);
 	~Texture();

	// slot determines the texture binding slot
	void Bind(unsigned int slot = 0) const;
	void Unbind() const;

	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }
};

} // namespace
#endif
//inc/Core/Primitives/DrawBuffer.hpp : 

#ifndef VERTEXBUFFERSTRUCTHEADERGUARD
#define VERTEXBUFFERSTRUCTHEADERGUARD

#include <vector>
//#include VertexBufferLayout.hpp"
//#include IndexBufferObject.hpp"

namespace MGLlib { 
namespace Primitives {

template<typename type>
class DrawBuffer{
	private:
		unsigned int* layout = nullptr;
		unsigned int  layoutSize;
		unsigned int* IndexBufferData = nullptr;
		unsigned int  IndexBufferSize;

		unsigned int VertexLength;
		unsigned int VertexCount;
		
		type* VertexDataArray = nullptr;

	// private methods
		void SetVertexLength(std::vector<unsigned int> layout){
			// get Vertex Length from layout
			VertexLength = 0;
			for(unsigned int i = 0; i < layout.size(); i++){
				VertexLength += layout[i];
			}
		}
		void DefaultConstructor(std::vector<type> array, std::vector<unsigned int> IndexBuffer, std::vector<unsigned int> layout){	
			// create space for data
			VertexCount = array.size() / VertexLength;
			VertexDataArray = new float[VertexCount*VertexLength];
			// set vertex data
			SetData(array);
			// create IBO
			SetIBO(IndexBuffer);
			// create VBO layout
			SetLayout(layout);
		}

	public:
	// constructors
		DrawBuffer(unsigned int count, std::vector<unsigned int> IndexBuffer, std::vector<unsigned int> layout = {3, 3, 4}){
			SetVertexLength(layout);
			std::vector<type> data;
			for(int i = 0; i < count * VertexLength; i++){
				data.push_back(nullptr);
			}
			DefaultConstructor(data, IndexBuffer, layout);
		}
		DrawBuffer(std::vector<type> array, std::vector<unsigned int> IndexBuffer, std::vector<unsigned int> layout = {3, 3, 4}){
			SetVertexLength(layout);
			DefaultConstructor(array, IndexBuffer, layout);
		}
		~DrawBuffer(void){
			delete [] VertexDataArray;
			delete [] IndexBufferData;
			delete [] layout;
		}

	// setters
		void SetData(const std::vector<type> array){
			if((array.size() % VertexLength)){
				#ifdef VBLAYOUTWARNING
					pWARNING("Vertex Data does not match given layout of %d values", VertexLength);
				#endif
			}
			for(unsigned int i = 0; i < array.size(); i++){
				this->VertexDataArray[i] = array[i];
			}
		}
		void SetIBO(std::vector<unsigned int> IndexBuffer){
			if(this->IndexBufferData){
				delete this->IndexBufferData;
			}
			IndexBufferData = new unsigned int[IndexBuffer.size()];
			for(unsigned int i = 0; i < IndexBuffer.size(); i++){
				IndexBufferData[i] = IndexBuffer[i];
			}
			IndexBufferSize = IndexBuffer.size();
		}
		void SetLayout(std::vector<unsigned int> layout){
			if(this->layout){
				delete this->layout;
				SetVertexLength(layout);
			}
			this->layout = new unsigned int[layout.size()];
			for(unsigned int i = 0; i < layout.size(); i++){
				this->layout[i] = layout[i];
			}
			layoutSize = layout.size();
		}

	// getters
		type* GetData(void){
			return VertexDataArray;
		}
		unsigned int* GetIBO(void){
			return IndexBufferData;
		}
		unsigned int GetIBOSize(void){
			return IndexBufferSize;
		}
		unsigned int* GetLayout(void){
			return layout;
		}
		unsigned int GetLayoutSize(void){
			return layoutSize;
		}

		unsigned int getSize(){
			return VertexCount * VertexLength * sizeof(type);
		}

		unsigned int getVertexCount(){
			return VertexCount;
		}
};

}} // namespace

#endif
//inc/Core/Shapes/Shape.h : 

#ifndef SHAPEHEADERGUARD
#define SHAPEHEADERGUARD

//#include GenericShape.h"
//#include GL/glew.h"
//#include GLFW/glfw3.h"

namespace MGLlib{

class Shape : public GenericShape
{
	protected:
	unsigned int VertexCount = 0;
	std::vector<std::array<float, 3>> Model;
	std::vector<std::array<float, 2>> TextureCoords;
	std::vector<std::array<float, 4>> ColorPerVector;
	
	bool DataChanged = true;

	GLFWwindow* window;
	Shader* shader;

	void UpdateVectorBuffers();
	void UpdateVertexCount(std::vector<float> vector, int VertexSize);
	void SetData(void);

	public:
		Shape(ShapeType ST, Shader* shader);
		Shape(ShapeType ST, Shader* shader, GLFWwindow* window);
		~Shape();

		void UpdateData(void);
		void Draw(void);

		void SetModelNDC(std::vector<std::array<float, 3>> Model){this->Model = Model;}
		void SetModel3D(std::vector<float> Model);
		void SetModel2D(std::vector<float> Model);
		void SetTextureCoord(std::vector<float> TextureCoords);
		void SetColor(std::array<float, 4> Color);
		void SetColorNDC(std::array<float, 4> Color);
		void SetColorVec(unsigned int Vertex, std::array<float, 4> Color);
		void SetColorVecNDC(unsigned int Vertex, std::array<float, 4> Color);
		void SetVertex2D(unsigned int Vertex, std::array<float, 2> coor);
		void SetVertex3D(unsigned int Vertex, std::array<float, 3> coor);
		void SetVertex2D_NDC(unsigned int Vertex, std::array<float, 2> coor);
		void SetVertex3D_NDC(unsigned int Vertex, std::array<float, 3> coor);

		void SetWindowContext(GLFWwindow* window){this->window = window;}

		std::vector<std::array<float, 3>> GetModel(void){return this->Model;}
};

}
#endif
//inc/Core/Shapes/GenericAbstractShape.h : 

#ifndef GASHEADERGUARD
#define GASHEADERGUARD

//#include DrawBuffer.hpp"

//#include VertexArrayObject.hpp"
//#include VertexBufferObject.hpp"
//#include IndexBufferObject.hpp"

//#include Texture.hpp"
//#include Shader.hpp"

//#include Debugger.hpp"

namespace MGLlib { 

template<typename type>
class GenericAbstractShape{
	public:
		GenericAbstractShape(Shader* shader, unsigned int DrawType = GL_TRIANGLES);
		GenericAbstractShape(const GenericAbstractShape& GAS);
		GenericAbstractShape(Shader* shader, Primitives::DrawBuffer<type>* data, unsigned int DrawType = GL_TRIANGLES);
		~GenericAbstractShape();

		void Draw(void);
		void RenderTextures(void);
		void SetTexture(Texture* texture, int slot = 0, bool UseDefaultShader = true);
		void DisableTexture(void);
		void EnableTexture(void);
		void BindTexture(void);
		void SetIndexBuffer(unsigned int* IndexBuffer, unsigned int count);
		void SetVertexBuffer(const void* data, unsigned int size);
		void SetVertexLayout(unsigned int* layout, unsigned int count);
		void SetScale(type x, type y, type z);
		void SetShader(Shader* shader);
		Shader* GetShader(void);

	private:
		Primitives::VertexArrayObject* 	VA 		= nullptr;
		Primitives::VertexBufferObject* VB 		= nullptr;
		Primitives::VertexBufferLayout* Vlayout = nullptr;
		Primitives::IndexBufferObject* 	IB		= nullptr;
		Texture* 						texture = nullptr;
		unsigned int 					TextureSlot = 0;
		bool 							UseDefaultShader = false;
		Shader*							shader	= nullptr;
		// used for when no IBO is given
		unsigned int					Vertices;
		unsigned int					DrawType;
};


// template implementation

template<typename type>
GenericAbstractShape<type>::GenericAbstractShape(Shader* shader, unsigned int DrawType)
	: VA(new Primitives::VertexArrayObject()), DrawType(DrawType)
{
	float data[] = {0.0f};
	VB = new Primitives::VertexBufferObject(data, sizeof(float));
	Vlayout = new Primitives::VertexBufferLayout();
	this->shader = shader;
}

template<typename type>
GenericAbstractShape<type>::GenericAbstractShape(const GenericAbstractShape& GAS){
	this->VA = GAS.VA;
	this->Vlayout = GAS.Vlayout;
	this->IB = GAS.IB;
	this->texture = GAS.texture;
	this->shader = GAS.shader;
	this->use_texture = GAS.use_texture;
}

template<typename type>
GenericAbstractShape<type>::GenericAbstractShape(Shader* shader, Primitives::DrawBuffer<type>* data, unsigned int DrawType)
	: VA(new Primitives::VertexArrayObject()), DrawType(DrawType)
{	
	this->shader = shader;
	VB = new Primitives::VertexBufferObject(data->GetData(), data->getSize());
	IB = new Primitives::IndexBufferObject(data->GetIBO(), data->GetIBOSize());
	Vlayout = new Primitives::VertexBufferLayout();
	unsigned int* layoutlocal = data->GetLayout();
	for(unsigned int i = 0; i < data->GetLayoutSize(); i++){
		Vlayout->Push<type>(layoutlocal[i]);
	}
	VA->AddBuffer(VB, Vlayout);
}

template<typename type>
GenericAbstractShape<type>::~GenericAbstractShape(){
	delete VA;
	delete VB;
	delete IB;
	delete Vlayout;
}

template<typename type>
void GenericAbstractShape<type>::SetTexture(Texture* texture, int slot, bool UseDefaultShader){
	this->texture = texture;
	this->TextureSlot = slot;
	this->UseDefaultShader = UseDefaultShader;
}

template<typename type>
void GenericAbstractShape<type>::DisableTexture(void){
	shader->Bind();
	//texture->Unbind();
	shader->SetUniform1i("u_Use_Texture", false); // disable textures
}

template<typename type>
void GenericAbstractShape<type>::EnableTexture(void){
	shader->Bind();
	shader->SetUniform1i("u_Use_Texture", true); // disable textures
}

template<typename type>
void GenericAbstractShape<type>::BindTexture(void){
	shader->Bind();
	this->texture->Bind(TextureSlot);
	if(UseDefaultShader){
		shader->SetUniform1i("u_Texture", TextureSlot);
		shader->SetUniform1i("u_Use_Texture", true); // enable textures
	}
}

template<typename type>
void GenericAbstractShape<type>::SetIndexBuffer(unsigned int* IndexBuffer, unsigned int count){
	if(IB){
		delete IB;
	}
	this->IB = new Primitives::IndexBufferObject(IndexBuffer, count); 
}
template<typename type>
void GenericAbstractShape<type>::SetVertexBuffer(const void* data, unsigned int size){
	if(VB){
		delete VB;
	}
	if(VA){
		delete VA;
	}
	VA = new Primitives::VertexArrayObject();
	VB = new Primitives::VertexBufferObject(data, size);
	VA->AddBuffer(VB, Vlayout);
}

template<typename type>
void GenericAbstractShape<type>::SetVertexLayout(unsigned int* layout, unsigned int count){
	if(Vlayout){
		delete Vlayout;
	}
	if(VA){
		delete VA;
	}
	VA = new Primitives::VertexArrayObject();
	Vlayout = new Primitives::VertexBufferLayout();
	unsigned int* layoutlocal = layout;
	for(unsigned int i = 0; i < count; i++){
		Vlayout->Push<type>(layoutlocal[i]);
	}
	VA->AddBuffer(VB, Vlayout);
}

template<typename type>
void GenericAbstractShape<type>::SetShader(Shader* shader){
	this->shader = shader;
}

template<typename type>
void GenericAbstractShape<type>::SetScale(type x, type y, type z){
	shader->SetUniform4f("u_Scale", x, y, z, 1.0);
}

template<typename type>
Shader* GenericAbstractShape<type>::GetShader(void){
	return this->shader;
}

template<typename type>
void GenericAbstractShape<type>::Draw(void){
	shader->Bind();
	VA->Bind();
	IB->Bind();
	GLCall(glDrawElements(DrawType, IB->GetCount(), GL_UNSIGNED_INT, nullptr));
}

} // namespace

#endif

//inc/Core/Shapes/GenericShape.h : 

#ifndef GENERICSHAPEHEADER
#define GENERICSHAPEHEADER

#include <vector>
#include <array>
#include <string>
//#include GenericAbstractShape.h"
//#include DrawBuffer.hpp"

namespace MGLlib{

	struct ColorNDC {
		float R = 1.0f;
		float G = 1.0f;
		float B = 1.0f;
		float A = 1.0f;
	};

	struct Color {
		int R = 255;
		int G = 255;
		int B = 255;
		int A = 255;
	};

	enum ShapeType {
		MGL_CUSTOM,
		MGL_POINTS,
		MGL_LINES,
		MGL_LINE_STRIP,
		MGL_LINE_LOOP,
		MGL_TRIANGLES,
		MGL_TRIANGLE_STRIP,
		MGL_TRIANGLE_FAN,
		MGL_POLYGON = MGL_TRIANGLE_FAN,
		MGL_QUADS,
		MGL_QUAD_STRIP,
		// keep this as last type
		__SHAPETYPECOUNT
	};

typedef void (*SetColorFunc)(Shader* shader, ColorNDC RGBA);
void SetColorDefault(Shader* shader, ColorNDC RGBA);

class GenericShape
{
	private:
	struct ShapeMapItem{
		ShapeType ST;
		std::string ShapeName;
		unsigned int GLtype;
		std::vector<unsigned int> IndexBufferBase;
		unsigned int baseCount;
		unsigned int extendCount;
	};
	static const ShapeMapItem ShapeMap[__SHAPETYPECOUNT];
	static bool v_UseDefaultShader;

	unsigned int VertexLength;	// how many floats in 1 vertex
	unsigned int VertexCount;	// how many vertexes in the buffer

	bool use_texture = false;
	bool CHcolor_on_render = true;
	bool CHtexture_on_render = true;

protected:
	ShapeType ST;

	GenericAbstractShape<float>* GAShape;
	SetColorFunc SCF = SetColorDefault;
	ColorNDC color = {1.0f, 1.0f, 1.0f};

	glm::vec3 Position = glm::vec3(0.0f);

	void GenerateGAS(Shader* shader, ShapeType ST, std::vector<float> VertexData, std::vector<unsigned int> VertexLayout = {3});
	std::vector<unsigned int> GenerateIndexBuffer(ShapeType ST, unsigned int VertexCount);

	public:
		//! @brief takes vector of points, layout should describe the layout of 1 vector
		//! @remark VertexLayout example: vector {X, Y, Z, R, G, B, A} layout : {3, 4}
		GenericShape(Shader* shader, ShapeType ST, std::vector<float> VertexData, std::vector<unsigned int> VertexLayout = {3});
		//! @brief this constructor is used by the Shape class
		GenericShape(Shader* shader, ShapeType ST);
		~GenericShape();

		//! @brief returns the name of the shape
		std::string getName();
		
		//! @brief draws the shape
		//! @remark Before drawing bind the right shader
		void Draw(void);

		//! @brief sets scale of shape
		void Scale(float x, float y, float z = 1.0f);
		//! @brief Sets the color of the model
		//! @remark Color value between 0.0 and 1.0
		void SetColorNDC(ColorNDC RGBA);
		//! @brief Sets the color of the model
		//! @remark Color value between 0 and 255
		void SetColor(Color RGBA);
		//! @brief Sets the "SetColor" function, usefull when using custom shaders
		//! @remark this function should accept RGBA values between 0.0 and 1.0
		void SetSetColorFunc(SetColorFunc SCF);
		//! @brief When turned off the shape uses the color of last drawn shape
		void change_color_on_render(bool v){this->CHcolor_on_render = v;}
		//! @brief When turned off the shape uses the texture of the last drawn shape
		void change_texture_on_render(bool v){this->CHtexture_on_render = v;}
		//! @brief Sets the texture of the shape
		//! @remark When using a custom shader disable the UseDefaultShader
		//! @remark Or disable it globally by using UseDefaultShader()
		void SetTexture(Texture* texture, int slot = 0, bool UseDefaultShader = v_UseDefaultShader);
		//! @brief Sets the default value for all GenericShape SetTexture function's "UseDefaultShader" parameter
		void UseDefaultShader(bool v);
		//! @brief Disables the set texture
		void DisableTexture();
		//! @brief Enables the set texture
		void EnableTexture();
		
		//! @brief Sets the position of the object relative to the world
		void SetPosition(std::array<float, 3> position);
		void SetPosition(glm::vec3 position);
		//! @brief returns the objects position relative to the world
		glm::vec3 GetPosition(void);
		//! @brief returns the objects model matrix
		glm::mat4 GetModelMat(void);

		//! @brief Sets vertex data
		//! @remark This function is slow, if you only want to move the model use SetPosition();
		void SetVertexData(std::vector<float> VertexData, std::vector<unsigned int> VertexLayout = {3});
		//! @brief Sets the Index Buffer of the shape
		//! @remark Call this function when using a [MGL_CUSTOM] with a special indexbufferobject;
		void SetIndexBuffer(std::vector<unsigned int> IndexBuffer);

		Shader* GetShader(void);
};

}	// namespace

#endif
//inc/Core/3Drender/Camera.h : 

#ifndef CAMERAHEADERGUARD
#define CAMERAHEADERGUARD

//#include DefineDebug.h"


#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <array>

namespace MGLlib { 

class Camera
{
	public:
		Camera(std::array<float, 3>  pos, std::array<float, 3> lookat);
		~Camera();

		void Position(std::array<float, 3> pos);
		void Lookat(std::array<float, 3> lookat);
		void SetUp(std::array<float, 3> up);
		void SetFOV(float FOV);
		void SetPlane(float NearPlane, float FarPlane);

		void UpdateAspectRatio(float width, float height);
		void UpdatePerspective(void);
		void UpdateCamera(void);
		bool CheckUpdated(void);

		float* getPosition(void) { return pos; }
		float* getLookat(void) { return lookat; }
		glm::mat4 getProjectionMatrix(void){return pers;}
		glm::mat4 getViewMatrix(void){return view;}
	private:
		glm::mat4 pers;
		glm::mat4 view;
		float pos[3], lookat[3], up[3];
		float FOV;
		float aspectratio;
		std::array<float, 2> Zplane;
		bool updated = true;

};

} // namespace
#endif
//inc/Shapes/Cube.hpp : 

#ifndef CUBEHEADERGUARD
#define CUBEHEADERGUARD

//#include Shape.h"

namespace MGLlib
{
	class Cube : public Shape{
	private:
		bool _hollow;
		// list of vertices

	public:
		Cube(Shader* shader, bool hollow = false);
		~Cube();

		// set the cube's material to given material
		//void SetMaterial(GLMaterialTracker *M);
		// set's the cube's material to side 0
		//void SetMaterialShape(void);
		// get side 0's material
		//GLMaterialTracker*& getMaterial(void);

		void SetCube(float x, float y, float z, float width, float height, float depth);
		void SetCube(float x0, float y0, float z0, 
					float x1, float y1, float z1, 
					float x2, float y2, float z2,
					float x3, float y3, float z3,
					float x4, float y4, float z4,
					float x5, float y5, float z5,
					float x6, float y6, float z6,
					float x7, float y7, float z7);
		void SetCube(float c[8][3]);
		void SetCube(Cube *C);
		//   5
		// 0 1 2 3
		//   4
		void SetSideColor(int i, unsigned char R, unsigned char G, unsigned char B, unsigned char A = 255);
		//GLMaterialTracker*& getSideMaterial(int i);
		//void setSideMaterial(int i, GLMaterialTracker *M);
		Shape* getSides(void);
	};
	
} // namespace MGLlib


#endif
//inc/Shapes/Rectangle.hpp : 

#ifndef RECTANGLEHEADERGUARD
#define RECTANGLEHEADERGUARD

//#include Shape.h"

namespace MGLlib
{
	class Rectangle : public Shape
	{
		float width, height, depth;

		public:
			Rectangle(Shader* shader);
			void MakeShape(float x, float y, float z, float width, float height, float depth, bool tild = false);
			~Rectangle();
	};

} // namespace MGLlib


#endif
//inc/Shapes/Circle.hpp : 

#ifndef CIRCLEHEADERGUARD
#define CIRCLEHEADERGUARD

//#include Shape.h"

namespace MGLlib
{
	class Circle : public Shape {
		float Mx, My;
		
		public:
		Circle(Shader* shader);
		void MakeShape(float Mx, float My, float Z, float Radius, int Sectors);
		~Circle();
	};
} // namespace MGLlib


#endif
//inc/Window.h : 

#ifndef WINDOWHEADERGUARD
#define WINDOWHEADERGUARD

		// sudo apt-get install libglew-dev
		// sudo apt-get install libglfw3-dev	
#include <glm/glm.hpp>		// sudo apt-get install libglm-dev

//#include ShapeHandler.h"

namespace MGLlib {
	
	class Window : public ShapeHandler
	{
		private:
			// used for enabling functions
			typedef void (Window::*WindowVoidFunc)(void);

			glm::vec4 ClearColor = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
			//TODO: add lights

			bool use_custom_callback = true;

			void (*render)(void);
			void (*update)(void);

			void enableLighting(void);
			void enable3D(void);
			void enableAA(void);
			void enableAlpha(void);
			void enableVsync(void);

		public:
			typedef enum __features__ {
				F_3D 			= 0x0001,
				F_AA 			= 0x0002,
				F_ALPHA 		= 0x0004,
				F_LIGHTING 		= 0x0008,
				F_VSYNC 		= 0x0010,
			} feature;

			Window(const char* title, int width = 500, int height = 500, int offsetx = 100, int offsety = 100);
			~Window();

			/*! @brief enable extra window features
			 *  @param[in] f enable multiple features by | them together
		 	 *  @remark example: WindowEnableFeatures(F_3D | F_AA);
			 *  call before Show();
			 */
			void WindowEnableFeatures(unsigned int f);
			/*! @brief set Render and Update callback loop
			 *  @param[in] render draws objects
			 *  @param[in] update changes objects values
			 */
			void SetCallback(void (*render)(void), void (*update)(void) = nullptr);
			/*! @brief use show when using the default render callback loop.
			 *	Clears screen, pollsevents and Calls set Render function after Update function.
			 *	Closes window on [x] button and ESC key.
			 * 	For other behaviour create custom own GLFW3 loop.
			 */
			void Show(void);
			void Exit(void);
			
			void SetClearColor(glm::vec4 RGBA){this->ClearColor = RGBA;}
			void getDimensions(int *width, int *height){return glfwGetWindowSize(window, width, height);}
			/*! @brief Use this when a function requires a GLFW3 Window
			 *	THIS IS NOT THE WINDOW OBJECT!
			 */
			GLFWwindow* getGLFWwindow(void){return this->window;}
	};

} // namespace
#endif
//inc/Controllers/ShapeHandler.h : 

#ifndef SHAPEHANDLER
#define SHAPEHANDLER

//#include GL/glew.h"
//#include GLFW/glfw3.h"

//#include Camera.h"
//#include Shader.hpp"		// for default shader

//#include GenericShape.h"
//#include Shape.h"

//#include Rectangle.hpp"
//#include Circle.hpp"
//#include Cube.hpp"

namespace MGLlib {

// Determines the conversion based on which length
enum C2NDC : int {
	NDC_XY		= -1,
	NDC_XYZ		= -2,
	NDC_RGB 	= -3,
	NDC_RGBA 	= -4,
};

class ShapeHandler
{
	private:
		glm::mat4 vp;

		void SetProjectionView(void);

	protected:
		Camera*		cam		= nullptr;
		Shader*		shader	= nullptr;
		GLFWwindow* window	= nullptr;
		bool		_3Drendering = false;
		bool		_OrthoRender = false;

	public:
		ShapeHandler();
		~ShapeHandler();

		void Enable3Drender(void);
		void Disable3Drender(void);
		void Enableortho(void);
		void Disableortho(void);
		
		void InitHandler(GLFWwindow* window);

		void SetDefaultShader(Shader* shader);
		//! @brief draws the shape
		//! @param DrawFunc Performs this function between binding the shader and drawing the Shape
		void Draw(GenericShape* Shape, void (*DrawFunc)(void) = nullptr);
		void Draw(Shape* Shape, void (*DrawFunc)(void) = nullptr);

		Camera* GetCamera(void);
		Shader* GetGlobalShader(void);

		//! @brief creates a Shape based on coordinates between -1 and 1
		GenericShape* CreateGenShapeNDC(ShapeType ST, std::vector<float> VertexBuffer, std::vector<unsigned int> VertexLayout = {3});

		//! @brief creates a Shape based on coordinates between pixels, origin is at the bottom left
		//! @param VertexLayout pass a value of type "enum C2NDC" or a positive int value
		GenericShape* CreateGenShape(ShapeType ST, std::vector<float> VertexBuffer, std::vector<int> VertexLayout = {NDC_XYZ});

		//! @brief returns an empty shape object
		//! @remark shape data should be set after calling this function
		Shape* CreateShape(ShapeType ST);

		//! @brief returns a rectangle object
		Rectangle* CreateRectangle(float x, float y, float z, float width, float height, float depth, bool tild = false);
		Rectangle* CreateRectangle(float x, float y, float width, float height);
		Circle* CreateCircle(float x, float y, float z, float Radius, int Sectors);
		Circle* CreateCircle(float x, float y, float Radius, int Sectors);
		Cube* CreateCube(float x, float y, float z, float width, float height, float depth);
};

} // namespace
#endif
//inc/Controllers/ImGuiHelper.h : 

#ifndef IMGUIHELPERHEADERGUARD
#define IMGUIHELPERHEADERGUARD

//#include ImGui/imgui.h"
//#include ImGui/imgui_impl_glfw.h"
//#include ImGui/imgui_impl_opengl3.h"	// TODO: check what is neede from example files

namespace MGLlib{
	class Window;

	class ImGuiHelper 
	{
		private:
		// GL 3.0 + GLSL 130
   		const char* glsl_version = "#version 330";

		public:
			ImGuiHelper(Window *w);
			~ImGuiHelper();

			void newFrame();
			void render();

			//! @brief Call before exiting window object
			void Exit(void);
	};
} // namespace

#endif

#include <vector>
#include <fstream>
#include <sstream>

#include <GL/glew.h>

#include "Shader.hpp"
#include "Debugger.hpp"

namespace MGLlib { 
namespace Primitives { 

void Shader::Bind() const{
	GLCall(glUseProgram(m_RendererID));
}

void Shader::UnBind() const{
	GLCall(glUseProgram(0));
}

// loads the shader with 2 files
Shader::Shader(const char* vertex_file_path, const char* fragment_file_path) : m_RendererID(0) {
	this->m_filepath[SINGLE] = "";
	this->m_filepath[Vertex] = vertex_file_path;
	this->m_filepath[Fragment] = fragment_file_path;

	this->m_RendererID = glCreateProgram();
	unsigned int VertexShaderID = CompileShader(GL_VERTEX_SHADER, ReadShaderFile(Vertex));
	unsigned int FragmentShaderID = CompileShader(GL_FRAGMENT_SHADER, ReadShaderFile(Fragment));

	LinkShaders(VertexShaderID, FragmentShaderID);
}
// loads the shader with 1 file
Shader::Shader(const char* shader_file_path) : m_RendererID(0){
	this->m_filepath[SINGLE] = shader_file_path;
	this->m_filepath[Vertex] = "";
	this->m_filepath[Fragment] = "";

	this->m_RendererID = glCreateProgram();
	ShaderProgramSource SPS = ParseShader();
	unsigned int VertexShaderID = CompileShader(GL_VERTEX_SHADER, SPS.VertexSource);
	unsigned int FragmentShaderID = CompileShader(GL_FRAGMENT_SHADER, SPS.FragmentSource);

	LinkShaders(VertexShaderID, FragmentShaderID);
}

Shader::~Shader(){
	GLCall(glDeleteProgram(m_RendererID));
}

int Shader::GetUniformLocation(const std::string& name){
	if (m_uniformLocationCache.find(name) != m_uniformLocationCache.end()){
		return m_uniformLocationCache[name];
	}

	GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
	if (location == -1){
		#ifdef SHADERWARNINGS
			std::cout << "[Shader Warning]: uniform " << name << " does not exist.\n" << std::endl;
		#endif
	}
	else{
		m_uniformLocationCache[name] = location;
	}
	return location;
}

void Shader::SetUniform1f(const std::string& name, float value){
	GLCall(glUniform1f(GetUniformLocation(name), value));
}

// used for multiple shader files
std::string Shader::ReadShaderFile(enum __ShaderType T){
	// Read the Vertex Shader code from the file
	std::string ShaderCode;
	std::ifstream ShaderStream(m_filepath[T]);
	if(ShaderStream.is_open()){
		std::stringstream sstr;
		sstr << ShaderStream.rdbuf();
		ShaderCode = sstr.str();
		ShaderStream.close();
	}else{
		std::cout << "[Shader Error]: Impossible to open " << m_filepath[T] << ". Are you in the right directory ?" << std::endl;
		getchar();
		return 0;
	}
	return ShaderCode;
}
// used for single shader file
Shader::ShaderProgramSource Shader::ParseShader(){
	std::ifstream stream(m_filepath[0]);

	enum ShaderType{
		NONE = -1, VERTEX = 0, FRAGMENT = 1,
		SHADERTYPES
	};

	std::string line;
	std::stringstream ss[SHADERTYPES];	// 1 for vertex, 1 for fragment
	ShaderType type = NONE;
	while (getline(stream, line))
	{
		if(line.find("#shader") != std::string::npos){
			if(line.find("vertex") != std::string::npos){
				// set mode to vertex
				type = VERTEX;
			}
			else if (line.find("fragment") != std::string::npos){
				// set mode to fragment
				type = FRAGMENT;
			}
			// else if for other types of shaders
		}
		else{
			ss[(int)type] << line << '\n';
		}
	}

	return { ss[0].str(), ss[1].str() };
}
// compiles the shader
unsigned int Shader::CompileShader(unsigned int type, const std::string& ShaderCode){
	// Create the shaders
	GLuint ShaderID = glCreateShader(type);

	GLint Result = GL_FALSE;
	int InfoLogLength;

	char const *SourcePointer = ShaderCode.c_str();
	glShaderSource(ShaderID, 1, &SourcePointer , NULL);
	glCompileShader(ShaderID);

	// Check Vertex Shader
	glGetShaderiv(ShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(ShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> ShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(ShaderID, InfoLogLength, NULL, &ShaderErrorMessage[0]);
		std::cout << &ShaderErrorMessage[0] << std::endl;
	}

	#ifdef SHADERDEBUG
		pINFO("[SHADER]: Compiled %s shader", ShaderType[type - 0x8B30].c_str());
	#endif
	return ShaderID;
}
// links the shader files
void Shader::LinkShaders(unsigned int VertexShaderID, unsigned int FragmentShaderID){
	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Link the program
	#ifdef SHADERDEBUG
		pINFO("[SHADER]: Linking program");
	#endif

	glAttachShader(m_RendererID, VertexShaderID);
	glAttachShader(m_RendererID, FragmentShaderID);
	glLinkProgram(m_RendererID);

	// Check the program
	glGetProgramiv(m_RendererID, GL_LINK_STATUS, &Result);
	glGetProgramiv(m_RendererID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> ProgramErrorMessage(InfoLogLength+1);
		glGetProgramInfoLog(m_RendererID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		std::cout << &ProgramErrorMessage[0] << std::endl;
	}

	
	glDetachShader(m_RendererID, VertexShaderID);
	glDetachShader(m_RendererID, FragmentShaderID);
	
	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);
}


// UNIFORM SETTING CODE STARTS HERE

void Shader::SetUniform1i(const std::string& name, int value){
	// HACK: dont debug, flippin breaks for some reason, TODO: test on a different arcitecture?
	glUniform1i(GetUniformLocation(name), value);
}

void Shader::SetUniform1ui(const std::string& name, unsigned int value){
	GLCall(glUniform1ui(GetUniformLocation(name), value));
}

void Shader::SetUniform1fv(const std::string& name, int count, const float *value){
	GLCall(glUniform1fv(GetUniformLocation(name), count, value));
}

void Shader::SetUniform1iv(const std::string& name, int count, const int *value){
	GLCall(glUniform1iv(GetUniformLocation(name), count, value));
}

void Shader::SetUniform1uiv(const std::string& name, int count, const unsigned int *value){
	GLCall(glUniform1uiv(GetUniformLocation(name), count, value));
}

void Shader::SetUniform2f(const std::string& name, float v0, float v1){
	GLCall(glUniform2f(GetUniformLocation(name), v0, v1));
}

void Shader::SetUniform2i(const std::string& name, int v0, int v1){
	GLCall(glUniform2i(GetUniformLocation(name), v0, v1));
}

void Shader::SetUniform2ui(const std::string& name, unsigned int v0, unsigned int v1){
	GLCall(glUniform2ui(GetUniformLocation(name), v0, v1));
}

void Shader::SetUniform2fv(const std::string& name, int count, const float *value){
	GLCall(glUniform2fv(GetUniformLocation(name), count, value));
}

void Shader::SetUniform2iv(const std::string& name, int count, const int *value){
	GLCall(glUniform2iv(GetUniformLocation(name), count, value));
}

void Shader::SetUniform2uiv(const std::string& name, int count, const unsigned int *value){
	GLCall(glUniform2uiv(GetUniformLocation(name), count, value));
}

void Shader::SetUniform3f(const std::string& name, float v0, float v1, float v2){
	GLCall(glUniform3f(GetUniformLocation(name), v0, v1, v2));
}

void Shader::SetUniform3i(const std::string& name, int v0, int v1, int v2){
	GLCall(glUniform3i(GetUniformLocation(name), v0, v1, v2));
}

void Shader::SetUniform3ui(const std::string& name, unsigned int v0, unsigned int v1, unsigned int v2){
	GLCall(glUniform3ui(GetUniformLocation(name), v0, v1, v2));
}

void Shader::SetUniform3fv(const std::string& name, int count, const float *value){
	GLCall(glUniform3fv(GetUniformLocation(name), count, value));
}

void Shader::SetUniform3iv(const std::string& name, int count, const int *value){
	GLCall(glUniform3iv(GetUniformLocation(name), count, value));
}

void Shader::SetUniform3uiv(const std::string& name, int count, const unsigned int *value){
	GLCall(glUniform3uiv(GetUniformLocation(name), count, value));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3){
	GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniform4i(const std::string& name, int v0, int v1, int v2, int v3){
	GLCall(glUniform4i(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniform4ui(const std::string& name, unsigned int v0, unsigned int v1, unsigned int v2, unsigned int v3){
	GLCall(glUniform4ui(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniform4fv(const std::string& name, int count, const float *value){
	GLCall(glUniform4fv(GetUniformLocation(name), count, value));
}

void Shader::SetUniform4iv(const std::string& name, int count, const int *value){
	GLCall(glUniform4iv(GetUniformLocation(name), count, value));
}

void Shader::SetUniform4uiv(const std::string& name, int count, const unsigned int *value){
	GLCall(glUniform4uiv(GetUniformLocation(name), count, value));
}

void Shader::SetuniformMat4f(const std::string& name, glm::mat4 matrix){
	GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

}} // namespace
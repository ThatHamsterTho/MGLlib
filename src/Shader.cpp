#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

#include <GL/glew.h>

#include "Shader.hpp"
#include "Debugger.hpp"

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

	int location = glGetUniformLocation(m_RendererID, name.c_str());
	if (location == -1){
		std::cout << "Warning: uniform " << name << " does not exist.\n" << endl;
	}
	else{
		m_uniformLocationCache[name] = location;
	}
	return location;
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3){
	GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

// used for multiple shader files
string Shader::ReadShaderFile(enum __ShaderType T){
	// Read the Vertex Shader code from the file
	string ShaderCode;
	ifstream ShaderStream(m_filepath[T]);
	if(ShaderStream.is_open()){
		stringstream sstr;
		sstr << ShaderStream.rdbuf();
		ShaderCode = sstr.str();
		ShaderStream.close();
	}else{
		cout << "Impossible to open " << m_filepath[T] << ". Are you in the right directory ? Don't forget to read the FAQ !" << endl;
		getchar();
		return 0;
	}
	return ShaderCode;
}
// used for single shader file
Shader::ShaderProgramSource Shader::ParseShader(){
	ifstream stream(m_filepath[0]);

	enum ShaderType{
		NONE = -1, VERTEX = 0, FRAGMENT = 1,
		SHADERTYPES
	};

	string line;
	stringstream ss[SHADERTYPES];	// 1 for vertex, 1 for fragment
	ShaderType type = NONE;
	while (getline(stream, line))
	{
		if(line.find("#shader") != string::npos){
			if(line.find("vertex") != string::npos){
				// set mode to vertex
				type = VERTEX;
			}
			else if (line.find("fragment") != string::npos){
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
unsigned int Shader::CompileShader(unsigned int type, const string& ShaderCode){
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
		cout << &ShaderErrorMessage[0] << endl;
	}

	cout << "Compiled " << ShaderType[type - 0x8B30] << " Shader" << endl;
	return ShaderID;
}
// links the shader files
void Shader::LinkShaders(unsigned int VertexShaderID, unsigned int FragmentShaderID){
	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Link the program
	cout << "Linking program" << endl;
	glAttachShader(m_RendererID, VertexShaderID);
	glAttachShader(m_RendererID, FragmentShaderID);
	glLinkProgram(m_RendererID);

	// Check the program
	glGetProgramiv(m_RendererID, GL_LINK_STATUS, &Result);
	glGetProgramiv(m_RendererID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> ProgramErrorMessage(InfoLogLength+1);
		glGetProgramInfoLog(m_RendererID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		cout << &ProgramErrorMessage[0] << endl;
	}

	
	glDetachShader(m_RendererID, VertexShaderID);
	glDetachShader(m_RendererID, FragmentShaderID);
	
	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);
}
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

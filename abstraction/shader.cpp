#include "shader.hpp"
#include <fstream>
#include <sstream>
#include <memory>
#include <glm/glm.hpp>

Shader::Shader(const char* vertexPath,const char* fragmentPath, const char* geometryPath)
{
   // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode, fragmentCode, geometryCode;
    std::ifstream vShaderFile,fShaderFile, gShaderFile;
// ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        // open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        if (!vShaderFile.is_open()) std::cout << "vertex shader failed to open\n";
        if (!fShaderFile.is_open()) std::cout << "fragment shader failed to open\n";
        std::stringstream vShaderStream,fShaderStream;
        // read file’s buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
        if(geometryPath != nullptr)
        {
            gShaderFile.open(geometryPath);
            std::stringstream gShaderStream;
            gShaderStream << gShaderFile.rdbuf();
            gShaderFile.close();
            geometryCode = gShaderStream.str();
        }
    }
    catch(const std::ifstream::failure& e)
    {
        std::cerr << e.what() << '\n';
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ";
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    unsigned int vertexCompile,fragmentCompile;
    // vertex shader compilation
    vertexCompile = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexCompile,1,&vShaderCode,nullptr);
    glCompileShader(vertexCompile);
    // error handling vertex shader
    int success;// for checking if compilation of shader is succesful
    glGetShaderiv(vertexCompile,GL_COMPILE_STATUS,&success);
    if (success == GL_FALSE)// vertex shader didn't compile
    {
        int error;
        glGetShaderiv(vertexCompile,GL_INFO_LOG_LENGTH,&error);
        std::unique_ptr<char> message = std::make_unique<char>(error * sizeof(char));
        glGetShaderInfoLog(vertexCompile,error,&error,message.get());
        std::cout << "vertex shader could not compile.\nError : " << message.release();
        glDeleteShader(vertexCompile);
    }
    // fragment shader compilation
    fragmentCompile = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentCompile,1,&fShaderCode,nullptr);
    glCompileShader(fragmentCompile);
    // error handling fragment shader
    glGetShaderiv(fragmentCompile,GL_COMPILE_STATUS,&success);
    if (success == GL_FALSE)// fragment shader didn't compile
    {
        int error;
        glGetShaderiv(fragmentCompile,GL_INFO_LOG_LENGTH,&error);
        std::unique_ptr<char> message = std::make_unique<char>(error * sizeof(char));
        glGetShaderInfoLog(fragmentCompile,error,&error,message.get());
        std::cout << "fragment shader could not compile.\nError : " << message.release();
        glDeleteShader(fragmentCompile);
    }
    // if geometry shader is given, compile geometry shader
    unsigned int geometryCompile;
    if (geometryPath != nullptr)
    {
        const char * gShaderCode = geometryCode.c_str();
        geometryCompile = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geometryCompile, 1, &gShaderCode, nullptr);
        glCompileShader(geometryCompile);

        // error handling geometry shader
        glGetShaderiv(geometryCompile, GL_COMPILE_STATUS, &success);
        if (success == GL_FALSE)// geometry shader didn't compile
        {
            int error;
            glGetShaderiv(geometryCompile, GL_INFO_LOG_LENGTH, &error);
            std::unique_ptr<char> message = std::make_unique<char>(error * sizeof(char));
            glGetShaderInfoLog(geometryCompile, error, &error, message.get());
            std::cout << "geometry shader could not compile.\nError : " << message.release();
            glDeleteShader(geometryCompile);
        }
    }
    // shader program
    programID = glCreateProgram();
    glAttachShader(programID,vertexCompile);
    glAttachShader(programID,fragmentCompile);
    if(geometryPath != nullptr)// link geometry shader if present
        glAttachShader(programID, geometryCompile);
    glLinkProgram(programID);
    // error handling for linking
    glGetProgramiv(programID,GL_LINK_STATUS,&success);
    if (success == GL_FALSE)
    {
        int error;
        glGetProgramiv(programID,GL_INFO_LOG_LENGTH,&error);
        std::unique_ptr<char> message = std::make_unique<char>(error * sizeof(char));
        glGetProgramInfoLog(programID,error,&error,message.get());
        std::cout << "Could not link program\nError : " << message.release();
        glDeleteProgram(programID);
    }        // delete the shaders as they're linked into our program now and no longer necessery
    glDeleteShader(vertexCompile);
    glDeleteShader(fragmentCompile);
    if(geometryPath != nullptr)
        glDeleteShader(geometryCompile);
}

void Shader::setBool(const std::string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(programID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, int value) const
{
    glUniform1i(glGetUniformLocation(programID, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(programID,name.c_str()), value);
}

void Shader::setVec2(const std::string &name, const glm::vec2 &value) const
{
    glUniform2fv(glGetUniformLocation(programID, name.c_str()), 1, &value[0]);
}

void Shader::setVec2(const std::string &name, float x, float y) const
{
    glUniform2f(glGetUniformLocation(programID, name.c_str()), x, y);
}

void Shader::setVec3(const std::string &name, const glm::vec3 &value) const
{
    glUniform3fv(glGetUniformLocation(programID, name.c_str()), 1, &value[0]);
}

void Shader::setVec3(const std::string &name, float x, float y, float z) const
{
    glUniform3f(glGetUniformLocation(programID, name.c_str()), x, y, z);
}

void Shader::setVec4(const std::string &name, const glm::vec4 &value) const
{
    glUniform4fv(glGetUniformLocation(programID, name.c_str()), 1, &value[0]);
}

void Shader::setVec4(const std::string &name, float x, float y, float z, float w) const
{
    glUniform4f(glGetUniformLocation(programID, name.c_str()), x, y, z, w);
}

void Shader::setMat2(const std::string &name, const glm::mat2 &value) const
{
    glUniformMatrix2fv(glGetUniformLocation(programID, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

void Shader::setMat3(const std::string &name, const glm::mat3 &value) const
{
    glUniformMatrix3fv(glGetUniformLocation(programID, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(programID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
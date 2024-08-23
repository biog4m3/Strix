#ifndef STRIX_SHADER_H
#define STRIX_SHADER_H

#include <strix/strix.h>
#include <strix/strix_gl.h>
#include <strix/strix_logger.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <fstream>
#include <sstream>

namespace strix
{
    class Shader
    {
    private:
        uint32 m_id;

    public:
        Shader(const char *vertexPath, const char *fragmentPath)
        {
            std::string vertexCode;
            std::string fragmentCode;
            std::ifstream vShaderFile;
            std::ifstream fShaderFile;
            vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
            fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
            try
            {
                vShaderFile.open(vertexPath);
                fShaderFile.open(fragmentPath);
                std::stringstream vShaderStream, fShaderStream;
                vShaderStream << vShaderFile.rdbuf();
                fShaderStream << fShaderFile.rdbuf();
                vShaderFile.close();
                fShaderFile.close();
                vertexCode = vShaderStream.str();
                fragmentCode = fShaderStream.str();
            }
            catch (std::ifstream::failure e)
            {
                Logger::Error("Failed to read shader file");
            }
            const char *vShaderCode = vertexCode.c_str();
            const char *fShaderCode = fragmentCode.c_str();
            uint32 vertex, fragment;
            int success;
            char infoLog[512];
            vertex = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vertex, 1, &vShaderCode, NULL);
            glCompileShader(vertex);
            glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(vertex, 512, NULL, infoLog);
                Logger::Error("Vertex shader compilation failed: %s", infoLog);
            }
            fragment = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragment, 1, &fShaderCode, NULL);
            glCompileShader(fragment);
            glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(fragment, 512, NULL, infoLog);
                Logger::Error("Fragment shader compilation failed: %s", infoLog);
            }
            m_id = glCreateProgram();
            glAttachShader(m_id, vertex);
            glAttachShader(m_id, fragment);
            glLinkProgram(m_id);
            glGetProgramiv(m_id, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(m_id, 512, NULL, infoLog);
                Logger::Error("Shader program linking failed: %s", infoLog);
            }
            glDeleteShader(vertex);
            glDeleteShader(fragment);
        }
    
        void use()
        {
            glUseProgram(m_id);
        }

        void setBool(const std::string &name, bool value) const
        {
            glUniform1i(glGetUniformLocation(m_id, name.c_str()), (int)value);
        }

        void setInt(const std::string &name, int value) const
        {
            glUniform1i(glGetUniformLocation(m_id, name.c_str()), value);
        }

        void setFloat(const std::string &name, float value) const
        {
            glUniform1f(glGetUniformLocation(m_id, name.c_str()), value);
        }

        void setVec2(const std::string &name, const glm::vec2 &value) const
        {
            glUniform2fv(glGetUniformLocation(m_id, name.c_str()), 1, glm::value_ptr(value));
        }

        void setVec3(const std::string &name, const glm::vec3 &value) const
        {
            glUniform3fv(glGetUniformLocation(m_id, name.c_str()), 1, glm::value_ptr(value));
        }

        void setVec4(const std::string &name, const glm::vec4 &value) const
        {
            glUniform4fv(glGetUniformLocation(m_id, name.c_str()), 1, glm::value_ptr(value));
        }

        void setMat2(const std::string &name, const glm::mat2 &mat) const
        {
            glUniformMatrix2fv(glGetUniformLocation(m_id, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
        }

        void setMat3(const std::string &name, const glm::mat3 &mat) const
        {
            glUniformMatrix3fv(glGetUniformLocation(m_id, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
        }

        void setMat4(const std::string &name, const glm::mat4 &mat) const
        {
            glUniformMatrix4fv(glGetUniformLocation(m_id, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
        }
        
    };
}

#endif // STRIX_SHADER_H
#pragma once

#include <string>
#include <unordered_map>
#include <glad/glad.h>
#include <glm/glm.hpp>

namespace openGL {

    class Shader {

    private:
        //---------- Attributes ----------//
        
    	GLuint _programId;
    	GLuint _vertexId;
    	GLuint _fragmentId;
        GLuint _geometryId;
        std::unordered_map<std::string, GLint> _uniformLocationCache;

        bool _compiled;

        //---------- Methodes ----------//
        GLuint compileShader(const GLenum &shaderType, const std::string &shaderStr);
        std::string parseFile(const char* filepath);
        std::string shaderTypeStr(const GLenum &shaderType);
        void Shader::AttachShaderId(const char* shaderName, const GLuint id);

    public:

    	Shader() = default;
		Shader(const Shader& s);
        Shader(const char* vertexShader, const char* fragmentShader, const char* geometryShader = nullptr, const bool &fromFile = true);
       ~Shader();

        bool bind() const;
        inline void unBind() const { glUseProgram(-1); }

        //---------- Uniforms ----------//
        GLint getUniform(const std::string &uniformName);
        void setInt(const std::string &uniformName, int v);
        void setFloat(const std::string &uniformName, float v);
        void setVec2f(const std::string &uniformName, const glm::vec2 &v);
        void setVec2f(const std::string &uniformName, const float &x, const float &y);
        void setVec3f(const std::string &uniformName, const glm::vec3 &v);
        void setVec3f(const std::string &uniformName, const float &x, const float &y, const float &z);
        void setVec4f(const std::string &uniformName, const glm::vec4 &v);
        void setVec4f(const std::string &uniformName, const float &a, const float &b, const float &c, const float &d);
        void setMat3(const std::string &uniformName, const glm::mat3 &mat);
        void setMat4(const std::string &uniformName, const glm::mat4 &mat);

        //---------- getters/setters ----------//
        
        inline GLuint getID() const { return _programId; };

    };

}
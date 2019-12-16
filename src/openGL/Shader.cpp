#include "Shader.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include "glException.hpp"

using namespace openGL;

Shader::Shader(const char* vertexShader, const char* fragmentShader, const char* geometryShader = nullptr, const bool &fromFile = true) 
    : _programId(0), _vertexId(0), _fragmentId(0), _geometryId(0), _compiled(false) {

    std::string vsStr;
    std::string fsStr;
    std::string gsStr;

    if (fromFile) {
        vsStr = parseFile(vertexShader);
        fsStr = parseFile(fragmentShader);
        if(geometryShader != nullptr)
            gsStr = parseFile(geometryShader);
    }else {
        vsStr = vertexShader;
        fsStr = fragmentShader;
        if(geometryShader != nullptr) {
            gsStr = geometryShader;
        }else {
            gsStr = "";
        }
            
    }

	GLCall(_programId = glCreateProgram()); // attribute program id 

    // compile shaders
    GLCall(_vertexId = compileShader(GL_VERTEX_SHADER, vsStr));
	GLCall(_fragmentId = compileShader(GL_FRAGMENT_SHADER, fsStr));
    if (geometryShader != nullptr)
		GLCall(_geometryId = compileShader(GL_GEOMETRY_SHADER, gsStr));

    AttachShaderId("vertex", _vertexId);
    AttachShaderId("fragment", _fragmentId);
    if (geometryShader != nullptr)
        AttachShaderId("geometry", _geometryId);
 
    //link them 
	GLCall(glLinkProgram(_programId));

	int linkResult;
	GLCall(glGetProgramiv(_programId, GL_LINK_STATUS, &linkResult));
	if (linkResult == GL_FALSE) {
		GLCall(glDeleteProgram(_programId));
		std::cerr << "Shader creation aborded (linking doesn't work)" << std::endl;
		// _compiled = false; // already false
    } else {
		GLCall(glValidateProgram(_programId));
		_compiled = true;
    }


    // delete the shaders as they're linked into our program now and no longer necessery
    glDeleteShader(_vertexId);
    glDeleteShader(_fragmentId);
    if (geometryShader != nullptr)
        glDeleteShader(_geometryId);
    // we no longer need them anymore, they will be destrol automaticaly 
    //If a shader object to be deleted is attached to a program object, it will be flagged for deletion, 
    //but it will not be deleted until it is no longer attached to any program object
}

Shader::Shader(const Shader& s) : _programId(s._programId), _vertexId(s._vertexId), _fragmentId(s._fragmentId), _geometryId(s._geometryId), _uniformLocationCache(s._uniformLocationCache) {
// copy constructor //TODO
}

Shader::~Shader() {
    if (_programId != 0)
        GLCall(glDeleteProgram(_programId));
}

//---------- private functions ----------//


void Shader::AttachShaderId(const char* shaderName, const GLuint id) {
    if (id != 0) {
        GLCall(glAttachShader(_programId, _vertexId));
        std::cout << shaderName << " shader attached " << std::endl;
    }
    else {
        std::cout << "[Shader] creation aborded(" << shaderName << "shaders not compiled)" << std::endl;
        assert(false);
    }
}

GLuint Shader::compileShader(const GLenum &shaderType, const std::string &shaderStr) {

    GLuint shaderId = glCreateShader(shaderType);

    if(shaderId) {

        const char* shaderCode = shaderStr.c_str();
        // const GLint shaderCodeSize = shaderStr.size();
        GLCall(glShaderSource(shaderId, 1, &shaderCode, nullptr));
        GLCall(glCompileShader(shaderId));

        //Debug 
        int compileResult;
        GLCall(glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compileResult));
        if (compileResult == GL_FALSE) {

            int infosLength;
            GLCall(glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &infosLength));
            std::vector<char> message(infosLength);
            GLCall(glGetShaderInfoLog(shaderId, infosLength, &infosLength, &message[0]));
            std::cout << "[Shader] error during compiling shader (" << shaderTypeStr(shaderType) << ") :" << std::endl << &message[0] << std::endl;
			debug_break();
            return 0;
        }
    } else {
        std::cerr << "[Shader] Failed to assign new shader id (" << shaderTypeStr(shaderType) <<  " shader)" << std::endl;
		debug_break();
    }
    return shaderId;
}

std::string Shader::parseFile(const char* filepath) {

    std::string shaderStr;
    std::ifstream file(filepath, std::ios::in);

    if (!file.is_open()){
		std::cerr << "[Shader] Failed to open file : " + *filepath << std::endl;
		debug_break();
    }

    // with fileStream
    // file.seekg(0, std::ios::end);
    // shaderStr.resize((unsigned int)file.tellg());
    // file.seekg(0, std::ios::beg);
    // file.read(&shaderStr[0], shaderStr.size());
    // file.close();

    // with string stream
    std::stringstream stream;
    stream << file.rdbuf();
    file.close();	
    shaderStr = stream.str();

    // std::cout << shaderStr << std::endl;
    return shaderStr;
}

bool Shader::bind() const{
	if (_compiled) {
		GLCall(glUseProgram(_programId));
	}
    return _compiled;
};

std::string Shader::shaderTypeStr(const GLenum &shaderType) {
	switch (shaderType) {
	case GL_FRAGMENT_SHADER:
		return "GL_FRAGMENT_SHADER";

	case GL_GEOMETRY_SHADER:
		return "GL_GEOMETRY_SHADER";

	case GL_COMPUTE_SHADER:
		return "GL_COMPUTE_SHADER";

	case GL_VERTEX_SHADER:
		return "GL_VERTEX_SHADER";

	case GL_TESS_CONTROL_SHADER:
		return "GL_TESS_CONTROL_SHADER";

	case GL_TESS_EVALUATION_SHADER:
		return "GL_TESS_EVALUATION_SHADER";

	default:
		assert(!"unknown shader type");
		return nullptr;
	}
}

//---------- Uniform ----------//
GLint Shader::getUniform(const std::string &uniformName) {
    if (_uniformLocationCache.find(uniformName) != _uniformLocationCache.end()) {
        return _uniformLocationCache[uniformName];
    }

	GLCall(GLint location = glGetUniformLocation(_programId, uniformName.c_str()));

    if (location ==  -1) {
        std::cerr << "[Shader] uniform \"" << uniformName << "\" doesn't exist !" << std::endl;
		debug_break();
    }

	_uniformLocationCache[uniformName] = location;
    return location;
}

void Shader::setInt(const std::string &uniformName, int v) {
    GLCall(glUniform1i(getUniform(uniformName), v));
}
void Shader::setFloat(const std::string &uniformName, float v) {
    GLCall(glUniform1f(getUniform(uniformName), v));
}
void Shader::setVec2f(const std::string &uniformName, const glm::vec2 &v) {
    GLCall(glUniform2f(getUniform(uniformName), v.x, v.y));
}
void Shader::setVec2f(const std::string &uniformName, const float &x, const float &y) {
    GLCall(glUniform2f(getUniform(uniformName), x, y));
}
void Shader::setVec3f(const std::string &uniformName, const glm::vec3 &v) {
    GLCall(glUniform3f(getUniform(uniformName), v.x, v.y, v.z));
}
void Shader::setVec3f(const std::string &uniformName, const float &x, const float &y, const float &z) {
    GLCall(glUniform3f(getUniform(uniformName), x, y, z));
}
void Shader::setVec4f(const std::string &uniformName, const glm::vec4 &v) {
    GLCall(glUniform4f(getUniform(uniformName), v.x, v.y, v.z, v.w));
}
void Shader::setVec4f(const std::string &uniformName, const float &x, const float &y, const float &z, const float &w) {
    GLCall(glUniform4f(getUniform(uniformName), x, y, z, w));
}
void Shader::setMat3(const std::string &uniformName, const glm::mat3 &m) {
    GLCall(glUniformMatrix3fv(getUniform(uniformName), 1, GL_FALSE, &m[0][0]));
}
void Shader::setMat4(const std::string &uniformName, const glm::mat4 &m) {
    GLCall(glUniformMatrix4fv(getUniform(uniformName), 1, GL_FALSE, &m[0][0]));
}

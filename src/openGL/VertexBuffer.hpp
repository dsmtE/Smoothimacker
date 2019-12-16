#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include "glException.hpp"

namespace openGL {

    class VertexBuffer {

    private:
        //---------- Attributes ----------//
        GLuint _index;

    public:
        VertexBuffer();
        ~VertexBuffer();

        //---------- Methodes ----------//
        
        void bind();
        void unbind();

        //void setData(const float* data, const GLuint &verticesCount, const GLenum &drawType);

        ///
        ///\brief set data to VertexBuffer (GL_ARRAY_BUFFER)
        ///\param indices data as a template vector 
        ///\param drawType GLenum specifies the expected usage of the data (GL_STATIC_DRAW, GL_STREAM_DRAW, GL_DYNAMIC_DRAW, ... )
        ///
        template<typename T>
        void setData(const std::vector<T>& data, const GLenum& drawType) {
            bind();
			GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(T) * data.size(), data.data(), drawType));
			unbind();
        }
        
		static void setVertexAttrib(const GLuint &VertexBufferID, const GLuint &size, GLenum dataType, const GLuint &stride, const GLuint &offset);
		static void setVertexAttribInteger(GLuint &VertexBufferID,const GLuint &size, GLenum dataType,const GLuint &stride,const GLuint &offset);
		static void setAttribDivisor(const GLuint &index, const GLubyte &divisor);

        //---------- getters/setters ----------//
        
		inline GLuint getId() const { return _index;}

    };
 
}
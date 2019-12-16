#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include "glException.hpp"

namespace openGL {

    /// \class ElementBuffer
    /// \namespace openGL
    /// \brief Class that abstracted the elementBufferArray
    class ElementBuffer {

    private:
        //---------- Attributes ----------//
        GLuint _index;

    public:

        ///
        ///\brief Init ElementBuffer by gen & bind id
        ///
        ElementBuffer();

        ///
        ///\brief Delete this ElementBuffer using glDeleteBuffers on its id
        ///
        ~ElementBuffer();

        //---------- Methodes ----------//
        
        ///
        ///\brief Bind this ElementBuffer using its id (GL_ELEMENT_ARRAY_BUFFER)
        ///
        void bind();

        ///
        ///\brief Unbind this ElementBuffer (GL_ELEMENT_ARRAY_BUFFER)
        ///
        void unbind();

        ///
        ///\brief set data to  ElementBuffer (GL_ELEMENT_ARRAY_BUFFER)
        ///\param indices data as std::vector of uvec3 send to this ElementBuffer using glBufferData
        ///\param drawType GLenum specifies the expected usage of the data (GL_STATIC_DRAW, GL_STREAM_DRAW, GL_DYNAMIC_DRAW, ... )
        ///
        void setData(const std::vector<glm::uvec3>& indices, const GLenum &drawType);

        //---------- getters/setters ----------//
        
        ///
        ///\brief set data to  ElementBuffer (GL_ELEMENT_ARRAY_BUFFER)
        ///\param indices data as std::vector of uvec3 send to this ElementBuffer using glBufferData
        ///\return id of this ElementBuffer
        ///
        inline GLuint getId() const { return _index; }

    };
}
#pragma once

#include <glm/glm.hpp>
#include <glad/glad.h>
#include "glException.hpp"

namespace openGL {

    class VertexArray {

    private:
        //---------- Attributes ----------//
        GLuint _index;

    public:
		VertexArray();
        ~VertexArray();

        //---------- Methodes ----------//
        
        void bind();
        void unbind();

        //---------- getters/setters ----------//
        
        inline GLuint getId() const { return _index; }

    };
}
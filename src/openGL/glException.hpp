// thanks to Guillaume Haerinck for this debug class for openGL error 

#pragma once

#include <glad/glad.h>
#include <assert.h>

#ifdef _MSC_VER
    #define debug_break __debugbreak
#else
    #define debug_break __builtin_trap
#endif

/**
 * @brief Assertion and logger handling for opengl functions
 */
#ifndef NDEBUG
    #define BreakAssert(x) if (!x) { debug_break(); assert(false); }
    #define GLCall(x) glexp::clear(); x; BreakAssert(glexp::doesFunctionWorks(#x, __FILE__, __LINE__))
#else
    #define GLCall(x) x
#endif

namespace glexp {
    /**
     * @brief Empty the OpenGl error buffer
     */
    void clear();

    /**
     * @brief Print OpenGl errors to the console if any
     * 
     * @param functionName
     * @param filename
     * @param line
     */
    bool doesFunctionWorks(const char* functionName, const char* filename, int line);

    /**
     * @brief Transform error enum to text for OpenGL
     * 
     * @param err 
     * @return char const* - Text corresponding to OpenGl error
     */
    char const* glErrorString(GLenum const err);
}
#pragma once

#include <SDL.h>

#include <glm/ext/vector_int2.hpp>

namespace openGL {

    class SDLWinWrapper {

    private:

        SDL_Window* _window;
        SDL_GLContext _glContext;
        bool _running;
        
        void initSDL();
        void initImgui();

    public:

        SDLWinWrapper(int width, int  height, const char* title);
        ~SDLWinWrapper();

		int handleSDLEvents(SDL_Event& e);
		void swapBuffers();
		void updateImgui();
		void renderImgui();

        inline SDL_Window* getWindow() const { return _window; }
        inline SDL_GLContext getContext() const { return _glContext; }
		glm::ivec2 getMouseCoord() const;
        glm::ivec2 windowSize() const;

        void onWindowResize();

        inline void exit() { _running = false; };
        inline bool isRunning() const { return _running; };

    };

} // namespace end


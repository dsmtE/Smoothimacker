#pragma once

#include <SDL.h>
#include <functional>

namespace gui {
	
	class Tool {
	
	private:
		std::function<void()> _action;

	public:
		Tool();
		~Tool() = default;

		void handleSDLEvents(SDL_Event sdlEvent);
		void setAction(std::function<void()> action);
	};
}

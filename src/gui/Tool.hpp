#pragma once

#include "toolStrategies/IToolStrategy.hpp"
#include <SDL.h>

namespace gui {
	
	class Tool {
	
	private:
		IToolStrategy* _strategy;
	
	public:
		Tool();
		~Tool();

		void handleSDLEvents(SDL_Event sdlEvent);
		void setStrategy(StrategyType type);
	};
}

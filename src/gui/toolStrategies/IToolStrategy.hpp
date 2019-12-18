#pragma once

#include <iostream>

namespace gui {

	enum StrategyType
	{
		Create, Delete, Extrude, Dig, Paint
	};

	class IToolStrategy {
	
	public:

		virtual ~IToolStrategy();

	private:

		virtual void action() = 0;

		friend class Tool;
	};
}
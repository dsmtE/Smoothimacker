#pragma once

#include "IToolStrategy.hpp"

namespace gui {

	class PaintStrategy : public IToolStrategy {

	public:
		PaintStrategy() = default;
		~PaintStrategy();

	private:
		void action() override;

	};
}

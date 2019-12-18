#pragma once

#include "IToolStrategy.hpp"

namespace gui {

	class DigStrategy : public IToolStrategy {

	public:
		DigStrategy() = default;
		~DigStrategy();

	private:

		void action() override;

	};
}

#pragma once

#include "IToolStrategy.hpp"

namespace gui {

	class ExtrudeStrategy : public IToolStrategy {

	public:
		ExtrudeStrategy() = default;
		~ExtrudeStrategy();

	private:

		void action() override;

	};
}

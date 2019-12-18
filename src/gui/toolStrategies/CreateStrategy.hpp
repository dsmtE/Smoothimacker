#pragma once

#include "IToolStrategy.hpp"

namespace gui {

	class CreateStrategy : public IToolStrategy {

	public:
		CreateStrategy() = default;
		~CreateStrategy();

	private:
		void action() override;

	};
}

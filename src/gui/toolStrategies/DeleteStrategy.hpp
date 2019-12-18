#pragma once

#include "IToolStrategy.hpp"

namespace gui {

	class DeleteStrategy : public IToolStrategy {

	public:
		DeleteStrategy() = default;
		~DeleteStrategy();

	private:
		void action() override;
	};
}

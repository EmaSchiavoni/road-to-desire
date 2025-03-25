#pragma once
#include "ListMenu.h"

namespace rtd {

    class MapSelectMenu : public rtd::ListMenu
	{
		std::vector<std::string> getMapList() const;

	public:
		MapSelectMenu();
		void update(const float& delta_time, rtd::Game& game) override;
		void select(rtd::Game& game) override;
	};

}
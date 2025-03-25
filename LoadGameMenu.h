#pragma once
#include "ListMenu.h"

namespace rtd {

	class LoadGameMenu : public rtd::ListMenu
	{
		std::string m_mapName;
		std::vector<std::string> getMapList() const;

	public:
		LoadGameMenu();
		void update(const float& delta_time, rtd::Game& game) override;
		void select(rtd::Game& game) override;
	};

}
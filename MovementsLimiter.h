#pragma once
#include "Player.h"
#include "LevelStructure.h"
#include <SFML/Graphics/RectangleShape.hpp>

namespace rtd {

	class MovementsLimiter
	{
		const rtd::Player& m_player;
		rtd::LevelStructure*& m_levelStructure;
		
		bool tileIntersectsProjection(int index_x, int index_y, const sf::FloatRect& projection) const;
		std::pair<float, sf::Vector2i> getDistanceToNearestSolidOnTop(const float& screen_offset) const;
		std::pair<float, sf::Vector2i> getDistanceToNearestSolidOnRight(const float& screen_offset) const;
		std::pair<float, sf::Vector2i> getDistanceToNearestSolidOnLeft(const float& screen_offset) const;
		std::pair<float, sf::Vector2i> getDistanceToNearestSolidOnBottom(const float& screen_offset) const;
	
	public:
		MovementsLimiter(const rtd::Player& player, rtd::LevelStructure*& levelStructure);
		std::map<rtd::Ubication, std::pair<float, sf::Vector2i>> getPlayerMovementLimits(const float& screen_offset) const;
	};

}
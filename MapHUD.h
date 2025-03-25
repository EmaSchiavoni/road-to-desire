#pragma once
#include <SFML/Graphics/CircleShape.hpp>
#include "HUD.h"
#include "HUDElement.h"
#include "Profile.h"

namespace rtd {

	class MapHUD : public rtd::HUD
	{
		rtd::HUDElement m_dragonBallsElement, m_pointsElement;
		sf::Texture m_dragonBallTextures[7];
		sf::Sprite m_dragonBallSprites[7];
		sf::CircleShape m_dragonBallCircles[7];
		sf::Text m_pointsNumberText, m_pointsLettersText;
		const sf::Vector2f m_ballsStartPos = sf::Vector2f(9, 7);
		const float m_space_between_balls = 3;
		const int m_ball_top = 5;
		const int m_points_top = 16;

		void prepareDragonBallsHUD();
		void preparePointsHUD();
		void drawDragonBalls(sf::RenderWindow& window);
		void drawPoints(sf::RenderWindow& window);
		void rescaleIcon(sf::Sprite& sprite);

	public:
		MapHUD();
		void setPoints();
		void setDragonBalls();
		void draw(sf::RenderWindow& window) override;
	};

}
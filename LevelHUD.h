#pragma once
#include <vector>
#include "HUD.h"
#include "HUDElement.h"
#include "Constants.h"
#include "Collectable.h"

namespace rtd {

	struct CollectableInHUD {
		int count = 0;
		sf::Sprite sprite;
		sf::Texture texture;
	};

	class LevelHUD : public rtd::HUD
	{
		rtd::HUDElement m_heartsElement, m_coinsElement;
		sf::Texture m_fullHeartTexture, m_emptyHeartTexture;
		sf::Sprite m_fullHeart, m_emptyHeart;
		sf::Text m_coinsText, m_timerText;
		int m_max_lives, m_lives;
		std::map< rtd::Collectable::Type, rtd::CollectableInHUD > m_collectables;
		const sf::Vector2f m_heartsStartPos = sf::Vector2f(8, 7);
		const int m_space_between_hearts = 3;
		const int m_space_between_coin = 5;
		const int m_space_between_collecteds = 2;
		const float m_coin_top = 7;
		const float m_collecteds_top = 33;

		void prepareHeartsHUD();
		void prepareCoinsHUD();
		void prepareTimer();
		void drawHearts(sf::RenderWindow& window);
		void drawCollectables(sf::RenderWindow& window);
		void drawTimer(sf::RenderWindow& window);
		void rescaleIcon(sf::Sprite& sprite);

	public:
		LevelHUD(int max_lives);
		void setLives(int current_lives);
		void setCoins();
		void setTimer(float time_as_seconds);
		void setCollectable(rtd::Collectable::Type type);
		void draw(sf::RenderWindow& window) override;
	};

}
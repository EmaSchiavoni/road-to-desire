#pragma once
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

namespace rtd{

	class HUDElement
	{
		sf::Texture m_backgroundTexture, m_borderTexture;
		sf::Sprite m_background, m_border;
		bool m_is_inverted = false;

	public:
		HUDElement();
		void invert(bool value);
		void setWidth(float width);
		void setHeight(float height);
		void setPosition(float x, float y);
		sf::FloatRect getGlobalBounds() const;
		void draw(sf::RenderWindow& window);
	};

}
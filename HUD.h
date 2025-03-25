#pragma once
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "HUDElement.h"
#include "ResourceHandler.h"

namespace rtd {

	class HUD
	{
	private:
		std::vector<sf::Sprite*> m_sprites;
		std::vector<sf::Text*> m_texts;
		std::vector<rtd::HUDElement*> m_elements;

	protected:
		sf::Font* m_font = nullptr;
		void setText(sf::Text& text);
		void setSprite(sf::Sprite& sprite);
		void setHUDElement(rtd::HUDElement& element);
		void removeSprite(sf::Sprite& sprite);

	public:
		HUD();
		~HUD();
		virtual void draw(sf::RenderWindow& window);
	};

}
#include "HUD.h"

using namespace rtd;
using namespace sf;

extern ResourceHandler resourceHandler;

HUD::HUD()
{
	m_font = resourceHandler.getHudFont();
}

void HUD::setText(Text& text)
{
	m_texts.push_back(&text);
}

void HUD::setSprite(Sprite& sprite)
{
	m_sprites.push_back(&sprite);
}

void HUD::setHUDElement(HUDElement& element)
{
	m_elements.push_back(&element);
}

void HUD::removeSprite(Sprite& spriteToRemove)
{
	for (auto spriteIt = m_sprites.begin(); spriteIt != m_sprites.end(); ++spriteIt)
		if (*spriteIt == &spriteToRemove) {
			*spriteIt = m_sprites.back();
			m_sprites.pop_back();
			break;
		}
}

void HUD::draw(RenderWindow& window)
{
	for (HUDElement* element : m_elements)
		element->draw(window);

	for (Sprite* sprite : m_sprites)
		window.draw(*sprite);

	for (Text* text : m_texts)
		window.draw(*text);
}

HUD::~HUD()
{
	resourceHandler.unloadHudFont();
}
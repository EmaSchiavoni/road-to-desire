#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/Sound.hpp>
#include "Animation.h"

namespace rtd {

	class GameOverAnimation
	{
		sf::SoundBuffer m_ballSoundBuffer;
		sf::Sound m_ballSound;
		rtd::Animation m_rectsAnimation, m_textAnimation;
		sf::Text m_text, m_shadow;
		sf::Color m_pureMagenta = { 255, 0, 42 };
		sf::Color m_pureYellow = { 255, 200, 0 };
		sf::RectangleShape m_topBgRect, m_botBgRect;
		float m_elapsed_time = 0;

		void prepareSound();
		void prepareRects();
		void prepareText();
		void prepareAnimations();
		void updateRectsAnimation();
		void updateTextAnimation();

	public:
		GameOverAnimation();
		bool animationsAreCompleted() const;
		void update(const float& delta_time);
		void draw(sf::RenderWindow& window);
	};

}
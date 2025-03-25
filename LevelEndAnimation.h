#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/Sound.hpp>
#include "Animation.h"

namespace rtd{

	class LevelEndAnimation
	{
		sf::SoundBuffer m_spraySoundBuffer, m_scoreSoundBuffer;
		sf::Sound m_spraySound, m_scoreSound;
		rtd::Animation m_brushAnimation, m_scoreAnimation, m_fadeoutAnimation;
		float m_player_score = 0;
		sf::Text m_scoreText, m_scoreCountText;
		sf::Texture m_brushTexture;
		sf::Sprite m_brushSprite;
		sf::RectangleShape m_blackScreen;
		float m_brush_width = 488;
		float m_elapsed_time = 0;

		void prepareSound();
		void prepareBrush();
		void prepareTexts();
		void prepareAnimations();
		void updateBrushAnimation();
		void updateScoreAnimation();
		void updateFadeoutAnimation();

	public:
		LevelEndAnimation(int lives_left, float time_left, int coins_count, int balls_count);
		float getScore() const;
		bool animationsAreCompleted() const;
		void update(const float& delta_time);
		void draw(sf::RenderWindow& window);
	};

}
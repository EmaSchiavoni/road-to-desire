#pragma once
#include "Collectable.h"

namespace rtd{

	class DragonBall : public rtd::Collectable
	{
		sf::Texture m_texture;
		int m_stars = 0;
		float m_oscilation_direction = 1; //Usado para la animacion oscilante
		float m_initial_y_pos; //Usado para la animacion oscilante
		float m_scale_to_add; //Usado para la animacion de collect
		float m_collected_scale = 1.3f; //Escala final de la animacion
		float m_collect_animation_duration = 3; 
		float m_collect_animation_elapsed_time = 0;
		sf::Vector2f m_distance_to_center; //Usado para la animacion de collect
		bool m_collected_animation_finished = false;

		void fadeOut();
		void updateCollectedAnimation(const float& delta_time);
		void updateOscilation(const float& delta_time);

	public:
		DragonBall(int stars, sf::Vector2u positionAsTile);
		~DragonBall();
		void update(const float& delta_time) override;
		void setTexture() override;
		bool canBeDeleted() override;
	};

}
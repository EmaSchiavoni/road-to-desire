#include <math.h>
#include "DragonBall.h"
#include "Constants.h"

using namespace std;
using namespace rtd;
using namespace sf;

DragonBall::DragonBall(int stars, Vector2u positionAsTile) : Collectable(positionAsTile)
{
	if (stars < 0 || stars > 7)
		throw runtime_error("Ocurrió un error inesperado al intentar crear una esfera del dragon.");

	m_initial_y_pos = this->getPositionAsTile().y * TILE_SIZE + TILE_SIZE/2;
	m_stars = stars;
	this->setType( (Collectable::Type)stars );
	setTexture();
}

DragonBall::~DragonBall()
{}

void DragonBall::setTexture()
{
	m_texture.loadFromFile("Textures/Animateds/dragon_ball"+ to_string(m_stars) + ".png");
	makeItAnimated(m_texture, 3.08f);
	m_texture.setSmooth(true);
	centerOrigin();
	float initial_scale = getScale().x;
	m_scale_to_add = m_collected_scale - initial_scale;
}

void DragonBall::update(const float& delta_time)
{
	Entity::update(delta_time);

	if (wasCollected())
		updateCollectedAnimation(delta_time);
	else
		updateOscilation(delta_time);
}

void DragonBall::updateCollectedAnimation(const float& delta_time)
{
	m_collect_animation_elapsed_time += delta_time;

	Vector2f A = getPosition(); //Punto A
	Vector2f B = {WIN_WIDTH/2, WIN_HEIGHT/2}; //Punto B

	if (A == B) {
		if (m_collect_animation_elapsed_time >= m_collect_animation_duration) {
			m_collected_animation_finished = true;
			return;
		}

		fadeOut();

		return;
	}

	if (m_distance_to_center.x == 0 && m_distance_to_center.y == 0) {
		Vector2f AB = { B.x - A.x, B.y - A.y };
		m_distance_to_center.x = B.x - A.x;
		m_distance_to_center.y = B.y - A.y;
	}

	float x_distance = abs(B.x - A.x);
	int x_direction = (B.x > A.x) ? 1 : -1;
	float x = A.x + m_distance_to_center.x / (m_collect_animation_duration/3 / delta_time);


	float y_distance = abs(B.y - A.y);
	int y_direction = (B.y > A.y) ? 1 : -1;
	float y = A.y + m_distance_to_center.y / (m_collect_animation_duration/3 / delta_time);

	if (m_distance_to_center.x > 0)
		x = (x > B.x)? B.x : x;
	else
		x = (x < B.x) ? B.x : x;

	if (m_distance_to_center.y > 0)
		y = (y > B.y) ? B.y : y;
	else
		y = (y < B.y) ? B.y : y;

	setPosition({x, y});
	
	if (getScale().x < m_collected_scale)
	{
		Vector2f AB = { B.x - A.x, B.y - A.y };
		float distance_to_center = (float)pow(AB.x * AB.x + AB.y * AB.y, .5);

		float new_scale = getScale().x + m_scale_to_add / (m_collect_animation_duration/3 / delta_time);
		setScale(new_scale, new_scale);
	}
}

void DragonBall::fadeOut()
{
	if (m_collect_animation_elapsed_time > m_collect_animation_duration * 2 / 3 &&
		m_collect_animation_elapsed_time < m_collect_animation_duration)
	{
		float fading_duration = m_collect_animation_duration / 3;
		float fading_elapsed_time = m_collect_animation_elapsed_time - (m_collect_animation_duration * 2 / 3);
		Uint8 alpha = 255 - Uint8((int)floor(fading_elapsed_time * (255 / fading_duration)));
		this->setColor({ 255,255,255, alpha });
	}
}

void DragonBall::updateOscilation(const float& delta_time)
{
	auto currentPos = this->getGlobalPosition();
	float new_y = currentPos.y + m_oscilation_direction * delta_time * 4;

	if (new_y >= m_initial_y_pos + 3) {
		new_y = m_initial_y_pos + 3;
		m_oscilation_direction = -1;
	}
	else if (new_y <= m_initial_y_pos - 3) {
		new_y = m_initial_y_pos - 3;
		m_oscilation_direction = 1;
	}

	setGlobalPosition(currentPos.x, new_y);
}

bool DragonBall::canBeDeleted()
{
	return wasCollected() && m_collected_animation_finished;
}
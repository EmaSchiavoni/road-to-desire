#include "Hitbox.h"

using rtd::Hitbox;

Hitbox::Hitbox()
{}

Hitbox::Hitbox(float left, float top, float widthFactor, float heightFactor)
{
	this->left = left;
	this->top = top;
	this->widthFactor = widthFactor;
	this->heightFactor = heightFactor;
}
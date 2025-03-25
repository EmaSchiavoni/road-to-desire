#pragma once

namespace rtd {

	struct Hitbox {
		float left = 0;
		float top = 0;
		float widthFactor = 1;
		float heightFactor = 1;
		
		Hitbox();
		Hitbox(float left, float top, float widthFactor, float heightFactor);
	};

}
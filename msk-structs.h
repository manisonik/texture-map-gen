#pragma once
#include "msk-enum.h"

namespace TexGen {
	struct MouseEvent
	{
		bool mouseOver;
		bool mouseDown;
		float x, y;
	};

	struct KeyboardEvent
	{
		int key;
		int keyDown;
	};

	struct FontCharacter {
		GLfloat ax;
		GLfloat ay;
		GLfloat bw;
		GLfloat bh;
		GLfloat bl;
		GLfloat bt;
		GLfloat tx;
	};
}
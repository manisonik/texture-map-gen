#pragma once
#include "msk-enum.h"

namespace Manisonik {
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

	struct SFontCharacter {
		GLfloat ax;
		GLfloat ay;
		GLfloat bw;
		GLfloat bh;
		GLfloat bl;
		GLfloat bt;
		GLfloat tx;
	};
}
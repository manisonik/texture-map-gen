#include "msk-cairo-object.h"

#pragma once
class CCairoRectangle
{
public:
	CCairoRectangle();
	~CCairoRectangle();

	void Move(int x, int y);
	void SetWidth(int width);
	void SetHeight(int height);
	void SetBounds(int x, int y, int width, int height);
	void Scale(int x, int y);
	void Rotate(int radius);
	void Shear();

	// Draws a filled rectangle
	void Fill(bool useFill);
	void Stroke(bool useStroke);
	void Draw();
};


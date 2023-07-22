#pragma once
#include "msk-core.h"

class MCairo : public
	MskCairo
{
private:
	cairo_t* m_pCairo;
	cairo_surface_t* m_pSurface;
	unsigned char* m_pSurfaceData;
	int m_nChannels;
	int m_nWidth;
	int m_nHeight;

public:
	MCairo();
	MCairo(int width, int height);
	~MCairo();

	GLsizei GetWidth();
	GLsizei GetHeight();
	GLvoid Clear(double r, double g, double b, double a);
	GLvoid Rectangle(double x, double y, double width, double height);
	GLvoid Circle(double x, double y, double width, double height);
	GLvoid Triangle(double x, double y, double width, double height);
	GLvoid FillRectangle(double x, double y, double width, double height);

	unsigned char* GetSurfaceData();
};


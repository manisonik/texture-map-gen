#include "stdafx.h"
#include "msk-cairo.h"

MCairo::MCairo() : 
	MCairo(1024, 1024)
{
}

MCairo::MCairo(int width, int height) :
	m_nChannels(4)
{
	// Store width and height
	m_nWidth = width;
	m_nHeight = height;

	// Create cairo-surface/context to act as OpenGL-texture source
	m_pSurfaceData = (unsigned char*)calloc(m_nChannels * width * height, sizeof(unsigned char));

	if (!m_pSurfaceData)
	{
		free(m_pSurfaceData);
		throw std::exception("Cairo - Couldn't allocate buffer\n");
	}

	m_pSurface = cairo_image_surface_create_for_data(
		m_pSurfaceData,
		CAIRO_FORMAT_ARGB32,
		width,
		height,
		m_nChannels * width
	);

	if (cairo_surface_status(m_pSurface) != CAIRO_STATUS_SUCCESS)
	{
		free(m_pSurfaceData);
		throw std::exception("Cairo -  Couldn't create surface\n");
	}

	m_pCairo = cairo_create(m_pSurface);
	if (cairo_status(m_pCairo) != CAIRO_STATUS_SUCCESS)
	{
		free(m_pSurfaceData);
		throw std::exception("Cairo - Couldn't create context\n");
	}
}

MCairo::~MCairo(){
	cairo_destroy(m_pCairo);
	cairo_surface_destroy(m_pSurface);
	free(m_pSurfaceData);

	m_pCairo = NULL;
	m_pSurface = NULL;
	m_pSurfaceData = NULL;
}

GLsizei MCairo::GetWidth()
{
	return m_nWidth;
}

GLsizei MCairo::GetHeight()
{
	return m_nHeight;
}

GLvoid MCairo::Clear(double r, double g, double b, double a)
{
	cairo_set_source_rgba(m_pCairo, r, g, b, a);
	cairo_rectangle(m_pCairo, 0, 0, m_nWidth, m_nHeight);
	cairo_fill(m_pCairo);
}

GLvoid MCairo::Rectangle(double x, double y, double width, double height) {
	cairo_set_line_width(m_pCairo, 100);
	cairo_set_line_join(m_pCairo, CAIRO_LINE_JOIN_BEVEL);
	cairo_set_source_rgb(m_pCairo, 1.0, 0.0, 0.0);
	cairo_rectangle(m_pCairo, x, y, width, height);
	cairo_stroke(m_pCairo);
}

GLvoid MCairo::Circle(double x, double y, double width, double height) {
	double stroke = 5;
	cairo_set_source_rgb(m_pCairo, 0, 0, 0);
	cairo_arc(m_pCairo, stroke / 2 + x + width / 2.0f, stroke / 2 + y + width / 2.0f, width / 2.0f, 0, 2 * M_PI);
	cairo_set_line_width(m_pCairo, stroke);
	cairo_stroke_preserve(m_pCairo);
	cairo_set_source_rgb(m_pCairo, 0, 0, 1);
	cairo_fill(m_pCairo);
}

GLvoid MCairo::Triangle(double x, double y, double width, double height) {
	cairo_set_source_rgb(m_pCairo, 0, 0, 0);
	cairo_move_to(m_pCairo, x, y);
	cairo_line_to(m_pCairo, x + width / 2.0f, y + height);
	cairo_line_to(m_pCairo, x + width, y);
	cairo_fill(m_pCairo);
}

GLvoid MCairo::FillRectangle(double x, double y, double width, double height)
{
	double aspect = 10.0;     /* aspect ratio */
	double corner_radius = height / 1.0f;   /* and corner curvature radius */
	double radius = corner_radius / aspect;
	double degrees = M_PI / 180.0;

	cairo_save(m_pCairo);
		cairo_new_sub_path(m_pCairo);
		cairo_arc(m_pCairo, x + width - radius, y + radius, radius, -90 * degrees, 0 * degrees);
		cairo_arc(m_pCairo, x + width - radius, y + height - radius, radius, 0 * degrees, 90 * degrees);
		cairo_arc(m_pCairo, x + radius, y + height - radius, radius, 90 * degrees, 180 * degrees);
		cairo_arc(m_pCairo, x + radius, y + radius, radius, 180 * degrees, 270 * degrees);
		cairo_close_path(m_pCairo);

		cairo_set_source_rgb(m_pCairo, 0.5, 0.5, 1);
		cairo_fill_preserve(m_pCairo);
		cairo_set_source_rgba(m_pCairo, 0.5, 0, 0, 0.5);
		cairo_set_line_width(m_pCairo, 5.0);
		cairo_stroke(m_pCairo);
	cairo_restore(m_pCairo);
}

unsigned char* MCairo::GetSurfaceData()
{
	return m_pSurfaceData;
}

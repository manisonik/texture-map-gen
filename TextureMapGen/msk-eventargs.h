#pragma once

enum PaintArea {
	Client,
	NonClient
};

class EventArgs
{
public:
	EventArgs();
	~EventArgs();
};

class MouseEventArgs : public EventArgs {
public:
	MouseEventArgs() {}
	~MouseEventArgs() {}
	GLuint x;
	GLuint y;
	GLuint buttonDown;
};

class PaintArgs : public EventArgs {
public:
	PaintArgs() {}
	~PaintArgs() {}
	PaintArea Area;
};

class RenderArgs : public EventArgs {
public:
	RenderArgs() {}
	~RenderArgs() {}
};

class UpdateEventArgs : public EventArgs {
public:
	UpdateEventArgs() {}
	~UpdateEventArgs() {}

	GLdouble DeltaTime;
};

class KeyEventArgs : public EventArgs {
public:
	KeyEventArgs() {}
	~KeyEventArgs() {}

	GLuint Key;
};

class ResizeEventArgs : public EventArgs {
public:
	ResizeEventArgs() {}
	~ResizeEventArgs() {}

	GLuint m_nWidth;
	GLuint m_nHeight;
};




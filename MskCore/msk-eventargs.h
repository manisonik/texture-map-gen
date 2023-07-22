#pragma once
#include "msk-enum.h"
#include "msk-object.h"

namespace Manisonik {
	class MEventArgs : public MObject {};

	class MKeyEventArgs : public MEventArgs
	{
	public:
		MKeyEventArgs()
		{
			KeyCode = (Keys)0;
			KeyData = (Keys)0;
			Alt = false;
			Shift = false;
			Control = false;
		}
		Keys KeyCode;
		Keys KeyData;
		bool Alt;
		bool Shift;
		bool Control;
	};

	class MShutdownEventArgs : public MEventArgs
	{
	public:
		MShutdownEventArgs() {}
	};

	class MMouseEventArgs : public MEventArgs
	{
	public:
		MMouseEventArgs() 
		{
			this->button = MouseButton::None;
			this->x = 0;
			this->y = 0;
		}
		MouseButton button;
		int32_t x;
		int32_t y;
	};

	class MResizeEventArgs : public MEventArgs
	{
	public:
		MResizeEventArgs() 
		{
			this->width = 0;
			this->height = 0;
		}
		int32_t width;
		int32_t height;
	};
}
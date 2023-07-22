#pragma once
#include "msk-core.h"

class MOGLView : 
	public MskOglView
{
public:
	MOGLView();
	~MOGLView();

	void Show();
	void Hide();
	void OnRenderScene(MskObject* sender, EventArgs& e);
};


#pragma once
#include "msk-gl-texture.h"

namespace TexGen {
	class TgTextureRegion
	{
	private:
		TgTexture m_texture;
		float m_u;
		float m_v;
		float m_u2;
		float m_v2;
		float m_regionWidth;
		float m_regionHeight;

	public:
		TgTextureRegion();
		TgTextureRegion(const TgTexture& texture);
		TgTextureRegion(const TgTexture& texture, float width, float height);
		TgTextureRegion(const TgTexture& texture, float x, float y, float width, float height);
		~TgTextureRegion();

		//void SetRegion(float u, float v, float u2, float v2);
		void setRegion(float x, float y, float width, float height);
		void setRegion(const TgTexture& texture);
		float getU() const;
		float getV() const;
		float getU2() const;
		float getV2() const;
		const TgTexture& getTexture() const;
	};
}


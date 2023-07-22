#pragma once
#include "msk-gl-texture.h"

namespace Manisonik {
	class MTextureRegion
	{
	private:
		std::shared_ptr<MTexture> m_pTexture;
		float m_u;
		float m_v;
		float m_u2;
		float m_v2;
		float m_nRegionWidth;
		float m_nRegionHeight;

	public:
		MTextureRegion();
		MTextureRegion(std::shared_ptr<MTexture>& texture);
		//MTextureRegion(std::shared_ptr<MTexture>& texture, float u, float v, float u2, float v2);
		MTextureRegion(std::shared_ptr<MTexture>& texture, float x, float y, float width, float height);
		~MTextureRegion();

		//void SetRegion(float u, float v, float u2, float v2);
		void SetRegion(float x, float y, float width, float height);
		void SetRegion(std::shared_ptr<MTexture> texture);
		float GetU();
		float GetV();
		float GetU2();
		float GetV2();
		std::shared_ptr<MTexture>& GetTexture();
	};
}


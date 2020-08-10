#pragma once
#include "msk-core.h"

class MImage : public MskImage
{
protected:
	unsigned int m_nWidth;
	unsigned int m_nHeight;
	unsigned char m_nColorType;
	unsigned char m_nBitDepth;
	unsigned int m_nNumOfPasses;
	std::shared_ptr<unsigned char> m_pBytes;

public:
	MImage();
	virtual ~MImage();

	HRESULT __stdcall Load(std::string filename);
	HRESULT __stdcall GetImageData(std::shared_ptr<unsigned char>& data);
};


#include "stdafx.h"
#include "msk-gui-image.h"


MImage::MImage()
{
	m_nWidth = 0;
	m_nHeight = 0;
	m_nColorType = 0;
	m_nBitDepth = 0;
	m_nNumOfPasses = 0;
	m_pBytes = 0;
}

MImage::~MImage()
{
}

HRESULT MImage::Load(std::string filename)
{
	FILE* fp = NULL;
	errno_t err;
	png_byte pngsig[8];

	/* Open file and check signature */
	err = fopen_s(&fp, filename.c_str(), "rb");

	// Error occured while opening file
	if (err != 0)
		return E_FAIL;

	if (fp == NULL)
		return E_FAIL;

	// File is not recognized as a PNG file
	fread(pngsig, 1, 8, fp);
	if (png_sig_cmp(pngsig, 0, 8))
		return E_FAIL;

	// PNG cannot be initialized
	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png_ptr)
		return E_FAIL;

	// PNG cannot be initialized
	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr)
		return E_FAIL;

	// PNG cannot be initialized
	if (setjmp(png_jmpbuf(png_ptr)))
		return E_FAIL;

	png_init_io(png_ptr, fp);
	png_set_sig_bytes(png_ptr, 8);
	png_read_info(png_ptr, info_ptr);

	m_nWidth		= png_get_image_width(png_ptr, info_ptr);
	m_nHeight		= png_get_image_height(png_ptr, info_ptr);
	m_nColorType	= png_get_color_type(png_ptr, info_ptr);
	m_nBitDepth		= png_get_bit_depth(png_ptr, info_ptr);
	m_nNumOfPasses	= png_set_interlace_handling(png_ptr);

	png_read_update_info(png_ptr, info_ptr);

	// There is a problem reading PNG file.
	if (setjmp(png_jmpbuf(png_ptr)))
		return E_FAIL;

	// Allocate the data
	png_bytep* row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * m_nHeight);
	for (unsigned int y = 0; y < m_nHeight; y++) {
		row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png_ptr, info_ptr));
	}

	// Read the image
	png_read_image(png_ptr, row_pointers);

	// Save a copy
	m_pBytes = std::shared_ptr<unsigned char>((unsigned char*)row_pointers);

	// Close the file
	fclose(fp);

	return S_OK;
}

HRESULT MImage::GetImageData(std::shared_ptr<unsigned char>& data)
{
	data = m_pBytes;

	return S_OK;
}

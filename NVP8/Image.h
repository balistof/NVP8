#pragma once

using namespace System;

#define VPX_CODEC_DISABLE_COMPAT 1
#include<vpx/vpx_image.h>

namespace NVP8 
{
	public ref class VPXImage
	{
	public:
		VPXImage(int width, int height)
		{
			imageData = new vpx_image_t;
			vpx_img_alloc(imageData, VPX_IMG_FMT_I420, width, height, 1); 
		}

		!VPXImage()
		{
			vpx_img_free(imageData);
			delete imageData;
		}

		~VPXImage()
		{
			this->!VPXImage();
		}

		vpx_image_t* GetImageData()
		{
			return imageData;
		}

	private:
		vpx_image_t* imageData;
	};
}
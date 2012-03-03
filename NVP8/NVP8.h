// NVP8.h

#pragma once

using namespace System;

#define VPX_CODEC_DISABLE_COMPAT 1
#include<vpx\vpx_encoder.h>
#include<vpx\vpx_decoder.h>
#include<vpx\vp8cx.h>
#include<vpx\vp8dx.h>
#define interface (vpx_codec_vp8_cx())

namespace NVP8 {

	public ref class Encoder
	{
	public:
		Encoder(int width, int height)
		{
			/* Populate encoder configuration */
			cfg = new vpx_codec_enc_cfg_t();
			int res = vpx_codec_enc_config_default(interface, cfg, 0);
			if(res) 
			{
				// todo: error handling
			}    

			codec = new vpx_codec_ctx_t();
			if(vpx_codec_enc_init(codec, interface, cfg, 0))
			{
				// todo: error handling
			}
		}

		!Encoder()
		{
			delete cfg;
			delete codec;
		}

		~Encoder()
		{
			this->!Encoder();
		}

		int Encode()
		{
			return false;
		}

	private:
		vpx_codec_ctx_t* codec;
		vpx_codec_enc_cfg_t* cfg;
	};
}

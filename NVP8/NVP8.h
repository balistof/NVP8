#pragma once

using namespace System;
using namespace System::Runtime::InteropServices;

#define VPX_CODEC_DISABLE_COMPAT 1
#include<vpx\vpx_encoder.h>
#include<vpx\vpx_decoder.h>
#include<vpx\vp8cx.h>
#include<vpx\vp8dx.h>
#define interface (vpx_codec_vp8_cx())

#include"Image.h"

namespace NVP8 
{
	public ref class Packet
	{
	public:
		Packet(const vpx_codec_cx_pkt* packetData)
		{
			this->packetData = packetData;
		}

		void Init(const vpx_codec_cx_pkt* packetData)
		{
			this->packetData = packetData;
		}

		int WriteAsIVF(array<Byte>^ data, int offset, int length)
		{
			if(packetData->kind != VPX_CODEC_CX_FRAME_PKT)
				return 0;
 
			// write IVF header
			Array::Copy(BitConverter::GetBytes(packetData->data.frame.sz), 0, data, offset, sizeof(size_t));
			offset += sizeof(size_t);
			vpx_codec_pts_t pts = packetData->data.frame.pts;
			Array::Copy(BitConverter::GetBytes(pts), 0, data, offset, sizeof(vpx_codec_pts_t));
			offset += sizeof(vpx_codec_pts_t);
 
			// write payload
			Marshal::Copy((IntPtr)packetData->data.frame.buf, data, offset, packetData->data.frame.sz);
			return sizeof(size_t) + sizeof(vpx_codec_pts_t) + packetData->data.frame.sz;
		}

	private:
		const vpx_codec_cx_pkt* packetData;
	};

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

			cfg->rc_end_usage = VPX_VBR;
			cfg->rc_target_bitrate = width * height * cfg->rc_target_bitrate / cfg->g_w / cfg->g_h;
			cfg->g_w = width;
			cfg->g_h = height;

			codec = new vpx_codec_ctx_t();
			if(vpx_codec_enc_init(codec, interface, cfg, 0))
			{
				// todo: error handling
			}
		}

		bool Encode(VPXImage^ image, int timeStamp)
		{
			iter = NULL;
			return vpx_codec_encode(codec, image->GetImageData(), timeStamp, 1, 0, VPX_DL_REALTIME) == VPX_CODEC_OK;
		}

		bool GetEncodedPacket(Packet^ packetData)
		{
            const vpx_codec_cx_pkt_t* pkt = vpx_codec_get_cx_data(codec, iter);
			if (pkt != NULL && pkt->kind == VPX_CODEC_CX_FRAME_PKT)
			{
				packetData->Init(pkt);
				return true;
			}
			return false;
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

	private:
		vpx_codec_ctx_t* codec;
		vpx_codec_enc_cfg_t* cfg;
		vpx_codec_iter_t* iter;
	};
}

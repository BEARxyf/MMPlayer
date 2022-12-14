#include "MMAV.h"
#include "MMAVDecoderPrivate.h"
#include "MMAVStreamPrivate.h"
#include "MMAVPacketPrivate.h"
#include "MMAVFramePrivate.h"

MMAVDecoder::MMAVDecoder()
{
	imp = new MMAVDecoderPrivate();
	imp->codecContext = avcodec_alloc_context3(nullptr);
}
MMAVDecoder::~MMAVDecoder()
{
	if (imp->codecContext != nullptr) {
		avcodec_free_context(&imp->codecContext);
		imp->codecContext = nullptr;
	}
	if (imp != nullptr) {
		delete imp;
		imp = nullptr;
	}
}

int MMAVDecoder::Init(MMAVStream* stream)
{
	avcodec_parameters_to_context(imp->codecContext, stream->imp->codecpar);//
	AVCodec* avCodec = avcodec_find_decoder(imp->codecContext->codec_id);

	int ret = avcodec_open2(imp->codecContext, avCodec, nullptr);
	if (ret) {
		printf("avcodec_open2 failed:%d\n", ret);
		return -1;
	}
	return 0;
}

int MMAVDecoder::SendPacket(MMAVPacket* pkt)
{
	int ret = 0;
	if (pkt == nullptr) {
		ret = avcodec_send_packet(imp->codecContext, nullptr);
	}
	else {
		ret = avcodec_send_packet(imp->codecContext, pkt->imp->pkt);

	}
	return ret;
}
int MMAVDecoder::RecvFrame(MMAVFrame* frame)
{
	int ret = avcodec_receive_frame(imp->codecContext, frame->imp->frame);

	return ret;
}

int MMAVDecoder::Close()
{
	return avcodec_close(imp->codecContext);
}


int MMAVDecoder::GetData_size()
{
	return av_get_bytes_per_sample(imp->codecContext->sample_fmt); // 每帧音频数据量的大小
}
bool MMAVDecoder::isPlanar()
{
	if (av_sample_fmt_is_planar(imp->codecContext->sample_fmt))
		return true;
	else
		return false;
}
int MMAVDecoder::GetChannels()
{
	return imp->codecContext->channels;
}


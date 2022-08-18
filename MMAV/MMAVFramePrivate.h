#pragma once

extern "C"
{
#include "../3rdpart/ffmpeg/include/libavutil/frame.h"
//#include "../3rdpart/ffmpeg/include/libswresample/swresample.h"
//#include "../3rdpart/ffmpeg/include/libavutil/opt.h"
//#include "../3rdpart/ffmpeg/include/libavutil/channel_layout.h"
//#include "../3rdpart/ffmpeg/include/libavutil/avutil.h"

};

class MMAVFramePrivate
{
public:
	AVFrame* frame = nullptr;


};
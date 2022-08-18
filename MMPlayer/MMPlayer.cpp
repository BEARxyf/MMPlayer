// MMPlayer.cpp: 定义应用程序的入口点。
//

#include <thread>
#include <chrono>
#include <vector>
#include "MMThread/MMThread.h"

void threadFunc(int i)
{
	printf("Thread Function%d\n", i);
}

void threadFun1(int index) {
	for (int i = 0; i < 1000; i++) {
		printf("Thread:%d,index%d\n", index, i);
	}
}

//int main2()
//{
//	std::thread t1(threadFun1,1);
//	std::thread t2(threadFun1, 2);
//	t1.join();
//	t2.join();
//	return 0; 
//}

class MyMMThread :public MMThread
{
public:
	virtual void run()
	{
		printf("MyMMThread\n");
	}
};
//int main_thread()
//{
//	MyMMThread t;
//	t.Start();
//	std::this_thread::sleep_for(std::chrono::seconds(2));
//	return 0;
//}

#include "MMAV/MMAV.h"
#include "MMAV/MMAVFramePrivate.h"
int main()
{
	MMAVReader reader;
	int ret = reader.Open("d://test_media/test_1920x1080.mp4");
	if (ret) {
		printf("Open Failed..\n");
		return -1;
	}

	int videoStreamIndex = reader.GetVideoStreamIndex();
	int audioStreamIndex = reader.GetAudioStreamIndex();
	// 音频数据——————
	//uint8_t** src_data, ** dst_data;
	//int src_nb_channels = 0, dst_nb_channels = 0;
	//int64_t src_ch_layout = AV_CH_LAYOUT_STEREO, dst_ch_layout = AV_CH_LAYOUT_STEREO;


	//——————————
	printf("VideoStreamIndex:%d\n", videoStreamIndex);
	printf("AudioStreamIndex:%d\n", audioStreamIndex);
	//打印媒体信息：av_dump_format

	std::vector<MMAVDecoder*>decoderList;

	int streamCount = reader.GetStreamCount();
	for (int i = 0; i < streamCount; i++) {
		MMAVStream avSream;
		reader.GetStream(&avSream, i);
		printf("StreamIndex:%d\n", avSream.streamIndex);//找到了两个流

		//初始化解码器
		MMAVDecoder* decoder = new MMAVDecoder();
		int ret = decoder->Init(&avSream);
		if (ret) {
			printf("Init decoder failed\n");
		}
		decoderList.push_back(decoder);
	}

	FILE* f = fopen("d://test_media/test_1920x1080.yuv", "wb");
	FILE* f1 = fopen("d://test_media/test_1920x1080.pcm", "wb");//输出pcm
	//FILE* f2 = fopen("d://test_media/test_1920x1080_f32Tos16.pcm", "wb");

	while (1) {
		MMAVPacket pkt;
		ret = reader.Read(&pkt);
		if (ret) {
			break;
		}
		//printf("Read Packet Success\n");
		int streamIndex = pkt.GetIndex();
		MMAVDecoder* decoder = decoderList[streamIndex];
		ret = decoder->SendPacket(&pkt);
		if (ret) {//send失败
			continue;
		}

		while (1) {
			MMAVFrame frame;
			ret = decoder->RecvFrame(&frame);
			if (ret) {
				break;
			}
			//recv success
			if (streamIndex == videoStreamIndex) {
				//frame.VideoPrint();
				int width = frame.GetW();
				int height = frame.GetH();
				unsigned char* y = (unsigned char*)malloc(width * height);
				unsigned char* u = (unsigned char*)malloc(width / 2 * height / 2);
				unsigned char* v = (unsigned char*)malloc(width / 2 * height / 2);
				frame.GetY(y);
				frame.GetU(u);
				frame.GetV(v);

				fwrite(y, width * height, 1, f);
				fwrite(u, width / 2 * height / 2, 1, f);
				fwrite(v, width / 2 * height / 2, 1, f);

				free(y);
				free(u);
				free(v);
			}//
			if (streamIndex == audioStreamIndex) {
				frame.AudioPrint();
				int data_size = decoder->GetData_size();
				int channels = decoder->GetChannels();
				int nb_samples = frame.imp->frame->nb_samples;
				if (!(decoder->isPlanar())) {
					printf("pcm planar模式\n");
					for (int i = 0; i < nb_samples; i++) {
						for (int ch = 0; ch < channels; ch++) {
							fwrite(frame.imp->frame->data[ch] + data_size * i, 1, data_size,f1 );
						}
					}
				}
				else {
					printf("pcm pack模式\n");
					fwrite(frame.imp->frame->data[0], nb_samples * data_size,1, f1);
					fwrite(frame.imp->frame->data[1], nb_samples * data_size, 1, f1);
					//swr_convert
				}
			}
			//frame.VideoPrint();
		}
	}
	//清空解码器
	for (int i = 0; i < decoderList.size(); i++) {
		MMAVDecoder* decoder = decoderList[i];
		ret = decoder->SendPacket(nullptr);//告诉解码器发送完了
		while (1) {
			MMAVFrame frame;
			ret = decoder->RecvFrame(&frame);
			if (ret) {
				break;
			}
		}
	}

	reader.Close();
	//释放
	for (int i = 0; i < decoderList.size(); i++) {
		MMAVDecoder* decoder = decoderList[i];
		decoder->Close();
		delete decoder;
	}
	decoderList.clear();
	fclose(f);
	fclose(f1);
	//fclose(f2);
	return 0;
}
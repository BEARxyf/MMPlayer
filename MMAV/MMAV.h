#pragma once
class MMAVFramePrivate;
class MMAVPacketPrivate;
class MMAVStreamPrivate;

class MMAVStream;

class MMAVPacket
{
public:
	MMAVPacket();
	~MMAVPacket();
	int GetIndex();
public:
	MMAVPacketPrivate* imp = nullptr;
};

class MMAVFramePrivate;

class MMAVFrame
{
public:
	MMAVFrame();
	~MMAVFrame();
	int VideoPrint();
	int AudioPrint();
	int GetW();
	int GetH();
	int GetY(unsigned char* y);
	int GetU(unsigned char* u);
	int GetV(unsigned char* v);
public:
	MMAVFramePrivate* imp = nullptr;
};

class MMAVReaderPrivate;

class MMAVReader
{
public:
	MMAVReader();
	~MMAVReader();
	int Open(const char* path);
	int Close();
	int GetStreamCount();
	int GetStream(MMAVStream* stream, int streamId);
	int Read(MMAVPacket* packet);
	int GetVideoStreamIndex();
	int GetAudioStreamIndex();
private:
	MMAVReaderPrivate* imp = nullptr;
};

class MMAVStreamPrivate;

class MMAVStream
{
public:
	MMAVStream();
	~MMAVStream();
public:
	int streamIndex = -1;
	MMAVStreamPrivate* imp = nullptr;
};


class MMAVDecoderPrivate;

class MMAVDecoder
{
public:
	MMAVDecoder();
	~MMAVDecoder(); 
	int Init(MMAVStream* stream);
	int SendPacket(MMAVPacket* pkt);
	int RecvFrame(MMAVFrame* frame);
	int Close();
	int GetData_size();
	bool isPlanar();
	int GetChannels();
	
private:
	MMAVDecoderPrivate* imp = nullptr;
};
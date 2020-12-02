#pragma once


#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <cstdint>
#include <thread>
#include <mutex>
#include <deque>

extern "C" {
#include "vcinpl.h"
#include "vcitype.h"
#include "vcierr.h"
}

struct CanMessage
{
	std::int32_t payloadLen;
	std::uint32_t id;
	unsigned char bytes[8];
};

struct IxxatDevice
{
	VCIID deviceId;
	UINT32 ctrlIdx;
	std::thread worker;
	bool m_valid;
	bool m_exit;
	std::mutex mutex;
	std::deque<CanMessage> m_queue;
	HANDLE m_canDevHandle;
	HANDLE m_canCtrlHandle;
	HANDLE m_canChannelHandle;

	IxxatDevice();
	void Run();
	void Stop();
	bool Send(const CanMessage &message);
	void Flush();
	bool Read(CanMessage &message);
};

struct IxxatVciBaudRateRegisters {
	char baud_rate[5];
	std::uint8_t bt0;
	std::uint8_t bt1;
};

class Ixxat
{
public:
	Ixxat();

	void Enumerate();
	bool Create(const std::string & name);
	std::string GetNameAt(size_t index);
	size_t GetNumberOfDevices();

	bool Init(const std::string &name, const std::string &bitRate);
	void Stop(const std::string &name);
	void Flush(const std::string &name);
	bool Send(const std::string &name, const CanMessage &message);
	bool Read(const std::string &name, CanMessage &message);

	std::string GetErrorMessage();
	bool HasError();

private:
	std::map<std::string, VCIID> m_devList;
	std::map<std::string, std::shared_ptr<IxxatDevice>> m_controllers;
	HANDLE devEnumeratorHandle;
};


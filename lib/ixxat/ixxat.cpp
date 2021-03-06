
#include "pch.h"
#include "Ixxat.h"
#include <deque>
#include <string>

static std::string mLastError;
static bool mHasError = false;

inline IxxatDevice::IxxatDevice()
	: m_valid(false)
	, m_exit(false)
	, m_canDevHandle(nullptr)
	, m_canCtrlHandle(nullptr)
	, m_canChannelHandle(nullptr)
{

}

void IxxatDevice::Stop()
{
	mutex.lock();
	m_queue.clear();
	m_exit = true;
	mutex.unlock();

	if (worker.joinable())
	{
		worker.join();
	}

	if (m_canChannelHandle != nullptr)
	{
		std::cout << "Close IXXAT VCI channel" << std::endl;
		canChannelActivate(m_canChannelHandle, false);
		canChannelClose(m_canChannelHandle);
	}

	if (m_canCtrlHandle != nullptr)
	{
		std::cout << "Close IXXAT VCI controller" << std::endl;
		canControlStart(m_canCtrlHandle, false);
		canControlReset(m_canCtrlHandle);
		canControlClose(m_canCtrlHandle);
	}

	if (m_canDevHandle != nullptr)
	{
		std::cout << "Close IXXAT VCI device" << std::endl;
		vciDeviceClose(m_canDevHandle);
	}

	m_valid = false;
	m_canDevHandle = nullptr;
	m_canCtrlHandle = nullptr;
	m_canChannelHandle = nullptr;
	
}

void IxxatDevice::Run()
{
	CANMSG canMsg;

	while (!m_exit)
	{
		std::cout << "Waiting for CAN messages ..." << std::endl;
		memset(&canMsg, 0x00, sizeof(canMsg));
		// Timeout is in milliseconds
		HRESULT result = canChannelReadMessage(m_canChannelHandle, 1000, &canMsg);
		if (result == VCI_OK)
		{
			if (canMsg.uMsgInfo.Bytes.bType == CAN_MSGTYPE_DATA)
			{
				if (canMsg.uMsgInfo.Bits.rtr == 0)
				{
					CanMessage msg;

					msg.id = (std::uint32_t)(canMsg.dwMsgId);
					msg.payloadLen = (std::int32_t)(canMsg.uMsgInfo.Bits.dlc);

					if (msg.payloadLen <= 8)
					{
						std::cout << "Received CAN message: Size=" << (int)msg.payloadLen << " ID=" << msg.id << " DATA= ";
						for (int i = 0; i < msg.payloadLen; i++)
						{
							msg.bytes[i] = canMsg.abData[i];
							std::cout << std::hex << (int)canMsg.abData[i] << ", ";
						}

						std::cout << std::endl;

						mutex.lock();
						m_queue.push_back(msg);
						mutex.unlock();
					}
					else
					{
						mLastError = "Bad received message size";
						mHasError = true;
					}
				}
				else
				{
					std::cout << "\nTime: " << (int)canMsg.dwTime << " ID: " << (int)canMsg.dwMsgId << " DLC: " << (int)canMsg.uMsgInfo.Bits.dlc << " Remote Frame " << std::endl;
				}
			}
			else if (canMsg.uMsgInfo.Bytes.bType == CAN_MSGTYPE_INFO)
			{
				//
				// show informational frames
				//
				switch (canMsg.abData[0])
				{
				case CAN_INFO_START: std::cout << "\nCAN started..." << std::endl; break;
				case CAN_INFO_STOP: std::cout << "\nCAN stoped..." << std::endl;  break;
				case CAN_INFO_RESET: std::cout << "\nCAN reseted..." << std::endl; break;
				}
			}
			else if (canMsg.uMsgInfo.Bytes.bType == CAN_MSGTYPE_ERROR)
			{
				//
			// show error frames
			//
				switch (canMsg.abData[0])
				{
					case CAN_ERROR_STUFF: std::cout << "\nstuff error..." << std::endl;          break;
					case CAN_ERROR_FORM: std::cout << "\nform error..." << std::endl;           break;
					case CAN_ERROR_ACK: std::cout << "\nacknowledgment error..." << std::endl; break;
					case CAN_ERROR_BIT: std::cout << "\nbit error..." << std::endl;            break;
					case CAN_ERROR_CRC: std::cout << "\nCRC error..." << std::endl;            break;
					case CAN_ERROR_OTHER:
					default: printf("\nother error...");          break;
				}
			}
		}
		else if (result == HRESULT(VCI_E_TIMEOUT) ||
			result == HRESULT(VCI_E_RXQUEUE_EMPTY)) {
			// NOTE : nothing to read yet...
		}
		else {
			std::cout << "Error " << result << " receiving CAN frame ! ";
		}
	}

	std::cout << "CAN worker thread stopped." << std::endl;
}

bool IxxatDevice::Send(const CanMessage &message)
{
	bool ret = false;

	static CANMSG canMsg;
	memset(&canMsg, 0x00, sizeof(canMsg));

	//UINT payloadLen = 8;
	canMsg.dwTime = 0;
	canMsg.dwMsgId = message.id;    // CAN message identifier

	//canMsg.uMsgInfo.Bits.type = CAN_MSGTYPE_DATA;
	//canMsg.uMsgInfo.Bits.dlc = message->getCanData().getLength();
	//canMsg.uMsgInfo.Bits.ext = message->getCanId().isEFF();
	//canMsg.uMsgInfo.Bits.rtr = message->getCanId().isRTR();

	canMsg.uMsgInfo.Bytes.bType = CAN_MSGTYPE_DATA;
	// Flags:
	// srr = 1
	canMsg.uMsgInfo.Bytes.bFlags = CAN_MAKE_MSGFLAGS(CAN_LEN_TO_SDLC(message.payloadLen), 0, 1, 0, 0);
	// Flags2:
	// Set bFlags2 to 0
	canMsg.uMsgInfo.Bytes.bFlags2 = CAN_MAKE_MSGFLAGS2(0, 0, 0, 0, 0);

	//canMsg.dwMsgId = (message->getCanId().isEFF() ? message->getCanId().canIdEFF() : message->getCanId().canIdSFF());
	//for (quint idx = 0; idx < CanData::MAX_BYTES; ++idx) {
	//	canMsg.abData[idx] = (idx < message->getCanData().getLength() ? message->getCanData().getByte(idx) : 0x00);
	//}

	for (int i = 0; i < message.payloadLen; i++)
	{
		canMsg.abData[i] = message.bytes[i];
	}

	if (canChannelPostMessage(m_canChannelHandle, &canMsg) == VCI_OK)
	{
		std::cout << "CAN message sent!" << std::endl;
		ret = true;
	}
	else
	{
		std::cout << "Can't send CAN message !" << std::endl;
	}

	return ret;
}

void IxxatDevice::Flush()
{
	mutex.lock();
	m_queue.clear();
	mutex.unlock();
}

bool IxxatDevice::Read(CanMessage & message)
{
	bool hasMessage = false;
	
	mutex.lock();
	if (m_queue.size() > 0)
	{
		message = m_queue.front();
		m_queue.pop_front();
		hasMessage = true;
	}
	mutex.unlock();
	
	return hasMessage;
}

// ===============================================================================================================
// =====================================  IXXAT SINGLE MAIN OBJECT =====================================
// ===============================================================================================================
void Ixxat::Enumerate()
{
	if (vciEnumDeviceOpen(&devEnumeratorHandle) == VCI_OK) {
		vciEnumDeviceReset(devEnumeratorHandle);
		HANDLE devHandle;
		VCIDEVICEINFO devInfo;
		
		m_devList.clear();
		while (vciEnumDeviceNext(devEnumeratorHandle, &devInfo) == VCI_OK)
		{
			if (vciDeviceOpen(devInfo.VciObjectId, &devHandle) == VCI_OK)
			{
				std::cout << "Found: " << devInfo.UniqueHardwareId.AsChar << "\r\n"
					<< "Manufacturer: " << devInfo.Manufacturer << "\r\n"
					<< "Description: " << devInfo.Description << "\r\n";

				m_devList[devInfo.UniqueHardwareId.AsChar] = devInfo.VciObjectId;

				vciDeviceClose(devHandle);
			}
		}
		vciEnumDeviceClose(devEnumeratorHandle);
	}
	else {
		std::cout << "Can't open VCI device enumerator !" << std::endl;
	}
}

bool Ixxat::Create(const std::string &name)
{
	bool success = false;
	VCIDEVICECAPS devCaps;

	for (const auto &dev : m_devList)
	{
		if (dev.first == name)
		{
			HANDLE devHandle;
			if (vciDeviceOpen(dev.second, &devHandle) == VCI_OK)
			{
				vciDeviceGetCaps(devHandle, &devCaps);

				for (int ctrlIdx = 0; ctrlIdx < devCaps.BusCtrlCount; ++ctrlIdx)
				{
					if (((devCaps.BusCtrlTypes[ctrlIdx] >> 8) & 0xFF) == VCI_BUS_CAN)
					{
						std::shared_ptr<IxxatDevice> addr = std::make_shared<IxxatDevice>();
						addr->deviceId = dev.second;
						addr->ctrlIdx = ctrlIdx;
						m_controllers[name] = addr;
						success = true;
						break;
					}
				}
			}
			vciDeviceClose(devHandle);
		}
	}

	return success;
}


bool Ixxat::Init(const std::string &name, const std::string &bitRate)
{
	bool success = false;
	if ((name.size() > 0) && Create(name))
	{
		std::cout << "Using controller with name : " << name << std::endl;

		if (m_controllers.count(name) > 0)
		{
			std::shared_ptr<IxxatDevice> ctrlAddr = m_controllers.at(name);

			std::cout << "Using board " << ctrlAddr->deviceId.AsInt64 << ", controller " << ctrlAddr->ctrlIdx << std::endl;

			if (!ctrlAddr->m_valid)
			{
				const int baudRatesCount = 9;
				const IxxatVciBaudRateRegisters listBaudRates[baudRatesCount] = {
					{ "10K",  0x31, 0x1C },
					{ "20K",  0x18, 0x1C },
					{ "50K",  0x09, 0x1C },
					{ "100K", 0x04, 0x1C },
					{ "125K", 0x03, 0x1C },
					{ "250K", 0x01, 0x1C },
					{ "500K", 0x00, 0x1C },
					{ "800K", 0x00, 0x16 },
					{ "1M",   0x00, 0x14 },
				};
				int idxBaudRate = -1;
				for (int tmp = 0; tmp < baudRatesCount; ++tmp) {
					if (listBaudRates[tmp].baud_rate == bitRate) {
						idxBaudRate = tmp;
						break;
					}
				}

				if (idxBaudRate > -1)
				{
					std::cout << "Using bitrate " << bitRate << std::endl;
					if (vciDeviceOpen(ctrlAddr->deviceId, &ctrlAddr->m_canDevHandle) == VCI_OK)
					{
						if (canControlOpen(ctrlAddr->m_canDevHandle, ctrlAddr->ctrlIdx, &ctrlAddr->m_canCtrlHandle) == VCI_OK)
						{
							const IxxatVciBaudRateRegisters btReg = listBaudRates[idxBaudRate];

							if (canControlInitialize(ctrlAddr->m_canCtrlHandle,
								CAN_OPMODE_STANDARD | CAN_OPMODE_EXTENDED | CAN_OPMODE_ERRFRAME,
								btReg.bt0,
								btReg.bt1) == VCI_OK)
							{
								if (canControlStart(ctrlAddr->m_canCtrlHandle, true) == VCI_OK)
								{
									if (canChannelOpen(ctrlAddr->m_canDevHandle, ctrlAddr->ctrlIdx, true, &ctrlAddr->m_canChannelHandle) == VCI_OK)
									{
										if (canChannelInitialize(ctrlAddr->m_canChannelHandle, 1000, 1, 1000, 1) == VCI_OK)
										{
											if (canChannelActivate(ctrlAddr->m_canChannelHandle, true) == VCI_OK)
											{
												std::cout << "CAN messages channel ready." << std::endl;

												ctrlAddr->m_valid = true;
												ctrlAddr->m_exit = false;
												ctrlAddr->worker = std::thread(&IxxatDevice::Run, ctrlAddr);

												success = true;
											}
											else {
												std::cout << "Can't activate CAN messages channel !" << std::endl;
											}
										}
										else {
											std::cout << "Can't initialize CAN messages channel !" << std::endl;
										}
									}
									else {
										std::cout << "Can't open CAN messages channel !" << std::endl;
									}
								}
								else {
									std::cout << "Can't start CAN controller !" << std::endl;
								}
							}
							else {
								std::cout << "Can't initialize CAN controller !" << std::endl;
							}

							if (!ctrlAddr->m_valid) {
								canChannelClose(ctrlAddr->m_canChannelHandle);
							}
						}
						else {
							std::cout << "Can't open CAN controller !" << std::endl;
						}
					}
					else {
						std::cout << "Can't open VCI device ! " << name << std::endl;
					}
				}
				else {
					std::cout << "Can't use invalid bitrate ! " << bitRate << std::endl;
				}
			}
			else
			{
				std::cout << "Device already initialized! " << name << std::endl;
			}
		}
		else {
			std::cout << "No controller with name! " << name << std::endl;
		}
	}
	else {
		std::cout << "Can't use empty controller name!" << std::endl;
	}

	return success;
}

void Ixxat::Stop(const std::string & name)
{
	if (name.size() > 0)
	{
		if (m_controllers.count(name) > 0)
		{
			std::cout << "IXXAT DLL: Stopping controller with name: " << name << std::endl;
			std::shared_ptr<IxxatDevice> ctrlAddr = m_controllers.at(name);

			if (ctrlAddr != nullptr)
			{
				ctrlAddr->Stop();
			}
			m_controllers.erase(name);
		}
		else
		{
			std::cout << "IXXAT DLL: cannot stop controller with name (not found): " << name << std::endl;
		}
	}
}

void Ixxat::Flush(const std::string & name)
{
	if (name.size() > 0)
	{
		if (m_controllers.count(name) > 0)
		{
			std::cout << "IXXAT DLL: flushing all CAN messages of controller with name: " << name << std::endl;
			std::shared_ptr<IxxatDevice> ctrlAddr = m_controllers.at(name);
			ctrlAddr->Flush();
		}
		else
		{
			std::cout << "IXXAT DLL: cannot flush controller with name (not found): " << name << std::endl;
		}
	}
}

bool Ixxat::Send(const std::string & name, const CanMessage & message)
{
	bool retCode = false;
	if (name.size() > 0)
	{
		std::cout << "Sending to controller with name : " << name << std::endl;

		if (m_controllers.count(name) > 0)
		{
			std::shared_ptr<IxxatDevice>  ctrlAddr = m_controllers.at(name);
			retCode = ctrlAddr->Send(message);
		}
	}
	return retCode;
}

bool Ixxat::Read(const std::string & name, CanMessage & message)
{
	bool retCode = false;
	if (name.size() > 0)
	{
		std::cout << "Reading from controller with name : " << name << std::endl;

		if (m_controllers.count(name) > 0)
		{
			std::shared_ptr<IxxatDevice> ctrlAddr = m_controllers.at(name);
			retCode = ctrlAddr->Read(message);
		}
	}
	return retCode;
}

std::string Ixxat::GetErrorMessage()
{
	return mLastError;
}

bool Ixxat::HasError()
{
	return false;
}

Ixxat::Ixxat()
{

}

std::string Ixxat::GetNameAt(size_t index)
{
	size_t i = 0;
	std::string name;

	if (index < m_devList.size())
	{
		for (const auto &dev : m_devList)
		{
			if (i == index)
			{
				name = dev.first;
				break;
			}
			i++;
		}
	}

	return name;
}

size_t Ixxat::GetNumberOfDevices()
{
	return m_devList.size();
}


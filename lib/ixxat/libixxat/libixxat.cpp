// libixxat.cpp : Définit les fonctions exportées pour l'application DLL.
//

#include "Ixxat.h"
#include "string.h"
#include "libixxat.h"

#include "TcpServer.h"
#include "Util.h"

static Ixxat gIxxat;

extern "C" size_t __stdcall ixxat_enumerate()
{
	gIxxat.Enumerate();
	return gIxxat.GetNumberOfDevices();
}

extern "C" void __stdcall ixxat_get_name(size_t index, char *name, int max_size)
{
	std::string nameFound = gIxxat.GetNameAt(index);

	if (nameFound.size() > 0)
	{
		sprintf_s(name, max_size, "%s", nameFound.c_str());
	}
}

extern "C" int __stdcall ixxat_init(const char * name, const char *bitRate)
{
	int retCode = 0;

	if (gIxxat.Init(name, bitRate))
	{
		retCode = 1;
	}

	return retCode;
}

extern "C" void __stdcall ixxat_stop(const char * name)
{
	gIxxat.Stop(name);
}

extern "C" int __stdcall ixxat_send(const char * name, unsigned long id, const char * data, int size)
{
	int retCode = 0;
	CanMessage msg;

	msg.id = id;
	for (int i = 0; i < size; i++)
	{
		msg.bytes[i] = data[i];
	}

	msg.payloadLen = size;

	if (gIxxat.Send(name, msg))
	{
		retCode = 1;
	}

	return retCode;
}

class TcpEventHandler : public tcp::TcpServer::IEvent
{
public:
	virtual ~TcpEventHandler() {}

	/**
	 * @brief NewConnection
	 * Called when a new TCP/IP connection has been created
	 * @param socket
	 */
	virtual void NewConnection(const tcp::Conn &conn)
	{
		std::cout << "Got cnx" << std::endl;
	}

	/**
	 * @brief ReadData
	 * Called when new data have been sent by the peer to the server
	 * @param socket
	 * @param data
	 */
	virtual void ReadData(const tcp::Conn &conn)
	{
		std::cout << "IXXAT DLL Got data: "  << conn.payload << std::endl;

		std::vector<std::string> request = Util::Split(conn.payload, ";");
		std::uint32_t size = request.size();
		std::string reply = "ERROR";

		if (size > 0)
		{
			std::string cmd = request[0];

			if (cmd == std::string("LIST"))
			{
				gIxxat.Enumerate();
				reply.clear();

				if (gIxxat.GetNumberOfDevices() > 0)
				{
					for (std::uint32_t i = 0U; i < gIxxat.GetNumberOfDevices(); i++)
					{
						reply += gIxxat.GetNameAt(i);

						if (i < (gIxxat.GetNumberOfDevices() - 1))
						{
							reply += ";";
						}
					}
				}
				else
				{
					reply = "EMPTY_LIST";
				}
			}
			else if (cmd == std::string("INIT"))
			{
				if (size >= 3)
				{
					std::string name = request[1];
					std::string bitRate = request[2];

					gIxxat.Stop(name); // stop any existing device
					if (gIxxat.Init(name, bitRate))
					{
						reply = "OK";
					}
				}
			}
			else if (cmd == std::string("FLUSH"))
			{
				if (size >= 2)
				{
					std::string name = request[1];
					gIxxat.Flush(name);
					reply = "OK";
				}
			}
			else if (cmd == std::string("SEND"))
			{
				if (size >= 3)
				{
					std::string name = request[1];
					CanMessage msg;

					std::uint32_t data_size = size - 3;
					data_size = data_size > 8 ? 8 : data_size; // protection

					msg.id = Util::FromString<std::uint32_t>(request[2]);
					if (data_size > 0)
					{
						
						for (std::uint32_t i = 0; i < data_size; i++)
						{
							msg.bytes[i] = static_cast<std::uint8_t>(Util::FromString<std::uint32_t>(request[3 + i]));
						}
					} 
					msg.payloadLen = data_size;
					std::cout << "LEN: " << (int)msg.payloadLen << std::endl;
					if (gIxxat.Send(name, msg))
					{
						reply = "OK";
					}
				}
			}
			else if (cmd == std::string("READ"))
			{
				reply = "NODATA";

				if (size >= 2)
				{
					std::string name = request[1];

					CanMessage msg;
					std::string devName(name);

					if (gIxxat.Read(devName, msg))
					{
						reply = std::to_string(msg.id) + ";";
						for (int i = 0; i < 8; i++)
						{
							if (i < msg.payloadLen)
							{
								reply += std::to_string(msg.bytes[i]);

								if (i < (msg.payloadLen - 1))
								{
									reply += ";";
								}
							}
						}
					}
				}
			}
		}

		tcp::TcpSocket peer(conn.peer);
		peer.Send(reply);
}

	/**
	 * @brief ClientClosed
	 * Called when a client has closed its connection
	 * @param socket
	 */
	virtual void ClientClosed(const tcp::Conn &conn) {}

	/**
	 * @brief ServerTerminated
	 * Called when the server is about to shutdown (mainly because of an internal problem)
	 */
	virtual void ServerTerminated(tcp::TcpServer::IEvent::CloseType type) {}
};


static TcpEventHandler handler;
static tcp::TcpServer server(handler);

extern "C" void __stdcall  init_server(void)
{
	tcp::TcpSocket::Initialize();
	server.Start(10, true, 64000);
}

extern "C" void __stdcall  stop_server(void)
{
	server.Stop();

	for (std::uint32_t i = 0U; i < gIxxat.GetNumberOfDevices(); i++)
	{
		gIxxat.Stop(gIxxat.GetNameAt(i));
	}
	
}


extern "C" int __stdcall ixxat_read(const char * name, unsigned long *id, char * data)
{
	int retCode = -1;
	
	CanMessage msg;
	std::string devName(name);

	if (gIxxat.Read(devName, msg))
	{
		*id = msg.id;
		for (int i = 0; i < 8; i++)
		{
			if (i < msg.payloadLen)
			{
				data[i] = msg.bytes[i];
			}
			else
			{
				data[i] = 0;
			}
		}

		retCode = msg.payloadLen;
	}

	return retCode;
}


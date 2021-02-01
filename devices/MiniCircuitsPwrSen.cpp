#include "MiniCircuitsPwrSen.h"

#include <iostream>
#include <string>

#include "serial.h"
#include "Util.h"
#include "Log.h"
#include "HttpProtocol.h"

MiniCircuitsPwrSen::MiniCircuitsPwrSen() noexcept
    : DeviceBase ("MiniCircuitsPwrSen")
{
    mHandle = nullptr;
    mInitialized = false;
}

MiniCircuitsPwrSen::~MiniCircuitsPwrSen()
{

}

bool MiniCircuitsPwrSen::Execute(const std::vector<Value> &args, Value &ret)
{
    bool retCode = true;
    (void) ret;

    if (args.size() > 0)
    {
        // First argument is the command
        if (args[0].GetType() == Value::STRING)
        {
            std::string cmd = args[0].GetString();

            if ((cmd == std::string("INIT")) && (args.size() >= 2))
            {
                Initialize();
            }
            else if (cmd == std::string("GET"))
            {
                if (args.size() > 1)
                {
                    ret = SendCommand(args[1].GetString(), false);
                }
                else
                {
                    SetError("Not enough arguments");
                }
            }
            else if (cmd == std::string("SET"))
            {
                if (args.size() > 1)
                {
                    ret = SendCommand(args[1].GetString(), true);
                }
                else
                {
                    SetError("Not enough arguments");
                }
            }
        }
    }

    return retCode;
}

void  MiniCircuitsPwrSen::AutoTest()
{

}

std::string  MiniCircuitsPwrSen::SendCommand(const std::string &command, bool post)
{
    HttpReply reply;
    mClient.Initialize();

    if (mClient.Connect(GetConnectionChannel(), 80))
    {
        HttpRequest req;
        req.method = post ? "POST" : "GET";
        req.query ="/" + command;
        req.protocol = "HTTP/1.1";

        req.headers["Host"] = GetConnectionChannel();

        std::string output;
        mClient.Send(HttpProtocol::GenerateRequest(req));
        if (mClient.RecvWithTimeout(output, 1024, 1000))
        {
            HttpProtocol::ParseReplyHeader(output, reply);
        }
        else
        {
            SetError("Cannot detect PWR-SEN-6RMS-RC device");
        }
    }
    else
    {
        SetError("Cannot connect to PWR-SEN-6RMS-RC device");
    }

    mClient.Close();

    return reply.body;
}
/*
void boardcast_msg(char *mess){
    int sock;
   struct sockaddr_in broadcastAddr;
   const char *broadcastIP;
   unsigned short broadcastPort;
   char *sendString;
   int broadcastPermission;
   int sendStringLen;

    broadcastIP = "255.255.255.255";
   broadcastPort = 4950;

    sendString = mess;           //string to broadcast


    if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0){
        fprintf(stderr, "socket error");
        exit(1);
    }


    broadcastPermission = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (void *) &broadcastPermission,sizeof(broadcastPermission)) < 0){
        fprintf(stderr, "setsockopt error");
        exit(1);
    }

    // Construct local address structure
    memset(&broadcastAddr, 0, sizeof(broadcastAddr));
   broadcastAddr.sin_family = AF_INET;
   broadcastAddr.sin_addr.s_addr = inet_addr(broadcastIP);
    broadcastAddr.sin_port = htons(broadcastPort);

    sendStringLen = strlen(sendString);

    // Broadcast sendString in datagram to clients
    if (sendto(sock, sendString, sendStringLen, 0, (struct sockaddr *)&broadcastAddr, sizeof(broadcastAddr)) != sendStringLen){
        fprintf(stderr, "sendto error");
        exit(1);
    }

}
*/

void MiniCircuitsPwrSen::Initialize()
{
/*
    boardcast_msg("coucou");

    UdpSocket udp;
    UdpPeer peer;

    peer.SetAddress("255.255.255.255", 4950);

    udp.CreateClient();
    udp.SetBroadcast();

    std::string discoverCmd = "MCL_POWERSENSOR?";
    int retSend = udp.SendTo(peer, reinterpret_cast<const uint8_t *>(discoverCmd.data()), discoverCmd.size());

    if (udp.WaitForData(peer) > 0)
    {
        std::cout << peer.data << std::endl;
    }
*/

    std::string reply = SendCommand(":MN?", false);
    SetInfo("Model name: " + reply);

    if (reply == "MN=PWR-SEN-6RMS-RC")
    {
        mInitialized = true;
    }
    else
    {
        mInitialized = false;
    }





/*
    // Initialize the hidapi library
    res = hid_init();

    // Open the device using the VID, PID,
    // and optionally the Serial number.
    mHandle = hid_open(0x20ce, 0x11, NULL);

    if (mHandle != 0)
    {
        mInitialized = true;
    }
    else
    {
        SetError("Cannot open USB HID device");
        mInitialized = false;
        mHandle = nullptr;
    }
*/
/*
        // Read Indexed String 1
        res = hid_get_indexed_string(handle, 1, wstr, MAX_STR);
        wprintf(L"Indexed String 1: %s\n", wstr);

        // Toggle LED (cmd 0x80). The first byte is the report number (0x0).
        buf[0] = 0x0;
        buf[1] = 0x80;
        res = hid_write(handle, buf, 65);

        // Request state (cmd 0x81). The first byte is the report number (0x0).
        buf[0] = 0x0;
        buf[1] = 0x81;
        res = hid_write(handle, buf, 65);

        // Read requested state
        res = hid_read(handle, buf, 65);

        // Print out the returned buffer.
        for (i = 0; i < 4; i++)
            printf("buf[%d]: %d\n", i, buf[i]);
*/

}


void MiniCircuitsPwrSen::Stop()
{
    if (mInitialized && (mHandle != nullptr))
    {
        // Close the device
        hid_close(mHandle);

        // Finalize the hidapi library
        (void)hid_exit();
    }
    mInitialized = false;
}




#include "ManoLabServer.h"

#include <iostream>
#include <fstream>
#include <string>
#include <JsonReader.h>
#include "Util.h"
#include "Log.h"

ManoLabServer::ManoLabServer() noexcept
    : DeviceBase ("ProdServer")
    , mHost("127.0.0.1")
    , mPort(3000U)
    , mConn(*this)
{

}

ManoLabServer::~ManoLabServer()
{

}

bool ManoLabServer::Execute(const std::vector<Value> &args, Value &ret)
{
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
            else if (cmd == std::string("GET_SERIAL"))
            {
                mConn.close();
                if (mConn.connect(mHost.c_str(), mPort))
                {
                    mData.clear();
                    std::string url = "/api/deviceadd/" + mProduct;
                    std::map<std::string, std::string> headers;
                    mConn.request("POST", url.c_str(), headers); // request is blocking
                    if (mData.size() > 0)
                    {
                        ret = Value(mData);
                        ret.SetJsonString(true);
                    }
                    else
                    {
                        SetError("No data received, timeout");
                    }
                }
                else
                {
                    SetError("Cannot connect to Manufacturing Server");
                }
            }
            else if (cmd == std::string("SET_TESTS"))
            {
                if (args.size() >= 2)
                {
                    std::ifstream inFile;
                    inFile.open(mReportFile);

                    std::stringstream strStream;
                    strStream << inFile.rdbuf();
                    std::string body = strStream.str();

                    mConn.close();
                    if (mConn.connect(mHost.c_str(), mPort))
                    {
                        mData.clear();
                        std::map<std::string, std::string> headers;
                        headers["Content-Type"] = "text/plain";

                        std::string url = "/api/device/" + args[1].GetString();
                        std::cout << "Sending report to: " << url << std::endl;
                        mConn.request("PUT", url.c_str(), headers, body.c_str(), static_cast<int>(body.size()));

                        if (mData.size() > 0)
                        {
                            ret = Value(mData);
                            ret.SetJsonString(true);
                        }
                        else
                        {
                            SetError("No data received, timeout");
                        }
                    }
                    else
                    {
                        SetError("Cannot connect to Manufacturing Server");
                    }
                }
                else
                {
                    SetError("Too few arguments, need device ID");
                }
            }
        }
    }

    return !HasError();
}

/*
#include "asio.hpp"


enum { max_length = 1024 };
*/

//#include "civetweb.h"

void  ManoLabServer::AutoTest()
{
    /*
    try
    {
        char buffer[max_length];

      asio::io_context io_context;

      asio::ip::tcp::socket s(io_context);
      asio::ip::tcp::resolver resolver(io_context);
      asio::connect(s, resolver.resolve(mHost, std::to_string(mPort)));


      // Use an asynchronous operation so that it can be cancelled on timeout.
      std::future<std::size_t> read_result = asio::async_read(
          socket, buffer, asio::use_future);

      // If timeout occurs, then cancel the operation.
      if (read_result.wait_for(std::chrono::seconds(1)) ==
          std::future_status::timeout)
      {
        socket.cancel();
      }
      // Otherwise, the operation completed (with success or error).
      else
      {
        // If the operation failed, then on_read.get() will throw a
        // boost::system::system_error.
        auto bytes_transferred = read_result.get();
        // process buffer
      }

      std::cout << "Enter message: ";
      char request[max_length];
      std::cin.getline(request, max_length);
      size_t request_length = std::strlen(request);

      asio::write(s, asio::buffer(request, request_length));

      char reply[max_length];
      size_t reply_length = asio::read(s, asio::buffer(reply, request_length));
      std::cout << "Reply is: ";
      std::cout.write(reply, reply_length);
      std::cout << "\n";
    }
    catch (std::exception& e)
    {
      std::cerr << "Exception: " << e.what() << "\n";
    }
    */
/*
    struct mg_connection *conn;
    char ebuf[1024] = {0};
    int is_ssl = 0;
    mg_init_library(MG_FEATURES_DEFAULT);
    std::string host = mHost + ":" + std::to_string(mPort);
    conn = mg_connect_client(mHost.c_str(), (int)mPort, is_ssl, ebuf, sizeof(ebuf));
    if (conn) {
        // Connecting to server worked
        char buf[1024] = {0};
        int ret;

        fprintf(stdout, "Connected to %s\n", host.c_str());

        // Send GET request
        mg_printf(conn,
                  "GET /%s HTTP/1.1\r\n"
                  "Host: %s\r\n"
                  "Connection: close\r\n"
                  "\r\n",
                  "api/about",
                  host.c_str());

        // Wait for server to respond with a HTTP header
        ret = mg_get_response(conn, ebuf, sizeof(ebuf), 10000);

        if (ret >= 0) {
            const struct mg_response_info *ri = mg_get_response_info(conn);

            fprintf(stdout,
                    "Response info: %i %s\n",
                    ri->status_code,
                    ri->status_text);

            // Respond reader read. Read body (if any)
            ret = mg_read(conn, buf, sizeof(buf));
            while (ret > 0) {
                fwrite(buf, 1, (unsigned)ret, stdout);
                ret = mg_read(conn, buf, sizeof(buf));
            }

            fprintf(stdout, "Closing connection to %s\n", host.c_str());

        } else {
            // Server did not reply to HTTP request
            fprintf(stderr, "Got no response from %s:\n%s\n", host.c_str(), ebuf);
        }
        mg_close_connection(conn);
    }
*/
    tcp::TcpSocket::Initialize();
    mConn.close();
    if (mConn.connect(mHost.c_str(), mPort))
    {
        mData.clear();
        std::string url = "/api/about";
        std::map<std::string, std::string> headers;


        mConn.request("GET", url.c_str(), headers); // request is blocking
        if (mData.size() > 0)
        {
            JsonReader reader;
            JsonValue value;
            if (reader.ParseString(value, mData))
            {
                std::string status, version, message;
                if ((value.GetValue("status", status)) &&
                    (value.GetValue("version", version)) &&
                    (value.GetValue("message", message)))
                {
                    std::cout << "Manolab server: " << status << " " << version << " " << message << std::endl;
                }
                else
                {
                    SetError("Bad information structure from manufacturing server");
                }
            }
            else
            {
                SetError("Bad manufacturng server reply, expected JSON info");
            }
        }
        else
        {
            SetError("Cannot send request to manufacturing server");
        }
    }
    else
    {
        SetError("Cannot connect to manufacturing server");
    }

    mConn.close();

}

void ManoLabServer::SetProduct(const std::string &product)
{
    mProduct = product;
}

void ManoLabServer::SetReportFile(const std::string &report)
{
    mReportFile = report;
}

void ManoLabServer::ResponseBegin(const http::Response *r)
{
    (void) r;
}

void ManoLabServer::ResponseData(const http::Response *r, const char *data, int numbytes)
{
    (void) r;
    (void) numbytes;
    std::cout << data << std::endl;
    mData.clear();
    mData.assign(data);
}

void ManoLabServer::ResponseComplete(const http::Response *r)
{
     (void) r;
}


void ManoLabServer::Initialize()
{
    tcp::TcpSocket::Initialize();
    std::vector<std::string> opt = Util::Split(GetDeviceOptions(), ":");

    if (opt.size() == 2)
    {
        mHost = opt[0];
        mPort = Util::FromString<std::uint16_t>(opt[1]);
    }
    else
    {
        TLogWarning("No manufacturing server specified, use default localhot:3000");
    }
}

void ManoLabServer::Stop()
{
    mConn.close();
}




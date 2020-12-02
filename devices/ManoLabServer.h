#ifndef MANOLAB_SERVER_H
#define MANOLAB_SERVER_H


#include <vector>
#include <map>
#include "DeviceBase.h"
#include "Http.h"

class ManoLabServer : public DeviceBase, public http::IEvent
{
public:

    ManoLabServer() noexcept;
    ~ManoLabServer();
    bool Execute(const std::vector<Value> &args, Value &ret);

    void Initialize();
    void Stop();
    void AutoTest();

    void SetProduct(const std::string &product);
    void SetReportFile(const std::string &report);

    // From http::IEvent
    void ResponseBegin( const http::Response* r);
    void ResponseData( const http::Response* r, const char* data, int numbytes );
    void ResponseComplete( const http::Response* r);

private:
    std::string mHost;
    std::string mData;
    std::uint16_t mPort;
    http::Connection mConn;
    std::string mProduct;
    std::string mReportFile;
};


#endif // MANOLAB_SERVER_H

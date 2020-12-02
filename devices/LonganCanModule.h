#ifndef LONGAN_CAN_MODULE_H
#define LONGAN_CAN_MODULE_H
// ID3 ID2 ID1 ID0 EXT RTR DTA0 DTA1 DTA2 DTA3 DTA4 DTA5 DTA6 DTA7

#include "SerialPort.h"
#include "DeviceBase.h"

#define uchar unsigned char


#define CAN_RATE_5      1
#define CAN_RATE_10     2
#define CAN_RATE_20     3
#define CAN_RATE_25     4
#define CAN_RATE_31_2   5
#define CAN_RATE_33     6
#define CAN_RATE_40     7
#define CAN_RATE_50     8
#define CAN_RATE_80     9
#define CAN_RATE_83_3   10
#define CAN_RATE_95     11
#define CAN_RATE_100    12
#define CAN_RATE_125    13
#define CAN_RATE_200    14
#define CAN_RATE_250    15
#define CAN_RATE_500    16
#define CAN_RATE_666    17
#define CAN_RATE_1000   18

class LonganCanModule : public DeviceBase
{
public:
    LonganCanModule();

    bool Execute(const std::vector<Value> &args, Value &ret);

    void send(unsigned long id, uchar ext, uchar rtrBit, uchar len, const uchar *buf);
    int recv(unsigned long *id, uchar *buf);
    
    unsigned char canRate(unsigned char rate);
    unsigned char baudRate(unsigned char rate);
    
    unsigned char setMask(unsigned long *dta);
    unsigned char setFilt(unsigned long *dta);
    
    unsigned char factorySetting();
    void debugMode();

    void AutoTest();
    void Initialize();
    void Stop();

private:
    unsigned char cmdOk(const char *cmd);
    unsigned char enterSettingMode();
    unsigned char exitSettingMode();
    void clear();
    char str_tmp[100];

    SerialPort mPort;
};

#endif // LONGAN_CAN_MODULE_H

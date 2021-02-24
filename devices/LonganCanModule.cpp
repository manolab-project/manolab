// ID3 ID2 ID1 ID0 EXT RTR DTA0 DTA1 DTA2 DTA3 DTA4 DTA5 DTA6 DTA7

#include <chrono>
#include <cstring>
#include <iostream>
#include "LonganCanModule.h"
#include "Util.h"
#include "Log.h"

unsigned long millis()
{
    auto time = std::chrono::system_clock::now(); // get the current time

    auto since_epoch = time.time_since_epoch(); // get the duration since epoch

    // I don't know what system_clock returns
    // I think it's uint64_t nanoseconds since epoch
    // Either way this duration_cast will do the right thing
    auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(since_epoch);

    unsigned long now = millis.count();

    return now;
}

LonganCanModule::LonganCanModule()
    : DeviceBase("LonganCanModule")
{

}

bool LonganCanModule::Execute(const std::vector<Value> &args, Value &ret)
{
    (void) ret;

    if (args.size() > 0)
    {
        // First argument is the command
        if (args[0].GetType() == Value::STRING)
        {
            std::string cmd = args[0].GetString();

            if (cmd == std::string("SEND"))
            {
                unsigned char dta[8] = {1, 2, 3, 4, 5, 6, 7, 8};

                // send(unsigned long id, byte ext, byte rtrBit, byte len, const byte *buf);

                send(0x55, 0, 0, 8, dta);   // SEND TO ID:0X55

            }
            else if (cmd == std::string("RECV"))
            {
                unsigned long id;
                uchar buf[20];

                int size = recv(&id, &buf[0]);

                if (size > 0)
                {
                    std::cout << "RCV: ID=" << id << " DATA: ";
                    for (int i = 0; i < size; i++)
                    {
                        std::cout << std::hex << (int)buf[i] << " ,";
                    }
                    std::cout << std::endl;
                }
            }
        }
    }

    return !HasError();
}

void LonganCanModule::send(unsigned long id, uchar ext, uchar rtrBit, uchar len, const uchar *buf)
{
    unsigned char dta[14] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    
    dta[0] = id>>24;        // id3
    dta[1] = id>>16&0xff;   // id2
    dta[2] = id>>8&0xff;    // id1
    dta[3] = id&0xff;       // id0
    
    dta[4] = ext;
    dta[5] = rtrBit;
    
    for(int i=0; i<len; i++)
    {
        dta[6+i] = buf[i];
    }
    
    mPort.Write(dta, 14);

//    for(int i=0; i<14; i++)
//    {
//        canSerial->write(dta[i]);
//    }
}


// 0: no data
// 1: get data
int LonganCanModule::recv(unsigned long *id, uchar *buf)
{
    std::string readData;
  //  if(canSerial->available())
    if (mPort.Read(readData, 1) == SerialPort::cPortReadSuccess)
    {
//        unsigned long timer_s = millis();
        
//        int len = 0;
//        uchar dta[20];
        
//        while(1)
        {
//            while(canSerial->available())
//            {
//                dta[len++] = canSerial->read();
//                if(len == 12)
//                    break;
//                timer_s = millis();
//            	if((millis()-timer_s) > 10)
//                    return 0; // Reading 12 bytes should be faster than 10ms, abort if it takes longer, we loose the partial message in this case
//            }

            
            //if(len == 12) // Just to be sure, must be 12 here
            if (readData.size() == 12)
            {
                unsigned long __id = 0;
                
                for(int i=0; i<4; i++) // Store the id of the sender
                {
                    __id <<= 8;
                    __id += readData[i];
                }
                
                *id = __id;
                
                for(int i=0; i<8; i++) // Store the message in the buffer
                {
                    buf[i] = readData[i+4];
                }
                return readData.size() - 4;
            }
        }
    }
    
    return 0;
}

unsigned char LonganCanModule::cmdOk(const char *cmd)
{
    
//    unsigned long timer_s = millis();
    unsigned char len = 0;

  //  canSerial->println(cmd);
    if (mPort.Write(reinterpret_cast<const std::uint8_t *>(cmd), std::strlen(cmd)) == SerialPort::cPortWriteSuccess)
//    while(1)
    {
//        if(millis()-timer_s > 500)
//        {
//            return 0;
//        }
        
    //    while(canSerial->available())
//        {

//            str_tmp[len++] = canSerial->read();
//            timer_s = millis();
//        }
        std::string readData;
        if (mPort.Read(readData, 1) == SerialPort::cPortReadSuccess)
        {



            if(len >= 4 && str_tmp[len-1] == '\n' && str_tmp[len-2] == '\r' && str_tmp[len-3] == 'K' && str_tmp[len-4] == 'O')
            {
                clear();
                return 1;
            }
        }

    }
    return 0;
}

/*
value	    01	02	03	04	05	    06	07	08	09	10	    11	12	13	14	15	16	17	18
rate(kb/s)	5	10	20	25	31.2	33	40	50	80	83.3	95	100	125	200	250	500	666	1000
*/
unsigned char LonganCanModule::canRate(unsigned char rate)
{
    enterSettingMode();
    if(rate < 10)
        sprintf(str_tmp, "AT+C=0%d\r\n", rate);
    else 
        sprintf(str_tmp, "AT+C=%d\r\n", rate);
    
    int ret = cmdOk(str_tmp);
    
    exitSettingMode();
    return ret;
}

/*
value	        0	    1	    2	    3   	4
baud rate(b/s)	9600	19200	38400	57600	115200
*/
unsigned char LonganCanModule::baudRate(unsigned char rate)
{
    enterSettingMode();
    
    sprintf(str_tmp, "AT+S=%d\r\n", rate);
    int ret = cmdOk(str_tmp);
    exitSettingMode();
    return ret;
}


void LonganCanModule::clear()
{
//    unsigned long timer_s = millis();
//    while(1)
//    {
//        if(millis()-timer_s > 50)return;
//        while(canSerial->available())
//        {
//            canSerial->read();
//            timer_s = millis();
//        }
//    }

    std::string readData;
    (void) mPort.Read(readData, 1);
}

unsigned char LonganCanModule::enterSettingMode()
{
//    canSerial->print("+++");

    mPort.Write(std::string("+++"));
    clear();
    return 1;
}

unsigned char LonganCanModule::exitSettingMode()
{
    clear();
    unsigned char ret = cmdOk("AT+Q\r\n");
    clear();
    return ret;
}

void make8zerochar(int n, char *str, unsigned long num)
{
    for(int i=0; i<n; i++)
    {
        str[n-1-i] = num%0x10;
        if(str[n-1-i]<10)str[n-1-i]+='0';
        else str[n-1-i] = str[n-1-i]-10+'A';
        num >>= 4;
    }
    str[n] = '\0';    
}

/*
+++	                    Switch from Normal mode to Config mode
AT+S=[value]	        Set serial baud rate
AT+C=[value]	        Set CAN Bus baud rate
AT+M=[N][EXT][value]    Set mask,AT+M=[1][0][000003DF]
AT+F=[N][EXT][value]    Set filter,AT+F=[1][0][000003DF]
AT+Q	            Switch to Normal Mode
*/
unsigned char LonganCanModule::setMask(unsigned long *dta)
{
    enterSettingMode();
    char __str[10];
    
    
    for(int i=0; i<2; i++)
    {
        make8zerochar(8, __str, dta[1+2*i]);
        //Serial.println(__str);
        sprintf(str_tmp, "AT+M=[%d][%d][", i, (int)dta[2*i]);
        for(int i=0; i<8; i++)
        {
            str_tmp[12+i] = __str[i];
        }
        str_tmp[20] = ']';
        str_tmp[21] = '\r';
        str_tmp[22] = '\n';
        str_tmp[23] = '\0';
        
        //Serial.println(str_tmp);
        
        if(!cmdOk(str_tmp))
        {
           // Serial.print("mask fail - ");
           // Serial.println(i);
            exitSettingMode();
            return 0;
        }
        clear();
        //
    }
    exitSettingMode();
    return 1;

}

unsigned char LonganCanModule::setFilt(unsigned long *dta)
{
    enterSettingMode();
    
    char __str[10];
    
    for(int i=0; i<6; i++)
    {
        make8zerochar(8, __str, dta[1+2*i]);
        //Serial.println(__str);
        sprintf(str_tmp, "AT+F=[%d][%d][", i, (int)dta[2*i]);
        for(int i=0; i<8; i++)
        {
            str_tmp[12+i] = __str[i];
        }
        str_tmp[20] = ']';
        str_tmp[21] = '\r';
        str_tmp[22] = '\n';
        str_tmp[23] = '\0';
        
        //Serial.println(str_tmp);
        
        clear();
        if(!cmdOk(str_tmp))
        {
            //Serial.print("filt fail at - ");
            //Serial.println(i);
            exitSettingMode();
            return 0;
        }
        clear();
        //
    }
    exitSettingMode();
    return 1;
}

/*
value	        0	    1	    2	    3   	4
baud rate(b/s)	9600	19200	38400	57600	115200
*/
unsigned char LonganCanModule::factorySetting()
{
    // check baudrate
//    unsigned long baud[5] = {9600, 19200, 38400, 57600, 115200};
    
    for(int i=0; i<5; i++)
    {

//        canSerial->begin(baud[i]);
//        canSerial->print("+++");
//        delay(100);
        
        if(cmdOk("AT\r\n"))
        {
//            Serial.print("SERIAL BAUD RATE IS: ");
//            Serial.println(baud[i]);
            baudRate(0);                // set serial baudrate to 9600
/*            Serial.println("SET SERIAL BAUD RATE TO: 9600 OK");
            canSerial->begin(9600)*/;
            break;            
        }
    }
    
    if(canRate(CAN_RATE_500))
    {
//        Serial.println("SET CAN BUS BAUD RATE TO 500Kb/s OK");
    }
    else
    {
//        Serial.println("SET CAN BUS BAUD RATE TO 500Kb/s FAIL");
        return 0;
    }
    
    unsigned long mask[4] = {0, 0, 0, 0,};
    unsigned long filt[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,};
    
    if(setFilt(filt))
    {
//        Serial.println("FACTORY SETTING FILTS OK");
    }
    else 
    {
//        Serial.println("FACTORY SETTING FILTS FAIL");
        return 0;        
    }
    
    if(setMask(mask))
    {
//        Serial.println("FACTORY SETTING MASKS OK");
    }
    else
    {
//        Serial.println("FACTORY SETTING MASKS FAIL");
        return 0;
    }
    
    return 1;
}

void LonganCanModule::debugMode()
{
//    while(Serial.available())
//    {
//        canSerial->write(Serial.read());
//    }
    
//    while(canSerial->available())
//    {
//        Serial.write(canSerial->read());
//    }
}

void LonganCanModule::AutoTest()
{

}

void LonganCanModule::Initialize()
{
    if (mPort.Open(GetConnectionChannel(), GetConnectionSettings()) == SerialPort::cPortAssociated)
    {
        TLogInfo(mPort.GetLastSuccess());
    }
    else
    {
        SetError(mPort.GetLastError());
    }
}

void LonganCanModule::Stop()
{
    mPort.Close();
}

// END FILE

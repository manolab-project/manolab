#ifndef ZEBRA7500_H
#define ZEBRA7500_H

#include "zebrafx7500_global.h"
#include "IPlugin.h"

class Zebra7500 : public mano::IPlugin
{
public:
    Zebra7500();
    virtual ~Zebra7500();

    bool Initialize();
    void Stop();

    bool onCommand(const char* node, const char* data, unsigned int size);
            // Handles a command from the application.
};

#endif // ZEBRA7500_H

#include "zebra7500.h"
#include <iostream>
#include "Plugin.h"

SCY_PLUGIN(Zebra7500, "ZebraFX7500 Plugin", "0.1.1")

Zebra7500::Zebra7500()
{

}

Zebra7500::~Zebra7500()
{

}

bool Zebra7500::Initialize()
{

}

void Zebra7500::Stop()
{

}

bool Zebra7500::onCommand(const char* node, const char* data, unsigned int size)
{
    std::cout << "TestPlugin: Command: " << node << ": " << data << std::endl;
    // Process commands as required
    return true;
}

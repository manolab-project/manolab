#ifndef IPLUGIN_H
#define IPLUGIN_H

namespace mano {

class IPlugin
    // Virtual plugin interface.
{
public:
    IPlugin() {};
    virtual ~IPlugin() {};

    virtual bool Initialize() = 0;
    virtual void Stop() = 0;
    virtual bool onCommand(const char* node, const char* data, unsigned int size) = 0;
        // Handles a command from the application.
};


}  // namespace mano


#endif // IPLUGIN_H

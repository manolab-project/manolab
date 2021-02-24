#ifndef PLUGIN_H
#define PLUGIN_H

#include "IPlugin.h"

namespace mano {

// Forward declare the plugin class which must be defined externally.

// Define the API version.
// This value is incremented whenever there are ABI breaking changes.
#define MANOLAB_PLUGIN_API_VERSION 1

// Shared library exports
#if defined(SCY_WIN) && defined(SCY_SHARED_LIBRARY)
    #if defined(Base_EXPORTS)
        #define Base_API __declspec(dllexport)
    #else
        #define Base_API __declspec(dllimport)
    #endif
#else
    #define Base_API // nothing
#endif

#ifdef WIN32
# define MANO_PLUGIN_EXPORT __declspec(dllexport)
#else
# define MANO_PLUGIN_EXPORT // empty
#endif

// Define a type for the static function pointer.
typedef IPlugin* (*GetInterfaceFunc)();

// Plugin details structure that's exposed to the application.
struct PluginDetails {
    int apiVersion;
    const char* fileName;
    const char* className;
    const char* pluginName;
    const char* pluginVersion;
    GetInterfaceFunc GetInterface;
};

#define MANO_PLUGIN_INFO_DATA \
    MANOLAB_PLUGIN_API_VERSION,       \
    __FILE__

#define MANOLAB_PLUGIN(classType, pluginName, pluginVersion)     \
  extern "C" {                                               \
      MANO_PLUGIN_EXPORT mano::IPlugin* GetInterface()     \
      {                                                      \
          static classType singleton;                        \
          return &singleton;                                 \
      }                                                      \
      MANO_PLUGIN_EXPORT mano::PluginDetails exports =  \
      {                                                      \
          MANO_PLUGIN_INFO_DATA,                         \
          #classType,                                        \
          pluginName,                                        \
          pluginVersion,                                     \
          GetInterface,                                         \
      };                                                     \
  }


class PluginBase : public IPlugin
{
public :
    PluginBase(const std::string &name)
        : mName(name)
        , mHasError(false)
    {

    }

    // Identification
    std::string GetName()
    {
        return mName;
    }

    void SetDeviceId(const std::string &id)
    {
        mId = id;
    }

    std::string GetDeviceId()
    {
        return mId;
    }

    // Connection parameters
    void SetConnectionChannel(const std::string &conn)
    {
        mConnChannel = conn;
    }

    std::string GetConnectionChannel()
    {
        return mConnChannel;
    }

    void SetConnectionSettings(const std::string &params)
    {
        mConnSettings = params;
    }

    std::string GetConnectionSettings()
    {
        return mConnSettings;
    }

    void SetDeviceOptions(const std::string &options)
    {
        mDeviceOptions = options;
    }

    std::string GetDeviceOptions()
    {
        return mDeviceOptions;
    }


    // Error management
    void SetError(const std::string &err)
    {
        mError = GetName() + ": " + err;
        mHasError = true;
    }

    std::string GetError() const
    {
        return mError;
    }

    void ClearError()
    {
        mHasError = false;
        mError.clear();
    }

    bool HasError() const
    {
        return mHasError;
    }

private:
    std::string mName;
    std::string mConnSettings;
    std::string mId;
    std::string mConnChannel;
    std::string mDeviceOptions;
    bool mHasError;
    std::string mError;
};

}  // namespace mano


#endif // PLUGIN_H

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

}  // namespace mano


#endif // PLUGIN_H

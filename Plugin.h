#ifndef PLUGIN_H
#define PLUGIN_H

namespace mano {

// Forward declare the plugin class which must be defined externally.
class IPlugin;

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
# define SCY_PLUGIN_EXPORT __declspec(dllexport)
#else
# define SCY_PLUGIN_EXPORT // empty
#endif

// Define a type for the static function pointer.
typedef IPlugin* (*InitFunc)();

// Plugin details structure that's exposed to the application.
struct PluginDetails {
    int apiVersion;
    const char* fileName;
    const char* className;
    const char* pluginName;
    const char* pluginVersion;
    InitFunc Initialize;
};

#define SCY_STANDARD_PLUGIN_STUFF \
    MANOLAB_PLUGIN_API_VERSION,       \
    __FILE__

#define SCY_PLUGIN(classType, pluginName, pluginVersion)     \
  extern "C" {                                               \
      SCY_PLUGIN_EXPORT mano::IPlugin* Initialize()     \
      {                                                      \
          static classType singleton;                        \
          return &singleton;                                 \
      }                                                      \
      SCY_PLUGIN_EXPORT mano::PluginDetails exports =  \
      {                                                      \
          SCY_STANDARD_PLUGIN_STUFF,                         \
          #classType,                                        \
          pluginName,                                        \
          pluginVersion,                                     \
          Initialize,                                         \
      };                                                     \
  }


}  // namespace mano


#endif // PLUGIN_H

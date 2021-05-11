#ifndef IPLUGIN_H
#define IPLUGIN_H

#include "IScriptEngine.h"


namespace mano {

class IPlugin
{
public:
    IPlugin() {};
    virtual ~IPlugin() {};

    class ICallBack
    {
    public:
//        virtual std::string Callback(const std::string &req) = 0;
        virtual std::string Callback(int category, const char *) = 0;
    };

    /**
     * Commande transférée au plugin
     *   {
     *      "cmd": "SetParameters",
     *      "data":  {
     *      }
     *   }
     *
     *
     * Retourne un JSON qui aura toujours la forme suivante :
     *   {
     *      "cmd": "ReplyTruc",
     *      "success": true | false,
     *      "message": "message d'erreur ou debug",
     *      "data": {
     *          ...
     *      }
     *   }
     */
    virtual std::string Request(const std::string &req) = 0;

    virtual bool Register(ICallBack *cb) = 0;
};


}  // namespace mano

/*
Supported commands:

// Commande d'initialisation du plug-in
// à réception, il est attendu que le plug-in :
//  - s'initialise
//  - S'auto-test
//  - retourner un message d'erreur en cas de problème

// réponse: cmd = "ReplyParameters"
 {
    "cmd": "SetParameters",
    "data": {
        "conn_channel": "10.0.0.14",
        "conn_settings": "",
        "id": "",
        "options": ""
    }
 }


 // Commande en provenance du script
 {
    "cmd": "Execute",
    "data": {
        "args": [ "toto", "tata" ]

    }
 }

 // Réponse :

 {
    "cmd": "ReplyExecute",
    "success": true,
    "message": "",
    "data": {
        "args": [ "toto", "tata" ]

    }
 }




*/



#endif // IPLUGIN_H

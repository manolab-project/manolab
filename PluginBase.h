#ifndef PLUGINBASE_H
#define PLUGINBASE_H

#include "Plugin.h"
#include "JsonReader.h"
#include "JsonValue.h"

namespace mano
{

class PluginBase : public IPlugin
{
public :
    PluginBase(const std::string &name)
    : mName(name)
    {
        
    }
    
    std::string GetName() { return mName; }
    
    static std::string ErrorResponse(const std::string &cmd, const std::string &message) {
        
        return GenericResponse(false, cmd, message);
    }
    
    static std::string GenericResponse(bool success, const std::string &cmd, const std::string &message = "") {
        
        std::string error = R"({
        "cmd": "Reply)" + ((cmd == "") ? "Unkown" : cmd) + R"(",
        "success": )" + (success ? "true" : "false") + R"(,
        "message": ")" + message + R"(",
        "data": null
    })";
    
    return error;
    }

    static Value JsonToValue(const JsonValue &json)
    {
        JsonValue::Tag tag = json.GetTag();
        Value val;
        if (tag == JsonValue::BOOLEAN)
        {
            val = Value(json.GetBool());
        }
        else if (tag == JsonValue::INTEGER)
        {
            val = Value(json.GetInteger());
        }
        else if (tag == JsonValue::DOUBLE)
        {
            val = Value(json.GetDouble());
        }
        else if (tag == JsonValue::STRING)
        {
            val = Value(json.GetString());
        }
        else if (tag == JsonValue::NULL_VAL)
        {
            val.SetNull();
        }
        else if (tag == JsonValue::OBJECT)
        {
            val = Value(json.ToString());
            val.SetJsonString(true);
        }
        return val;
    }

    static JsonValue ValueToJson(const Value &v)
    {
        Value::Type t = v.GetType();
        JsonValue json;

        if (t == Value::BOOLEAN)
        {
            json = JsonValue(v.GetBool());
        }
        else if (t == Value::DOUBLE)
        {
            json = JsonValue(v.GetDouble());
        }
        else if (t == Value::INTEGER)
        {
            json = JsonValue(v.GetInteger());
        }
        else if (t == Value::STRING)
        {
            if (v.IsJsonString())
            {
                JsonReader reader;
                if (!reader.ParseString(json, v.GetString()))
                {
                    json.SetNull();
                }
            }
            else
            {
                json = JsonValue(v.GetString());
            }
        }
        else if (t == Value::NULL_VAL)
        {
            json.SetNull();
        }

        return json;
    }
    
protected:
    std::string mName;
};

}  // namespace mano

#endif // PLUGINBASE_H

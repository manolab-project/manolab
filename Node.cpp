#include "Node.h"

Node::Node()
    : enable(true)
{


}

bool Node::Parse(const JsonValue &entry)
{
    bool success = false;
    if (entry.IsObject())
    {
        JsonValue titleJson = entry.FindValue("title");
        if (titleJson.IsString())
        {
            title = titleJson.GetString();
            JsonValue steps_value = entry.FindValue("steps");
            if (steps_value.IsArray())
            {
                JsonArray &stepsJson = steps_value.GetArray();
                for (std::uint32_t j = 0; j < stepsJson.Size(); j++)
                {
                    JsonValue stepJson = stepsJson.GetEntry(j);
                    if (stepJson.IsObject())
                    {
                        Step s;
                        s.name = stepJson.FindValue("name").GetString();
                        s.mode = stepJson.FindValue("mode").GetString();
                        s.event = stepJson.FindValue("event").GetString();
                        steps.push_back(s);
                    }
                }
                success = true;
            }
        }
    }

    return success;
}

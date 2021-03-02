#ifndef NODE_H
#define NODE_H

#include <vector>
#include <string>
#include "JsonValue.h"

struct Node
{
    std::string title;
    std::string id;
    bool enable;

    struct Step
    {
        std::string id;
        std::string name;
        std::string mode;
        std::string event;
    };

    std::vector<Step> steps;

    Node();

    bool Parse(const JsonValue &entry);
};

#endif // NODE_H

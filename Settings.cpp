#include "Settings.h"
#include "JsonReader.h"
#include "JsonWriter.h"
#include "Util.h"

void Settings::ReadSettings(IProcessEngine &engine)
{
    std::string defaultWorkspace = Util::HomePath() + Util::DIR_SEPARATOR + ".manloab";
    std::string workspace = defaultWorkspace;
    JsonReader confFile;
    JsonValue json;

    if (confFile.ParseFile(json, "manolab.json"))
    {
        workspace = json.FindValue("workspace").GetString();

        JsonArray plugins = json.FindValue("plugins").GetArray();

        std::vector<std::string> pList;
        for (const auto & p : plugins)
        {
            pList.push_back(p.GetString());
        }

        engine.SetPlugins(pList);
    }

    if (!Util::FolderExists(workspace))
    {
        workspace = defaultWorkspace;
        Util::Mkdir(workspace);
    }

    std::cout << "Current workspace is: " << workspace << std::endl;

    engine.SetWorkspace(workspace);
    WriteSettings(engine);
}

void Settings::WriteSettings(const IProcessEngine &engine)
{
    JsonObject json;
    json.AddValue("workspace", engine.GetWorkspace());
    JsonWriter::SaveToFile(json, "manolab.json");
}


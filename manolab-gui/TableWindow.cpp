#include "TableWindow.h"
#include "imgui.h"
#include "JsonReader.h"
#include "Util.h"

TableWindow::TableWindow()
{
    RefreshWindowParameter();
}

void TableWindow::RefreshWindowParameter()
{
    std::string tempWindow = std::to_string(mWindow/1000);
    sprintf(buf2, "%.10s", tempWindow.c_str());
}


void TableWindow::Draw(const char *title, bool *p_open)
{
    // Local copy to shorter mutex locking
    mMutex.lock();
    std::map<int64_t, Entry> table = mTable;
    int64_t startTime = mStartTime;
    mMutex.unlock();

    ImGui::Begin(title, p_open);

    ImGui::Text("Tableau des passages");               // Display some text (you can use a format strings too)

    ImGui::Text("Tags : %d", static_cast<int>(table.size()));

    ImGui::PushItemWidth(200);
    ImGui::InputText("Fenêtre de blocage (en secondes)",  buf2, sizeof(buf2), ImGuiInputTextFlags_CharsDecimal);
    ImGui::PopItemWidth();
    ImGui::SameLine();
    if (ImGui::Button( "Appliquer", ImVec2(100, 40)))
    {
       std::scoped_lock<std::mutex> lock(mMutex);
       mWindow = Util::FromString<int64_t>(buf2) * 1000; // en millisecondes
    }

    ImGuiTableFlags tableFlags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg;

    if (ImGui::BeginTable("table1", 4, tableFlags))
    {
        ImGui::TableSetupColumn("Participant", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("Numéro", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("Tours", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("Temps", ImGuiTableColumnFlags_WidthStretch);

        ImGui::TableHeadersRow();

        for (const auto & e : table)
        {
             ImGui::TableNextRow();

             ImGui::TableSetColumnIndex(1);
             ImGui::Text("%s", std::to_string(e.second.tag).c_str());

             ImGui::TableSetColumnIndex(2);
             ImGui::Text("%s", std::to_string(e.second.laps.size()).c_str());

             ImGui::TableSetColumnIndex(3);

             ImGui::Text("%s", e.second.ToString(startTime).c_str());
        }


        ImGui::EndTable();
    }

    ImGui::End();
}

void TableWindow::ParseAction(const std::vector<Value> &args)
{
    JsonReader reader;
    JsonValue json;

    if (args.size() > 0)
    {
        if (reader.ParseString(json, args[0].GetString()))
        {
            Entry e;
            e.tag = json.FindValue("tag").GetInteger64();
            int64_t time = json.FindValue("time").GetInteger64();

            mMutex.lock();

            if (mTable.size() == 0)
            {
                mStartTime = time;
            }

            if (mTable.count(e.tag) > 0)
            {
                std::vector<int64_t> &l = mTable[e.tag].laps;
                if (l.size() > 0)
                {
                    int64_t diff = time - l[l.size() - 1];

                    if (diff > mWindow)
                    {
                        mTable[e.tag].laps.push_back(time);
                    }
                }

            }
            else
            {
                e.laps.push_back(time);
                mTable[e.tag] = e;
            }

            mMutex.unlock();
        }
    }
}

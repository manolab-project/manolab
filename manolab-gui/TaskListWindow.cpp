#include "TaskListWindow.h"
#include "ImGuiFileDialog.h"
#include "Util.h"

#define IM_MIN(A, B)            (((A) < (B)) ? (A) : (B))
#define IM_MAX(A, B)            (((A) >= (B)) ? (A) : (B))
#define IM_CLAMP(V, MN, MX)     ((V) < (MN) ? (MN) : (V) > (MX) ? (MX) : (V))


TaskListWindow::TaskListWindow(IProcessEngine &engine)
    : mEngine(engine)
{

}

void TaskListWindow::Initialize()
{

}


void TaskListWindow::RefreshList()
{

}

void TaskListWindow::ScanWorkspace()
{
    std::string p = mEngine.GetWorkspace() + "/files";

    files = Util::GetDirectoryFiles(p, "js");
//    for (const auto & f : files)
//    {
//        std::cout << f << std::endl;
//    }
}

void TaskListWindow::Draw(const char *title, bool *p_open)
{
//    const char *columns[] = { "Jean", "57", "2", "19min 54s" };

    static ImGuiComboFlags comboFlags = 0;
    ImGui::Begin(title, p_open);                          // Create a window called "Hello, world!" and append into it.

    static int item_current_idx = 0;                    // Here our selection data is an index.
    const char* combo_label = files.size() > 0 ? files[item_current_idx].c_str() : nullptr;  // Label to preview before opening the combo (technically it could be anything)
    if (ImGui::BeginCombo("##Scripts", combo_label, comboFlags))
    {
        int n = 0;
        for (const auto & f : files)
        {
            const bool is_selected = (item_current_idx == n);
            if (ImGui::Selectable(f.c_str(), is_selected))
            {
                item_current_idx = n;
            }

            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
            if (is_selected)
            {
                ImGui::SetItemDefaultFocus();
            }
            n++;
        }
        ImGui::EndCombo();
    }

    ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);

    if (ImGui::Button( ICON_FA_FOLDER_OPEN " Load", ImVec2(100, 40)))
    {
        if (files.size() > 0)
        {
            std::string fullPath = mEngine.GetWorkspace() + Util::DIR_SEPARATOR + "files"  + Util::DIR_SEPARATOR + files[item_current_idx];
            mEngine.LoadScript(fullPath);
        }
    }

    if (ImGui::Button( ICON_FA_PLAY " Play", ImVec2(100, 40)))
    {
        if (mEngine.IsLoaded())
        {
            mEngine.Start();
        }
    }

    ImGuiTableFlags tableFlags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg;


    /*
    // Animate a simple progress bar
    static float progress = 0.0f, progress_dir = 1.0f;
    progress += progress_dir * 0.4f * ImGui::GetIO().DeltaTime;
    if (progress >= +1.1f) { progress = +1.1f; progress_dir *= -1.0f; }
    if (progress <= -0.1f) { progress = -0.1f; progress_dir *= -1.0f; }

    // Typically we would use ImVec2(-1.0f,0.0f) or ImVec2(-FLT_MIN,0.0f) to use all available width,
    // or ImVec2(width,0.0f) for a specified width. ImVec2(0.0f,0.0f) uses ItemWidth.
    ImGui::ProgressBar(progress, ImVec2(0.0f, 0.0f));
    ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
    ImGui::Text("Current test");

    float progress_saturated = IM_CLAMP(progress, 0.0f, 1.0f);
    char buf[32];
    sprintf(buf, "%d/%d", (int)(progress_saturated * 1753), 1753);
    ImGui::ProgressBar(progress, ImVec2(0.f, 0.f), buf);
    ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
    ImGui::Text("62%% Total");

    ImGui::LoadingIndicatorCircle("##ref_loading_circle", 20.0f,
                                                       ImGui::GetStyleColorVec4(ImGuiCol_ButtonHovered),
                                                       ImGui::GetStyleColorVec4(ImGuiCol_ButtonHovered),
                               10, 10.0);

    */

    if (ImGui::BeginTable("TestListTable", 2, tableFlags))
    {

        ImGui::TableSetupColumn("N°", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("Titre", ImGuiTableColumnFlags_WidthStretch);
//        ImGui::TableSetupColumn("Tour", ImGuiTableColumnFlags_WidthStretch);
//        ImGui::TableSetupColumn("Temps", ImGuiTableColumnFlags_WidthStretch);

        std::vector<Node> tests = mEngine.GetTests();

        ImGui::TableHeadersRow();
        int row = 1;
        for (const auto &t : tests)
        {
             ImGui::TableNextRow();
             ImGui::TableSetColumnIndex(0);
             ImGui::Text("%d", row);
             ImGui::TableSetColumnIndex(1);
             ImGui::Text("%s", t.title.c_str());
             row++;
        }

        ImGui::EndTable();
    }



    //    ImGui::SameLine();
    //    ImGui::Text("counter = %d", counter);

    ImGui::End();
}

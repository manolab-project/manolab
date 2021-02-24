#include "TaskListWindow.h"
#include "ImGuiFileDialog.h"

#define IM_MIN(A, B)            (((A) < (B)) ? (A) : (B))
#define IM_MAX(A, B)            (((A) >= (B)) ? (A) : (B))
#define IM_CLAMP(V, MN, MX)     ((V) < (MN) ? (MN) : (V) > (MX) ? (MX) : (V))


TaskListWindow::TaskListWindow()
{

}

void TaskListWindow::Initialize()
{

}

void TaskListWindow::Draw(const char *title, bool *p_open)
{
    const char *columns[] = { "Jean", "57", "2", "19min 54s" };
    ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg;
    static int counter = 0;

    ImGui::Begin(title, p_open);                          // Create a window called "Hello, world!" and append into it.

    // in an imgui window somewhere...
    ImGui::Text( ICON_FA_PAINT_BRUSH "  Paint" );    // use string literal concatenation

    ImGui::Text("Tableau des passages");               // Display some text (you can use a format strings too)

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

    if (ImGui::BeginTable("table1", 4, flags))
    {

        ImGui::TableSetupColumn("Participant", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("Numéro", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("Tour", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("Temps", ImGuiTableColumnFlags_WidthStretch);

        ImGui::TableHeadersRow();
        for (int row = 0; row < 5; row++)
        {
             ImGui::TableNextRow();
             for (int column = 0; column < 4; column++)
             {
                 ImGui::TableSetColumnIndex(column);
                 ImGui::Text("%s", columns[column]);
             }
        }

        ImGui::EndTable();
    }


    if (ImGui::Button("Button"))
    {
        counter++;
    }

    ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "Platform");
    ImGui::Text("%s", SDL_GetPlatform());
    ImGui::Text("CPU cores: %d", SDL_GetCPUCount());
    ImGui::Text("RAM: %.2f GB", SDL_GetSystemRAM() / 1024.0f);
    //    ImGui::SameLine();
    //    ImGui::Text("counter = %d", counter);

    //    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);


    ImGui::End();
}

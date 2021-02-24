// Dear ImGui: standalone example application for SDL2 + OpenGL
// (SDL is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation, etc.)
// (GL3W is a helper library to access OpenGL functions since there is no standard header to access modern OpenGL functions easily. Alternatives are GLEW, Glad, etc.)
// If you are new to Dear ImGui, read documentation from the docs/ folder + read the top of imgui.cpp.
// Read online: https://github.com/ocornut/imgui/tree/master/docs

#include "Gui.h"
#include "ConsoleWindow.h"
#include "imgui.h"
#include "CodeEditor.h"
#include "ProcessEngine.h"

void SetupFileMenu()
{
    ImGui::MenuItem("(demo menu)", NULL, false, false);
    if (ImGui::MenuItem("New")) {}
    if (ImGui::MenuItem("Open", "Ctrl+O")) {}
    if (ImGui::MenuItem("Save", "Ctrl+S")) {}
    if (ImGui::MenuItem("Save As..")) {}
}

void SetupMainMenuBar()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            SetupFileMenu();
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
            if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
            ImGui::Separator();
            if (ImGui::MenuItem("Cut", "CTRL+X")) {}
            if (ImGui::MenuItem("Copy", "CTRL+C")) {}
            if (ImGui::MenuItem("Paste", "CTRL+V")) {}
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

}



struct DrawImageWindow
{
    GLuint my_image_texture = 0;

    void Initialize() {

       int my_image_width = 0;
       int my_image_height = 0;

       bool ret = Gui::LoadTextureFromFile("test/background.bmp", &my_image_texture, &my_image_width, &my_image_height);
       IM_ASSERT(ret);
    }

    void Draw(const char* title, bool* p_open)
    {
        ImGui::SetNextWindowSize(ImVec2(626, 744), ImGuiCond_FirstUseEver);
        if (!ImGui::Begin(title, p_open))
        {
            ImGui::End();
            return;
        }

        ImGui::Image((void*)(intptr_t)my_image_texture, ImVec2(313, 367));

        float sz = ImGui::GetTextLineHeight();
        ImVec2 p = ImGui::GetCursorScreenPos();
        ImGui::GetWindowDrawList()->AddRectFilled(p, ImVec2(p.x + sz, p.y + sz), ImGui::GetColorU32(ImVec4(1.0, 1.0, 1.0, 1.0)));

        ImGui::End();
    }
};

#define IM_MIN(A, B)            (((A) < (B)) ? (A) : (B))
#define IM_MAX(A, B)            (((A) >= (B)) ? (A) : (B))
#define IM_CLAMP(V, MN, MX)     ((V) < (MN) ? (MN) : (V) > (MX) ? (MX) : (V))

// Main code
int main(int, char**)
{
    Gui gui;

    gui.Initialize();
    gui.ApplyTheme();


    // Our state

    ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg;

    // Main loop
    bool done = false;

    ProcessEngine engine;

    DrawImageWindow imgWindow;
    ConsoleWindow console;
    CodeEditor editor;

    editor.Initialize();
    imgWindow.Initialize();

    while (!done)
    {
        done = gui.PollEvent();

        gui.StartFrame();

        ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
        SetupMainMenuBar();

        console.Draw("Console", nullptr);

        imgWindow.Draw("ImageWindow", nullptr);

        editor.Draw("Code Editor", nullptr);

        const char *columns[] = { "Jean", "57", "2", "19min 54s" };

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
        {
             static int counter = 0;

             ImGui::Begin("Pier-O-Bois - Logiciel de course");                          // Create a window called "Hello, world!" and append into it.

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


             if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                 counter++;

             ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "Platform");
             ImGui::Text("%s", SDL_GetPlatform());
             ImGui::Text("CPU cores: %d", SDL_GetCPUCount());
             ImGui::Text("RAM: %.2f GB", SDL_GetSystemRAM() / 1024.0f);
         //    ImGui::SameLine();
         //    ImGui::Text("counter = %d", counter);

         //    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);


             ImGui::End();
        }

        gui.EndFrame();
    }

    gui.Destroy();

    return 0;
}

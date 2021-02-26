#include "TableWindow.h"
#include "imgui.h"

TableWindow::TableWindow()
{

}



void TableWindow::Draw()
{
    const char *columns[] = { "Jean", "57", "2", "19min 54s" };
    ImGui::Text("Tableau des passages");               // Display some text (you can use a format strings too)

    ImGuiTableFlags tableFlags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg;

    if (ImGui::BeginTable("table1", 4, tableFlags))
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

}

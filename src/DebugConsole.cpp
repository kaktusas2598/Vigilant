#include "DebugConsole.hpp"

#include "Engine.hpp"
#include "Level.hpp"
#include "ScriptEngine.hpp"

namespace Vigilant {

    bool DebugConsole::layerVisibility = false;
    bool DebugConsole::colliderVisibility = false;
    bool DebugConsole::showDemoWindow = false;

    DebugConsole* DebugConsole::s_pInstance = nullptr;

    DebugConsole::DebugConsole() {
        clearLog();
        memset(inputBuf, 0, sizeof(inputBuf));
        historyPos = -1;

        // "CLASSIFY" is here to provide the test case where "C"+[tab] completes to "CL" and display multiple matches.
        //Commands.push_back("HELP");
        //Commands.push_back("HISTORY");
        //Commands.push_back("CLEAR");
        //Commands.push_back("CLASSIFY");
        autoScroll = true;
        scrollToBottom = false;
        addLog("Lua Console");
    }
    DebugConsole::~DebugConsole() {
        clearLog();
        //for (int i = 0; i < History.Size; i++)
            //free(History[i]);
    }

    void DebugConsole::addLog(const char* fmt, ...) { // IM_FMTARGS(2) ???
        char buf[1024];
        va_list args;
        va_start(args, fmt);
        vsnprintf(buf, IM_ARRAYSIZE(buf), fmt, args);
        buf[IM_ARRAYSIZE(buf)-1] = 0;
        va_end(args);
        //items.push_back(Strdup(buf));
        items.push_back(strdup(buf));

        //char tmp[strlen(fmt) + 1];
        //strcpy(tmp, fmt);
        //items.push_back(strdup(tmp));
    }

    void DebugConsole::clearLog() {
        for (int i = 0; i < items.Size; i++)
            free(items[i]);
        items.clear();
    }

    void DebugConsole::exec(const char* command) {
        // run luachunk
        std::string output = ScriptEngine::Instance()->runChunk(command);
        //const char* output = ScriptEngine::Instance()->runChunk(command).c_str();
        if (output[0] != '\0')
            addLog(output.c_str());
        // get output
        // implement some commands here?
    }

    void DebugConsole::render() {
        ImGui::Begin("Debug Console");

        //ImGui::Checkbox("Show IMGui Demo Window", &showDemoWindow);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::Text("Active entities: %d", EntityManager::livingCount);
        ImGui::Checkbox("Show collision layers", &layerVisibility);
        ImGui::Checkbox("Show colliders", &colliderVisibility);
        ImGui::Separator();

        Level* level = TheEngine::Instance()->getLevel();
        if (level != nullptr) {
            int i = 0;
            for (auto it = level->getCollisionLayers()->begin(); it != level->getCollisionLayers()->end(); ++it) {
                (*it)->setVisible(layerVisibility);
                i++;
            }
        }

        // Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        //ImGui::ShowDemoWindow(&showDemoWindow);

        //Everything imgui and lua console related could go to new DebugConsole class
        if (ImGui::SmallButton("Clear")) {
            clearLog();
        }
        ImGui::SameLine();
        bool copyToClipboard = ImGui::SmallButton("Copy");

        // Reserve enough left-over height for 1 separator + 1 input text
        const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
        if (ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_HorizontalScrollbar)) {
            if (ImGui::BeginPopupContextWindow()) {
                if (ImGui::Selectable("Clear")) clearLog();
                ImGui::EndPopup();
            }

            // - Consider using manual call to IsRectVisible() and skipping extraneous decoration from your items.
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1)); // Tighten spacing
            if (copyToClipboard)
                ImGui::LogToClipboard();
            for (int i = 0; i < items.Size; i++) {
                const char* item = items[i];
                if (!filter.PassFilter(item))
                    continue;

                // Normally you would store more information in your item than just a string.
                // (e.g. make Items[] an array of structure, store color/type etc.)
                ImVec4 color;
                bool has_color = false;
                if (strstr(item, "[error]")) { color = ImVec4(1.0f, 0.4f, 0.4f, 1.0f); has_color = true; }
                else if (strncmp(item, "# ", 2) == 0) { color = ImVec4(1.0f, 0.8f, 0.6f, 1.0f); has_color = true; }
                if (has_color)
                    ImGui::PushStyleColor(ImGuiCol_Text, color);
                ImGui::TextUnformatted(item);
                if (has_color)
                    ImGui::PopStyleColor();
            }
            if (copyToClipboard)
                ImGui::LogFinish();

            // Keep up at the bottom of the scroll region if we were already at the bottom at the beginning of the frame.
            // Using a scrollbar or mouse-wheel will take away from the bottom edge.
            if (scrollToBottom || (autoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()))
                ImGui::SetScrollHereY(1.0f);
            scrollToBottom = false;

            ImGui::PopStyleVar();

        }
        ImGui::EndChild();
        ImGui::Separator();

        // Command-line
        bool reclaimFocus = false;
        ImGuiInputTextFlags inputTextFlags = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_EscapeClearsAll | ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackHistory;
        if (ImGui::InputText("Input", inputBuf, IM_ARRAYSIZE(inputBuf), inputTextFlags, &textEditCallbackStub, (void*)this)) {
            char* s = inputBuf;
            //Strtrim(s);
            if (s[0])
                exec(s);
            strcpy(s, "");
            reclaimFocus = true;
        }

        // Auto-focus on window apparition
        ImGui::SetItemDefaultFocus();
        if (reclaimFocus)
            ImGui::SetKeyboardFocusHere(-1); // Auto focus previous widget


        ImGui::End();
    }

    // In C++11 you'd be better off using lambdas for this sort of forwarding callbacks
    int DebugConsole::textEditCallbackStub(ImGuiInputTextCallbackData* data) {
        return DebugConsole::Instance()->textEditCallback(data);
    }

    int DebugConsole::textEditCallback(ImGuiInputTextCallbackData* data) {
        // TODO: autocompletion
    }
}

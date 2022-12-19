#include "DebugConsole.hpp"

#include "Engine.hpp"
#include "ScriptEngine.hpp"

namespace Vigilant {

    bool DebugConsole::layerVisibility = false;
    bool DebugConsole::colliderVisibility = false;
    bool DebugConsole::showDemoWindow = false;

    DebugConsole* DebugConsole::s_pInstance = nullptr;

    DebugConsole::DebugConsole() {
        clearLog();
        memset(inputBuf, 0, sizeof(inputBuf));
        //HistoryPos = -1;

        // "CLASSIFY" is here to provide the test case where "C"+[tab] completes to "CL" and display multiple matches.
        //Commands.push_back("HELP");
        //Commands.push_back("HISTORY");
        //Commands.push_back("CLEAR");
        //Commands.push_back("CLASSIFY");
        autoScroll = true;
        scrollToBottom = false;
        addLog("Welcome to Vigilant Engine");
    }
    DebugConsole::~DebugConsole() {
        clearLog();
        //for (int i = 0; i < History.Size; i++)
            //free(History[i]);
    }


    void DebugConsole::addLog(const char* log) {
        //int old_size = Buf.size();
        //va_list args;
        //va_start(args, fmt);
        //Buf.appendfv(fmt, args);
        //va_end(args);
        //for (int new_size = Buf.size(); old_size < new_size; old_size++)
            //if (Buf[old_size] == '\n')
                //LineOffsets.push_back(old_size + 1);
    }
    void DebugConsole::clearLog() {
        //for (int i = 0; i < Items.Size; i++)
            //free(Items[i]);
        //Items.clear();
    }

    void DebugConsole::exec(const char* command) {
        // run luachunk
        ScriptEngine::Instance()->loadChunk(command);
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


        ImGui::End();
    }

    // In C++11 you'd be better off using lambdas for this sort of forwarding callbacks
    int DebugConsole::textEditCallbackStub(ImGuiInputTextCallbackData* data) {
        return DebugConsole::Instance()->textEditCallback(data);
    }

    int DebugConsole::textEditCallback(ImGuiInputTextCallbackData* data) {
    }
}

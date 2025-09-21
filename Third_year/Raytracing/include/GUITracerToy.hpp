#pragma once

#include <memory>
#include <glm/glm.hpp>

#include "imgui.h"
#include "ImGuiFileDialog/ImGuiFileDialog.h"

#include <unordered_map>
#include "Controller.hpp"

using namespace glm;

class GUITracerToy {
    private:
        std::shared_ptr<Config> setup;
    public:
    GUITracerToy(int ww, int wh) {
            setup = std::make_shared<Config>(ww, wh);
    };
        
    void initStyle(ImGuiStyle &style);
    void renderConfigGUI();
    void renderMenus();
    std::shared_ptr<Config> getSetup() { return setup; }
    bool renderColorPicker(const char* label, glm::vec3* color);
    bool backgroundDialog();
    
    std::string openFileDialog();

    void openFileSaveDialog(bool isAnim);
    void handleFileDialog(bool isAnimation);
};

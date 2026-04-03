
#include "GUITracerToy.hpp"

// Funció per renderitzar la interfície de configuració
void GUITracerToy::renderConfigGUI() {
    // Estil de la GUI
    initStyle(ImGui::GetStyle());
    
    renderMenus();

    // Configuració de la finestra
    ImGui::SetNextWindowSizeConstraints(ImVec2(700, 700), ImVec2(1200, 800));
    ImGui::Begin("Configuració PathTracing", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize| ImGuiWindowFlags_NoMove);
    float msegons = Controller::getInstance()->getRenderTime();
   
    ImGui::Text("Rendering average %.3f ms/frame (%.1f FPS)", msegons, 1000.0f / msegons);

    // SECCIÓ CÀMERA
    if (ImGui::CollapsingHeader("Càmera", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::Separator();
        bool updated = false;

        updated |= ImGui::SliderFloat("X OBS", &setup->observador.x, -20.0f, 20.0f);
        updated |= ImGui::SliderFloat("Y OBS", &setup->observador.y, -20.0f, 20.0f);
        updated |= ImGui::SliderFloat("Z OBS", &setup->observador.z, -20.0f, 20.0f);
        updated |= ImGui::SliderFloat("X VRP", &setup->vrp.x, -1.0f, 1.0f);
        updated |= ImGui::SliderFloat("Y VRP", &setup->vrp.y, -1.0f, 1.0f);
        updated |= ImGui::SliderFloat("Z VRP", &setup->vrp.z, -1.0f, 1.0f);
        updated |= ImGui::SliderFloat("FOV (30-120)", &setup->fov, 30.0f, 120.0f);
        updated |= ImGui::SliderInt("Màxima profunditat", &setup->maxdepth , 0, 10);
        updated |= ImGui::SliderInt("Num Samples", &setup->numSamples, 1, 10);
        updated |= ImGui::SliderInt("Factor rugositat", &setup->KpathTracing, 0, 10);

        if (updated) {
            Controller::getInstance()->changeCamera(setup->observador, setup->vrp, vec3(0.0f, 1.0f, 0.0f), setup->fov);
            Controller::getInstance()->requestRender();
        }
    }
    
    // SECCIÓ LLUM
    if (ImGui::CollapsingHeader("Llum", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::Separator();
        bool updated = false;

        updated |= renderColorPicker("Ambient global", &setup->lightAmbientGlobal);

        updated |= ImGui::SliderFloat("X Posició", &setup->lightPos.x, -20.0f, 20.0f);
        updated |= ImGui::SliderFloat("Y Posició", &setup->lightPos.y, -20.0f, 20.0f);
        updated |= ImGui::SliderFloat("Z Posició", &setup->lightPos.z, -20.0f, 20.0f);

        updated |= renderColorPicker("Ambient", &setup->lightAmbient);
        updated |= renderColorPicker("Difusa", &setup->lightDiffuse);
        updated |= renderColorPicker("Especular", &setup->lightSpecular);

        if (updated) 
            Controller::getInstance()->requestRender();
    }

    // SECCIÓ ESCENA
    if (ImGui::CollapsingHeader("Escena", ImGuiTreeNodeFlags_DefaultOpen)) {

        ImGui::Separator();

        if (ImGui::SliderInt("Nombre d'Esferes", &setup->numSpheres, 1, 20)) {
            // TODO: Cal cridar el metode per a generar objectes aleatoris
            // i despres cridar a refrescar l'escena amb requestRender()
            // Controller::getInstance()->requestRender();

            Controller::getInstance()->generateRandomSpheres(setup->numSpheres);
            Controller::getInstance()->requestRender();
        }

        if (ImGui::Checkbox("Ombres", &setup->shadow)) {
            Controller::getInstance()->requestRender();
        }
        if (ImGui::Checkbox("Bounding Box", &setup->boundingBox)) {
            Controller::getInstance()->requestRender();
        }
        if (ImGui::Checkbox("Reflexa El Background", &setup->reflejaElBackground)) {
            Controller::getInstance()->requestRender();
        }
    }

    // SECCIÓ FONS
    if (ImGui::CollapsingHeader("Fons", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::Separator();
        
        ImGui::Text("Selecció de Fons:");
        bool updated = false;

        ImGui::RadioButton("Color", &setup->backgroundMode, 0);
        ImGui::SameLine();
        ImGui::RadioButton("Textura", &setup->backgroundMode, 1);

        if (setup->backgroundMode == 0) {
            updated |= renderColorPicker("Color de Fons", &setup->backgroundColor);
        } else {
            updated |= backgroundDialog();
        }

        if (updated) {
            Controller::getInstance()->requestRender();
        }          
    }

    // TODO: afegir més sessions o paràmetres si et calen

    ImGui::End();
}

void GUITracerToy::renderMenus() {
    // Menú superior
    std::string selectedTracer = "Raycasting";
    std::string selectedShading = "Normal Shading";

    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("Save")) {
            if (ImGui::MenuItem("Save Rendering")) {
                // Aquí pots guardar la renderització
                std::cout << "Saving Rendering..." << std::endl;
                initStyle(ImGui::GetStyle());
                openFileSaveDialog(false);
               
            }
        if (ImGui::MenuItem("Save Animation")) {
            // Aquí pots guardar l'animació
            initStyle(ImGui::GetStyle());
            std::cout << "Saving Animation..." << std::endl;
            openFileSaveDialog(true);
        }
        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Tracers")) {
        if (ImGui::MenuItem("Raycasting", NULL, selectedTracer == "Raycasting")) {
            setup->selectedTracer = 0;
            Controller::getInstance()->requestRender();
        }
        if (ImGui::MenuItem("Raytracing", NULL, selectedTracer == "Raytracing")) {
            setup->selectedTracer = 1;
            Controller::getInstance()->requestRender();
        }
        if (ImGui::MenuItem("Pathtracing", NULL, selectedTracer == "Pathtracing")) {
            setup->selectedTracer = 2;
            Controller::getInstance()->requestRender();
        }
        if (ImGui::MenuItem("examen", NULL, selectedTracer == "Examen")) {
            setup->selectedTracer = 3;
            Controller::getInstance()->requestRender();
        }
        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Shadings")) {
        if (ImGui::MenuItem("Normal", NULL, selectedShading == "Normal Shading")) {
            setup->selectedShader = 0;
            Controller::getInstance()->requestRender();
        }
        if (ImGui::MenuItem("Diffuse", NULL, selectedShading == "Diffuse")) {
            setup->selectedShader = 1;
            Controller::getInstance()->requestRender();
        }
        if (ImGui::MenuItem("Blinn-Phong", NULL, selectedShading == "Blinn-Phong")) {
            setup->selectedShader = 2;
            Controller::getInstance()->requestRender();
        }
        if (ImGui::MenuItem("Toon shading", NULL, selectedShading == "Toon Shading")) {
            setup->selectedShader = 3;
            Controller::getInstance()->requestRender();
        }
        ImGui::EndMenu();
    }

    ImGui::EndMainMenuBar();
    }
    // Mostrar les opcions seleccionades a la interfície
    ImGui::Text("Algorisme seleccionat: %s", selectedTracer.c_str());
    ImGui::Text("Shading seleccionat: %s", selectedShading.c_str());
    handleFileDialog(false);
    handleFileDialog(true);  
}


// Definim un widget que obre un diàleg de selecció de fitxers
bool GUITracerToy::backgroundDialog() {
    bool changed = false;

    ImGui::Text("Nom del fitxer de textura:");
    
    initStyle(ImGui::GetStyle());
    
    // Botó per obrir el diàleg
    if (ImGui::Button("Obrir fitxer")) {
        // Configuració del diàleg
        IGFD::FileDialogConfig config;
        config.path = "./resources";  // Ruta inicial
        config.flags = ImGuiFileDialogFlags_Modal;  // Opcional: obrir com a modal
        config.sidePaneWidth = 800; // Mida del diàleg
        
        ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Escull un fitxer", ".jpg,.ppm,.png,.jpeg", config);
    }

    // Mostrar el diàleg si està obert
    if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey")) {
        // Si es fa clic en "Obrir", obtenim el fitxer seleccionat

        if (ImGuiFileDialog::Instance()->IsOk()) {
            std::string filePath = ImGuiFileDialog::Instance()->GetFilePathName(); // Ruta completa
            std::string fileName = ImGuiFileDialog::Instance()->GetCurrentFileName(); // Nom del fitxer
            ImGui::Text("Fitxer seleccionat: %s", fileName.c_str());
            // TODO: Llegir el nom de fitxer a la configuracio per carregar la textura
            if (setup->background != nullptr) {
                setup->background->freeImage();
            }
            setup->background = make_shared<Image>(filePath.c_str());
            changed = true;
        }

        // Si es tanca la finestra sense seleccionar, només tanquem el diàleg
        ImGuiFileDialog::Instance()->Close();
    }
    
    return changed;
}

bool GUITracerToy::renderColorPicker(const char* label, glm::vec3* color) {
    static std::unordered_map<std::string, bool> showColorPicker;
    bool changed = false;

    initStyle(ImGui::GetStyle());
   
    if (ImGui::ColorEdit3(label, &color->x)) {
        changed = true;  // Notifica un canvi de color
    }

    // Si es fa clic, alterna l'estat d'aquest color picker (obert/tancat)
    if (ImGui::IsItemClicked()) {
        showColorPicker[label] = !showColorPicker[label];
    }

    // Si el ColorPicker està obert, mostra'l
    if (showColorPicker[label]) {
        ImGui::SetNextWindowPos(ImGui::GetMousePos(), ImGuiCond_Appearing);
        ImGui::Begin(label, &showColorPicker[label], ImGuiWindowFlags_NoCollapse);
        if (ImGui::ColorPicker3("Escull un color", &color->x)) {
            changed = true; // Notifica el canvi de color
        }
        ImGui::End();
    }

    // Quan un color picker es tanca, força un refresc manualment
    if (!showColorPicker[label]) {
        glfwPostEmptyEvent();  // Notifica GLFW per evitar bloquejos
    }
    //ImGui::PopStyleColor(6);
    return changed;
}

std::string GUITracerToy::openFileDialog() {
    std::string selectedFile = "";

    initStyle(ImGui::GetStyle());

    IGFD::FileDialogConfig config;
    config.path = "./resources";  // Ruta inicial
    config.flags = ImGuiFileDialogFlags_Modal;  // Opcional: obrir com a modal
    config.sidePaneWidth = 800; // Mida del diàleg
    
    // Obrir el diàleg
    ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Selecciona un fitxer", ".jpg,.png,.txt,.obj", config);
    
    // Mostrar el diàleg quan estigui obert
    if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey")) {
        // ✅ Si l'usuari ha seleccionat un fitxer, el retornem
        if (ImGuiFileDialog::Instance()->IsOk()) {

            selectedFile = ImGuiFileDialog::Instance()->GetFilePathName(); // Ruta completa
            std::string fileName = ImGuiFileDialog::Instance()->GetCurrentFileName(); // Nom del fitxer
            ImGui::Text("Fitxer seleccionat: %s", fileName.c_str());
        }
        ImGuiFileDialog::Instance()->Close();
    }

    return selectedFile;
}

void GUITracerToy::openFileSaveDialog(bool isAnimation) {
    IGFD::FileDialogConfig config;
    config.path = "./resources";  // Ruta inicial
    config.flags = ImGuiFileDialogFlags_Modal;  // Opcional: obrir com a modal
    config.sidePaneWidth = 800; // Mida del diàleg
    
    if (isAnimation) {
        ImGuiFileDialog::Instance()->OpenDialog(
            "SaveAnimationDialog", 
            "Selecciona un prefix per als frames de l'animació", 
            ".png,.jpg,.bmp", 
            config);
    } else {
        ImGuiFileDialog::Instance()->OpenDialog(
            "SaveRenderingDialog", 
            "Selecciona un fitxer per guardar la imatge", 
            ".png,.jpg,.bmp,.tga", 
            config);
    }
}

// Funció per mostrar el diàleg i capturar el fitxer seleccionat
void GUITracerToy::handleFileDialog(bool isAnimation) {
    const char* dialogKey = isAnimation ? "SaveAnimationDialog" : "SaveRenderingDialog";

    std::string saveFilePath = "";      // Ruta del fitxer únic
    std::string animationBaseName = ""; // Prefix per als frames de l'animació

    if (ImGuiFileDialog::Instance()->Display(dialogKey)) {
        if (ImGuiFileDialog::Instance()->IsOk()) {
            std::string filePath = ImGuiFileDialog::Instance()->GetFilePathName();
            if (isAnimation) {
                // Eliminar l'extensió per generar frames (ex: "output" de "output.png")
                animationBaseName = filePath.substr(0, filePath.find_last_of("."));
                std::cout << "Generant i Guardant animació a: " << animationBaseName << "_XXX.png" << std::endl;
                Controller::getInstance()->saveAnimation(animationBaseName.c_str());
            } else {
                saveFilePath = filePath;
                std::cout << "Fitxer guardat com: " << saveFilePath << std::endl;
                Controller::getInstance()->saveImage(saveFilePath.c_str());
            }
        }
        ImGuiFileDialog::Instance()->Close();
    }
}

// Funció per inicialitzar l'estil de la GUI
void GUITracerToy::initStyle(ImGuiStyle &style) {
        
    ImVec4* colors = style.Colors;
    
    // 🎨 Estil de colors
    ImVec4 lightGray = ImVec4(0.9f, 0.9f, 0.9f, 1.0f);
    ImVec4 darkGray = ImVec4(0.3f, 0.3f, 0.3f, 1.0f);
    ImVec4 blueAccent = ImVec4(0.2f, 0.4f, 0.8f, 1.0f);

    // 🎨 Colors generals
    colors[ImGuiCol_FrameBg] = lightGray;                               // Fons dels frames
    colors[ImGuiCol_WindowBg] = lightGray;                              // Fons de la finestra
    colors[ImGuiCol_TitleBg] = darkGray;                                // Fons de la barra de títol
    colors[ImGuiCol_TitleBgActive] = blueAccent;                        // Quan la finestra està activa
    colors[ImGuiCol_FrameBg] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);          // Fons blanc dels sliders
    colors[ImGuiCol_ChildBg] = ImVec4(0.98f, 0.98f, 0.98f, 1.0f);       // Fons per seccions
    colors[ImGuiCol_Border] = ImVec4(0.8f, 0.8f, 0.8f, 1.0f);           // Borde suau
    colors[ImGuiCol_FrameBg] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);          // Camps de text blancs
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.9f, 0.9f, 0.9f, 1.0f); 
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.85f, 0.85f, 0.85f, 1.0f);
    colors[ImGuiCol_Separator] = ImVec4(0.8f, 0.8f, 0.8f, 1.0f);        // Separadors
    colors[ImGuiCol_SeparatorHovered] = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);
    colors[ImGuiCol_SeparatorActive] = ImVec4(0.6f, 0.6f, 0.6f, 1.0f);

    // 🎨 Botons i elements interactius
    colors[ImGuiCol_Button] = ImVec4(0.85f, 0.85f, 0.9f, 1.0f);         // Color dels botons
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.5f, 0.5f, 0.6f, 1.0f);    // Botons en hover
    colors[ImGuiCol_ButtonActive] = ImVec4(0.1f, 0.3f, 0.6f, 1.0f);     // Botons quan són premuts
    colors[ImGuiCol_PopupBg] = ImVec4(0.95f, 0.95f, 0.98f, 1.0f);
    // 🎨 Barra de menú
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.9f, 0.9f, 0.95f, 1.0f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.8f, 0.8f, 0.9f, 1.0f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.75f, 0.75f, 0.85f, 1.0f);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.8f, 0.8f, 0.9f, 1.0f);
        
    // 🎨 Selecció i text
    colors[ImGuiCol_Text] = ImVec4(0.2f, 0.2f, 0.3f, 1.0f);
    colors[ImGuiCol_TextSelectedBg] = ImVec4(0.6f, 0.7f, 0.9f, 0.3f);
    colors[ImGuiCol_Header] = ImVec4(0.85f, 0.85f, 0.9f, 1.0f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.75f, 0.75f, 0.85f, 1.0f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.6f, 0.6f, 0.8f, 1.0f);
    colors[ImGuiCol_TableHeaderBg] = ImVec4(0.85f, 0.85f, 0.9f, 1.0f);
    colors[ImGuiCol_TableRowBg] = ImVec4(0.92f, 0.92f, 0.97f, 1.0f);
    colors[ImGuiCol_TableRowBgAlt] = ImVec4(0.88f, 0.88f, 0.94f, 1.0f);

    // 🔹 Vores i cantonades arrodonides
    style.WindowRounding = 6.0f;
    style.FrameRounding = 4.0f;
    style.ChildRounding = 6.0f;
    style.PopupRounding = 6.0f;
    style.GrabRounding = 4.0f;
    style.ScrollbarRounding = 6.0f;
    
    // 🔹 Ajust d'espaiats i marges
    style.WindowPadding = ImVec2(10, 10);
    style.FramePadding = ImVec2(8, 4);
    style.ItemSpacing = ImVec2(12, 6);
}    


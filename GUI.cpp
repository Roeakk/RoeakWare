#include "GUI.h"
#include <iostream>
#include <GL/gl.h>
#include <chrono>
#include <glm/ext/vector_float4.hpp>
#include <map> 

// Static initialization flags and settings
static bool is_init = false;
static bool do_draw = true;
static bool huddraw = true;

// Fonts
static ImFont* arialFont = nullptr;
static ImFont* bigFont = nullptr;

// GUI options (enabled or disabled states for features)
bool GUI::xrayEnabled = false;
bool GUI::esp2dEnabled = false;
bool GUI::tracersEnabled = false;
bool GUI::nametagsEnabled = false;
bool GUI::chinaHatEnabled = false;
bool GUI::triggerbotEnabled = false;
bool GUI::aimbotEnabled = false;
bool GUI::aimCircleEnabled = false;
bool GUI::aimOnlyClick = false;
bool GUI::leftclickEnabled = false;
bool GUI::rightclickEnabled = false;
bool GUI::playerScaleEnabled = false;
bool GUI::chamsEnabled = false;
bool GUI::triggerToggle = false;
bool GUI::wallHackEnabled = false;
bool GUI::nameTagsEnabled = false;
bool GUI::chestespEnabled = false;
bool GUI::itemScaleEnabled = false;
bool GUI::itemespEnabled = false;
bool GUI::playerwallhackEnabled = false;
bool GUI::chestwallhackEnabled = false;
bool GUI::itemwallhackEnabled = false;
bool GUI::chamsChest = false;
bool GUI::chamsPlayer = false;
bool GUI::chamsItem = false;

int espselected = (GUI::espModeSelected == 1.0f) ? 1 : 0;
const char* espOptions[] = { "3D", "2D" };

float GUI::playerx = 1.0f;
float GUI::playery = 1.0f;
float GUI::playerz = 1.0f;

float GUI::itemx = 1.0f;
float GUI::itemy = 1.0f;
float GUI::itemz = 1.0f;

float GUI::tagsscale = 1.0f;

float GUI::triggerBotRange = 5.0f;
float GUI::triggetbotmincps = 9;
float GUI::triggetbotmaxcps = 11;


float GUI::aimbotRange = 5.0f;
float GUI::aimbotSpeed = 0.08f;

float GUI::leftclickermincps = 9;
float GUI::rightclickermincps = 9;
float GUI::leftclickermaxcps = 11;
float GUI::rightclickermaxcps = 11;
float minLeftCps = 1.0f;
float maxLeftCps = 20.0f;
float minRightCps = 1.0f;
float maxRightCps = 20.0f;

bool GUI::chinaRainbowEnabled = false;
float GUI::chinaRainbowSpeed = 1.0f;
float GUI::chinaR = 255;
float GUI::chinaG = 255;
float GUI::chinaB = 255;

bool GUI::espRainbow = false;
float GUI::espModeSelected = 1.0f;
float GUI::espRainbowSpeed = 1.0f;
float GUI::espR = 255;
float GUI::espG = 255;
float GUI::espB = 255;

float GUI::chestR = 1.0f;     // Kırmızı
float GUI::chestG = 0.0f;     // Yeşil
float GUI::chestB = 0.0f;     // Mavi

float GUI::itemR = 1.0f;     // Kırmızı
float GUI::itemG = 0.0f;     // Yeşil
float GUI::itemB = 0.0f;     // Mavi

float GUI::largeChestR = 0.0f;
float GUI::largeChestG = 1.0f;
float GUI::largeChestB = 0.0f;

bool GUI::chamsRainbowEnabled = false;
float GUI::chamsRainbowSpeed = 1.0f;
float GUI::chamsR = 255;
float GUI::chamsG = 255;
float GUI::chamsB = 255;

bool GUI::tracerRainbow = false;
float GUI::tracerColorR = 255;
float GUI::tracerColorG = 255;
float GUI::tracerColorB = 255;
float GUI::tracerOpacity = 100;
float GUI::tracerRainbowSpeed = 1.0;
float GUI::tracerColorThickness = 1.0f;

float GUI::targetespColorR = 255;
float GUI::targetespColorG = 255;
float GUI::targetespColorB = 255;

int currentCategory = 1;
int GUI::currentBindKey = -1;
bool GUI::isBindingKey = false;
bool* GUI::currentModuleToggle = nullptr;

float titleHeight = 20.0f;
float windowRounding = 0.0f;

std::unordered_map<int, bool*> GUI::keyBinds;

bool GUI::init(HWND wnd_handle)
{
    if (is_init)
        return false;

    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    

    if (!io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\arial.ttf", 16.0f))
    {
        std::cerr << "Failed to load font." << std::endl;
        return false;
    }
    arialFont = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\arial.ttf", 16.0f);

    if (!io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\arial.ttf", 19.0f))
    {
        std::cerr << "Failed to load big font." << std::endl;
        return false;
    }
    bigFont = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\arial.ttf", 19.0f);

    ImGui::StyleColorsDark();
    ImGui_ImplWin32_Init(wnd_handle);
    ImGui_ImplOpenGL3_Init("#version 130");

    is_init = true;
    return true;
}

void GUI::shutdown()
{
    if (!is_init)
        return;

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    is_init = false;
}




bool espEnabled = false;
bool chamsEnabled = false;
bool xrayEnabled = false;
bool aimbotEnabled = false;
bool leftclickEnabled = false;
bool rightclickEnabled = false;
bool triggerbotEnabled = false;
int currentTab = 0; // **Global hale getirildi**



char searchBuffer[64] = "";

void GUI::draw() {
    if (!do_draw || !is_init)
        return;

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 5.0f;
    style.ChildRounding = 5.0f;
    style.FrameRounding = 3.0f;
    style.ScrollbarRounding = 3.0f;
    style.GrabRounding = 3.0f;
    style.ItemSpacing = ImVec2(8, 8);

    ImVec4 mainColor(0.1f, 0.1f, 0.1f, 1.0f);
    ImVec4 accentColor(0.0f, 0.5f, 1.0f, 1.0f);
    ImVec4 sideBarColor(0.08f, 0.08f, 0.08f, 1.0f);

    ImGui::PushStyleColor(ImGuiCol_WindowBg, mainColor);
    ImGui::PushStyleColor(ImGuiCol_TitleBg, accentColor);
    ImGui::PushStyleColor(ImGuiCol_TitleBgActive, accentColor);
    ImGui::PushStyleColor(ImGuiCol_TitleBgCollapsed, accentColor);

    ImGui::SetNextWindowSize(ImVec2(700, 500), ImGuiCond_Once);
    ImGui::Begin("Module Manager", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    // Sol Menü
    ImGui::BeginChild("Sidebar", ImVec2(170, 0), true, ImGuiWindowFlags_NoScrollbar);
    ImGui::PushStyleColor(ImGuiCol_ChildBg, sideBarColor);
    static int currentTab = 0;
    if (ImGui::Button("Duyuru", ImVec2(160, 40))) currentTab = 0;  // Duyuru tab'ı 0 yapıldı
    if (ImGui::Button("Ragebot", ImVec2(160, 40))) currentTab = 1;
    if (ImGui::Button("Legitbot", ImVec2(160, 40))) currentTab = 2;
    if (ImGui::Button("Visuals", ImVec2(160, 40))) currentTab = 3;  // Visuals tab'ı 3 yapıldı
    if (ImGui::Button("Configs", ImVec2(160, 40))) currentTab = 4;
    ImGui::PopStyleColor();
    ImGui::EndChild();

    ImGui::SameLine();
    ImGui::BeginChild("MainContent", ImVec2(0, 0), true);

    if (currentTab == 0) {  // Duyuru sekmesi
        ImGui::Text("Duyuru");
        ImGui::Separator();
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));  // Kırmızı renk
        ImGui::Text("06.04.2025 Cikis Tarihi");
        ImGui::PopStyleColor();
    }
    else if (currentTab == 1) {
        ImGui::Text("Ragebot Settings");
        ImGui::Separator();
        if (ImGui::CollapsingHeader("Aimbot")) {
            ImGui::Checkbox("Enable Aimbot", &aimbotEnabled);
        }
    }
    else if (currentTab == 2) { // Legitbot Sekmesi
        ImGui::Text("Legitbot Settings");
        ImGui::Separator();
        if (ImGui::CollapsingHeader("LegitAura")) {
            ImGui::Checkbox("Enable LegitAura", &triggerbotEnabled);
        }
    }
    else if (currentTab == 3) {  // Visuals Sekmesi
        ImGui::Text("Visuals Settings");
        ImGui::Separator();
        if (ImGui::CollapsingHeader("ESP")) {
            ImGui::Checkbox("Enable ESP", &esp2dEnabled);
        }
        if (ImGui::CollapsingHeader("Chams")) {
            ImGui::Checkbox("Enable Chams", &chamsEnabled);
            const char* chamStyles[] = { "Flat", "Glow", "Textured", "Glass" };
            static int currentStyle = 0;
            ImGui::Combo("Style", &currentStyle, chamStyles, IM_ARRAYSIZE(chamStyles));
        }
        if (ImGui::CollapsingHeader("X-Ray")) {
            ImGui::Checkbox("Enable X-Ray", &xrayEnabled);
        }
    }

    else if (currentTab == 4) {  // Configs Tab
        ImGui::Text("Player Scaling Settings");
        ImGui::Separator();

        // Player Scale Settings
        ImGui::Checkbox("Enable Player Scaling", &playerScaleEnabled);
        ImGui::SliderFloat("Scale X", &playerx, 0.5f, 2.0f);  // X boyutunu ayarlamak
        ImGui::SliderFloat("Scale Y", &playery, 0.5f, 2.0f);  // Y boyutunu ayarlamak
        ImGui::SliderFloat("Scale Z", &playerz, 0.5f, 2.0f);  // Z boyutunu ayarlamak
    }

    ImGui::EndChild();

    // Add the custom labels at the bottom left
    ImGui::SetCursorPos(ImVec2(10, ImGui::GetWindowHeight() - 40));  // Position the text
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 0.0f, 1.0f)); // Set yellow color for Premium text
    ImGui::Text("RoeakCheats Trial Version");
    ImGui::PopStyleColor();

    ImGui::SetCursorPos(ImVec2(10, ImGui::GetWindowHeight() - 20));  // Adjust position for footer
    ImGui::Text("Developed by Roeak1337 & schwenzyxrd");

    ImGui::PopStyleColor(4);
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}











void GUI::toggleVisibility()
{
    do_draw = !do_draw;
}

void GUI::hud(const std::string& featureName, bool featureEnabled, float& yOffset)
{
    if (featureEnabled) {

        auto draw_list = ImGui::GetBackgroundDrawList();

        ImVec2 featureTextSize = ImGui::CalcTextSize(featureName.c_str());

        ImVec2 featurePos(9.5f, yOffset);

        ImColor bgColor(1.0f, 1.0f, 1.0f, 0.5f); // Şeffaf beyaz arka plan
        ImColor textColor(50, 50, 50, 255); // Koyu gri yazı rengi

        float rounding = 0.0f; // Köşe yuvarlama yarıçapı
        float elementHeight = featureTextSize.y + 4.0f; // Kutunun yüksekliği

        // Arka plan (şeffaf beyaz ve yuvarlatılmış köşeler)
        draw_list->AddRectFilled(
            ImVec2(featurePos.x - 2.0f, featurePos.y - 2.0f),
            ImVec2(featurePos.x + featureTextSize.x + 4.0f, featurePos.y + featureTextSize.y + 2.0f),
            bgColor,
            rounding // Köşe yuvarlama
        );

        // Metin
        draw_list->AddText(featurePos, textColor, featureName.c_str());

        // Sonraki öğe için yOffset'i artır
        yOffset += elementHeight;
    }
}




std::chrono::steady_clock::time_point lastFpsUpdateTime = std::chrono::steady_clock::now();
int currentFps = 0;
int frameCount = 0;

#include <vector>
#include <chrono>

#include <vector>
#include <chrono>
#include <map>  // 📌 Eksik başlık eklendi!

struct Notification {
    std::string message;
    std::chrono::steady_clock::time_point startTime;
    float alpha;  // Opaklık (transparanlık) için animasyon
    float offsetX;  // Bildirimin sağa veya sola kayması için
};

std::vector<Notification> notifications;

void GUI::addNotification(const std::string& message) {
    notifications.clear(); // Önceki bildirimi sil
    notifications.push_back({message, std::chrono::steady_clock::now(), 1.0f, 0.0f});
}

void GUI::drawHud()
{
    updateRainbowEffects();
    if (!do_draw)
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        auto draw_list = ImGui::GetBackgroundDrawList();
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - lastFpsUpdateTime).count();

        if (elapsed >= 1) {
            currentFps = frameCount;
            frameCount = 0;
            lastFpsUpdateTime = now;
        }
        frameCount++;

        std::string fpsText = std::to_string(currentFps) + "";
        std::string watermark = "RoeakWare |"+ fpsText;

        ImVec2 textPos(15.0f, 15.0f);
        ImVec2 textSize = ImGui::CalcTextSize(watermark.c_str());
        ImColor bgColor(15, 15, 15, 180);
        ImColor glowColor(0, 255, 0, 150);  // Fosforlu yeşil, ama biraz daha az parlaklıkla

        float rounding = 8.0f;

        draw_list->AddRectFilled(
            ImVec2(textPos.x - 5.0f, textPos.y - 5.0f),
            ImVec2(textPos.x + textSize.x + 10.0f, textPos.y + textSize.y + 10.0f),
            bgColor, rounding
        );

        // Glow efektini biraz daha az yapmak için offset değeri küçük tutuldu
        for (int i = 0; i < 3; i++) {  // 3 defa çizilecek, daha yumuşak bir etki için
            float offset = i * 1.0f; // Glow etkisinin yayılma mesafesi biraz daha az
            draw_list->AddText(ImVec2(textPos.x + offset, textPos.y + offset), glowColor, watermark.c_str());
        }

        draw_list->AddText(ImVec2(textPos.x, textPos.y), ImColor(255, 255, 255, 255), watermark.c_str());  // Ana metin

        float yOffset = textPos.y + textSize.y + 25.0f;

        ImColor moduleBgColor(20, 20, 20, 200);
        ImColor moduleTextColor(255, 165, 0, 255);

        // 📌 **Önceki modül durumlarını saklamak için static bir harita (map)**
        static std::map<std::string, bool> previousModuleStates;

        std::vector<std::pair<bool, std::string>> modules = {
            {aimbotEnabled, "Aimbot"},
            {esp2dEnabled, "ESP"},
            {chamsEnabled, "Disabler (SO)"},
            {wallHackEnabled, "WallHack"},
            {tracersEnabled, "Tracers"},
            {leftclickEnabled, "LeftClicker"},
            {rightclickEnabled, "RightClicker"},
            {triggerbotEnabled, "LegitAura"}
        };

        for (const auto& module : modules) {
            if (module.first) {
                draw_list->AddRectFilled(ImVec2(textPos.x, yOffset), ImVec2(textPos.x + 220, yOffset + 22), moduleBgColor, 6.0f);
                draw_list->AddText(ImVec2(textPos.x + 8, yOffset + 3), moduleTextColor, module.second.c_str());
                yOffset += 30;
            }

            // 📌 **Eğer modülün durumu değiştiyse bildirim ekleyelim**
            if (previousModuleStates[module.second] != module.first) {
                std::string status = module.first ? "Enabled" : "Disabled";
                addNotification(module.second + " " + status);
                previousModuleStates[module.second] = module.first; // **Durumu güncelle**
            }
        }

        // 📢 **Bildirimleri sağ alt köşede TEK olarak gösterme**
        if (!notifications.empty()) {
            auto& lastNotification = notifications.back();
            auto timeElapsed = std::chrono::duration_cast<std::chrono::seconds>(now - lastNotification.startTime).count();

            // 📌 **Bildirim Süresi Kısaltıldı ve Animasyon Eklendi**
            if (timeElapsed < 1) { // Bildirim 1 saniye görünür kalacak
                lastNotification.alpha = 1.0f - (float)timeElapsed / 1.0f; // Opaklık kaybolma süresi
                lastNotification.offsetX = 250.0f * (1.0f - lastNotification.alpha); // Sağdan kayarak kaybolma

                float notifyX = ImGui::GetIO().DisplaySize.x - 250.0f + lastNotification.offsetX; // X konumunu animasyonla ayarla
                float notifyY = ImGui::GetIO().DisplaySize.y - 80.0f;  // Sağ alt köşe için Y

                ImVec2 notifySize = ImGui::CalcTextSize(lastNotification.message.c_str());
                draw_list->AddRectFilled(ImVec2(notifyX, notifyY), ImVec2(notifyX + notifySize.x + 20, notifyY + 30), ImColor(30, 30, 30, (int)(lastNotification.alpha * 200)), 6.0f);
                draw_list->AddText(ImVec2(notifyX + 10, notifyY + 7), ImColor(255, 255, 255, (int)(lastNotification.alpha * 255)), lastNotification.message.c_str());
            }
            else {
                notifications.clear(); // 📌 Bildirim süresi dolunca temizle
            }
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
}



void GUI::updateRainbowEffects()
{
    if (GUI::chinaRainbowEnabled)
    {
        float time = ImGui::GetTime() * GUI::chinaRainbowSpeed;
        float hue = fmodf(time / 10.0f, 1.0f);
        float r, g, b;
        ImGui::ColorConvertHSVtoRGB(hue, 1.0f, 1.0f, r, g, b);
        GUI::chinaR = r * 255.0f;
        GUI::chinaG = g * 255.0f;
        GUI::chinaB = b * 255.0f;
    }
    if (GUI::espRainbow)
    {
        float time = ImGui::GetTime() * GUI::espRainbowSpeed;
        float hue = fmodf(time / 10.0f, 1.0f);
        float r, g, b;
        ImGui::ColorConvertHSVtoRGB(hue, 1.0f, 1.0f, r, g, b);
        GUI::espR = r * 255.0f;
        GUI::espG = g * 255.0f;
        GUI::espB = b * 255.0f;
    }
    if (GUI::tracerRainbow)
    {
        float time = ImGui::GetTime() * GUI::tracerRainbowSpeed;
        float hue = fmodf(time / 10.0f, 1.0f);
        float r, g, b;
        ImGui::ColorConvertHSVtoRGB(hue, 1.0f, 1.0f, r, g, b);
        GUI::tracerColorR = r * 255.0f;
        GUI::tracerColorG = g * 255.0f;
        GUI::tracerColorB = b * 255.0f;
    }
}


bool GUI::getIsInit()
{
    return is_init;
}

bool GUI::getDoDraw()
{
    return do_draw;
}

void GUI::setDoDraw(bool new_value)
{
    do_draw = new_value;
}

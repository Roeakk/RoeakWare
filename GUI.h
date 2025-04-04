#ifndef GUI_H
#define GUI_H

#include <cstdio>
#include <Windows.h>
#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_win32.h>
#include <ImGui/imgui_impl_opengl3.h>
#include <unordered_map>
#include <string>
#include <iostream>
#include <GL/gl.h>
#include <chrono>
#include <cmath> 

class GUI
{
public:
    static bool init(HWND wnd_handle);
    static void shutdown();
    static void applyCustomTheme();
    static void updateRainbowColors();

    static void draw();
    static void setupImGuý();
    

    static void updateRainbowEffects();
    static void drawHud();
    static void hud(const std::string& featureName, bool featureEnabled, float& yOffset);

    static bool getIsInit();
    static bool getDoDraw();
    static void setDoDraw(bool new_value);
    static int currentBindKey;

    static float chinaR;
    static float chinaG;
    static float chinaB;

    static bool espRainbow;
    static float espModeSelected;
    static float espRainbowSpeed;
    static float espR;
    static float espG;
    static float espB;

    static bool chinaRainbowEnabled;
    static float chinaRainbowSpeed;

    


    static float chamsR;
    static float chamsG;
    static float chamsB;
    static bool chamsRainbowEnabled;
    static float chamsRainbowSpeed;

    static bool tracerRainbow;
    static float tracerColorR;
    static float tracerColorG;
    static float tracerColorB;
    static float tracerOpacity;
    static float tracerColorThickness;
    static float tracerRainbowSpeed;

    static float targetespColorR;
    static float targetespColorG;
    static float targetespColorB;

    static float playerx;
    static float playery;
    static float playerz;

    static float itemx;
    static float itemy;
    static float itemz;
    static float itemR, itemG, itemB;


    static float tagsscale;



    static float chestR, chestG, chestB;           // Küçük sandýk ESP rengi
    static float largeChestR, largeChestG, largeChestB;  // Büyük sandýk ESP rengi

    static float triggerBotRange;

    static float aimbotRange;
    static float aimbotSpeed;

    static float triggetbotmincps;
    static float triggetbotmaxcps;
    static bool triggerToggle;

    static float leftclickermincps;
    static float rightclickermincps;
    static float leftclickermaxcps;
    static float rightclickermaxcps;

    static bool itemespEnabled;
    static bool itemScaleEnabled;
    static bool chestespEnabled;
    static bool wallHackEnabled;
    static bool playerwallhackEnabled;
    static bool itemwallhackEnabled;
    static bool chestwallhackEnabled;
    static bool nameTagsEnabled;
    static bool xrayEnabled;
    static bool esp2dEnabled;
    static bool tracersEnabled;
    static bool nametagsEnabled;
    static bool chinaHatEnabled;
    static bool triggerbotEnabled;
    static bool playerScaleEnabled;
    static bool aimbotEnabled;
    static bool aimCircleEnabled;
    static bool aimOnlyClick;
    static bool leftclickEnabled;
    static bool rightclickEnabled;
    static bool chamsEnabled;
    static bool chamsPlayer;
    static bool chamsItem;
    static bool chamsChest;

    static std::unordered_map<int, bool*> keyBinds;
    static bool isBindingKey;
    static bool* currentModuleToggle;
    static std::string getBindButtonText(bool* toggleFlag);
    static void startKeyBinding(bool* moduleToggle);
    static void bindKey(int key);


private:
    static void handleKeyBinds();
    static void toggleVisibility();
    static void addNotification(const std::string& message); // static olarak deðiþtirildi
};

extern bool isESPEnabled;
extern bool isAimEnabled;
extern float aimdistance;
extern float entitySize;
extern const float baseSize;


#endif

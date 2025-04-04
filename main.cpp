#include "main.h"
#include <windows.h>
#include <cmath>
#include <GL/gl.h>
#include <GL/glu.h>
#include <fstream>
#include <sstream>
#include <thread>
#include <iostream>
#include <atomic>
#include <chrono>
#pragma comment(lib, "OpenGL32.lib")



std::chrono::steady_clock::time_point lastLeftTriggerTime;
std::random_device rdtrigger;
std::mt19937 gentrigger(rdtrigger());






void trigger()
{
    HWND HandleWindow = GetForegroundWindow();

    RECT rect;
    GetClientRect(HandleWindow, &rect);
    int screenWidth = rect.right - rect.left;
    int screenHeight = rect.bottom - rect.top;

    int centerX = screenWidth / 2;
    int centerY = screenHeight / 2;

    POINT pos_cursor;
    GetCursorPos(&pos_cursor);

    auto now = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastLeftTriggerTime).count();

    int clickInterval = 71; // 1000 / 14 ? 71ms

    if (duration < clickInterval) return;

    SendMessage(HandleWindow, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(pos_cursor.x, pos_cursor.y));
    SendMessage(HandleWindow, WM_LBUTTONUP, 0, MAKELPARAM(pos_cursor.x, pos_cursor.y));

    lastLeftTriggerTime = now;
}




void drawBox(glm::vec4 color)
{
    glColor4fv(glm::value_ptr(color));

    glBegin(GL_LINES);
    {
        glVertex3f(1.0f, -1.0f, 1.0f);
        glVertex3f(1.0f, 1.0f, 1.0f);

        glVertex3f(-1.0f, -1.0f, 1.0f);
        glVertex3f(1.0f, -1.0f, 1.0f);

        glVertex3f(-1.0f, -1.0f, 1.0f);
        glVertex3f(-1.0f, 1.0f, 1.0f);

        glVertex3f(-1.0f, 1.0f, 1.0f);
        glVertex3f(1.0f, 1.0f, 1.0f);

        glVertex3f(1.0f, 1.0f, 1.0f);
        glVertex3f(1.0f, 1.0f, -1.0f);

        glVertex3f(1.0f, 1.0f, -1.0f);
        glVertex3f(1.0f, -1.0f, -1.0f);

        glVertex3f(1.0f, 1.0f, -1.0f);
        glVertex3f(-1.0f, 1.0f, -1.0f);

        glVertex3f(-1.0f, 1.0f, -1.0f);
        glVertex3f(-1.0f, 1.0f, 1.0f);

        glVertex3f(-1.0f, 1.0f, -1.0f);
        glVertex3f(-1.0f, -1.0f, -1.0f);

        glVertex3f(-1.0f, -1.0f, -1.0f);
        glVertex3f(1.0f, -1.0f, -1.0f);

        glVertex3f(-1.0f, -1.0f, -1.0f);
        glVertex3f(-1.0f, -1.0f, 1.0f);

        glVertex3f(1.0f, -1.0f, -1.0f);
        glVertex3f(1.0f, -1.0f, 1.0f);
    }
    glEnd();
}

void drawVisual(const glm::vec3& entityPosition, const glm::mat4& projection, const glm::mat4& modelview, Position& position, std::string tur)
{
    if (GUI::esp2dEnabled)
    {
        if (GUI::espModeSelected == 0)
        {
            glm::vec4 color = glm::vec4(GUI::espR, GUI::espG, GUI::espB, 1.0f);
            drawBox(color);  // `drawBox` fonksiyonunu çaðýrarak kutuyu çiziyoruz
        }
        else
        {
            glm::vec4 entityPos4D = glm::vec4(entityPosition, 1.0f);
            glm::vec4 projectedPos = projection * modelview * entityPos4D;

            if (projectedPos.w != 0.0f)
            {
                projectedPos.x /= projectedPos.w;
                projectedPos.y /= projectedPos.w;
            }

            float screenX = (projectedPos.x * 0.5f + 0.5f) * screenWidth;
            float screenY = (1.0f - (projectedPos.y * 0.5f + 0.5f)) * screenHeight;

            float entityDistance = glm::length(glm::vec3(projectedPos));

            // Kutunun boyutlarýný büyütmek için oraný arttýrýyoruz
            float dynamicBoxHeight = (baseSize / entityDistance) * 1.2f;  // %50 daha büyük
            float dynamicBoxWidth = (dynamicBoxHeight / 2.0f) * 1.2f;    // Ayný oranda geniþletiyoruz

            glMatrixMode(GL_PROJECTION);
            glPushMatrix();
            glLoadIdentity();
            glOrtho(0, screenWidth, screenHeight, 0, -1, 1);

            glMatrixMode(GL_MODELVIEW);
            glPushMatrix();
            glLoadIdentity();

            // Fosforlu kýrmýzý renk
            glColor3f(1.0f, 0.0f, 0.0f);  // Kýrmýzý renk

            glBegin(GL_LINE_LOOP);
            glVertex2f(screenX - dynamicBoxWidth / 2 - 1, screenY - dynamicBoxHeight / 2 - 1);
            glVertex2f(screenX + dynamicBoxWidth / 2 + 1, screenY - dynamicBoxHeight / 2 - 1);
            glVertex2f(screenX + dynamicBoxWidth / 2 + 1, screenY + dynamicBoxHeight / 2 + 1);
            glVertex2f(screenX - dynamicBoxWidth / 2 - 1, screenY + dynamicBoxHeight / 2 + 1);
            glEnd();

            SetColorESP(GUI::espR, GUI::espG, GUI::espB);

            glBegin(GL_LINE_LOOP);
            glVertex2f(screenX - dynamicBoxWidth / 2, screenY - dynamicBoxHeight / 2);
            glVertex2f(screenX + dynamicBoxWidth / 2, screenY - dynamicBoxHeight / 2);
            glVertex2f(screenX + dynamicBoxWidth / 2, screenY + dynamicBoxHeight / 2);
            glVertex2f(screenX - dynamicBoxWidth / 2, screenY + dynamicBoxHeight / 2);
            glEnd();

            glPopMatrix();
            glMatrixMode(GL_PROJECTION);
            glPopMatrix();

            if (GUI::tracersEnabled) {
                glMatrixMode(GL_MODELVIEW);
                glPushMatrix();
                glLoadIdentity();

                glEnable(GL_LINE_SMOOTH);
                glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

                SetColorTracers(GUI::tracerColorR, GUI::tracerColorG, GUI::tracerColorB, GUI::tracerOpacity);
                glLineWidth(GUI::tracerColorThickness);

                glBegin(GL_LINES);
                for (int i = 0; i < position.modelview.size(); i += 16) {
                    glVertex3f(0, 0, -0.1F);
                    glVertex3f((position.modelview.data() + i)[12], (position.modelview.data() + i)[13], (position.modelview.data() + i)[14]);
                }
                glEnd();

                glDisable(GL_LINE_SMOOTH);
                glPopMatrix();

                position.projection.clear();
                position.modelview.clear();
            }
        }
    }
    position.projection.clear();
    position.modelview.clear();
}





bool isMenuOpen() {
    POINT cursorPos;
    GetCursorPos(&cursorPos);

    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    int screenCenterX = screenWidth / 2;
    int screenCenterY = screenHeight / 2;

    const int tolerance = 10; 

    if (std::fabs(cursorPos.x - screenCenterX) <= tolerance && std::fabs(cursorPos.y - screenCenterY) <= tolerance) {
        return false; 
    }
    else {
        return true;
    }
}

void WINAPI hk_glOrtho(double left, double right, double bottom, double top, double zNear, double zFar)
{
    if (!GUI::getDoDraw()) {
        AutoClicker::Update();
    }

    
        
    if (zNear == 1000.0 && zFar == 3000.0)
    {
        bool sInventory = true;
        glPushAttrib(GL_ALL_ATTRIB_BITS);
        glPushMatrix();

        glDisable(GL_TEXTURE_2D);
        glDisable(GL_CULL_FACE);
        glDisable(GL_LIGHTING);
        glDisable(GL_DEPTH_TEST);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        GLint viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);
        int width = viewport[2];
        int height = viewport[3];

        static auto startTime = std::chrono::high_resolution_clock::now();

       



        for (auto& object : objects)
        {
            if (object.entityDistance > GUI::aimbotRange) continue;

            

            glm::mat4& modelview = object.m_modelview;
            glm::mat4& projection = object.m_projection;

            glm::vec3 entityPosition(0.0f, 0.0f, 0.0f);

            glm::vec4 entityPos4D = glm::vec4(entityPosition, 1.0f);
            glm::vec4 projectedPos = projection * modelview * entityPos4D;

            if (projectedPos.w != 0.0f)
            {
                projectedPos.x /= projectedPos.w;
                projectedPos.y /= projectedPos.w;
            }

            float distance = glm::length(glm::vec3(projectedPos));
            object.entityDistance = distance;

            glm::vec4 color = glm::vec4(1.0f);
            glm::vec3 translate = glm::vec3(1.0f);
            glm::vec3 scale = glm::vec3(1.0f);


            switch (object.m_type)
            {
            case Object::Entity:
                color = glm::vec4(0.18f, 1.0f, 0.52f, 1.0f);
                translate = glm::vec3(0.0f, -1.0f, 0.0f);
                scale = glm::vec3(0.5f, 1.0f, 0.5f);
                modelview = glm::translate(modelview, translate);
                modelview = glm::scale(modelview, scale);
                drawVisual(entityPosition, projection, modelview, entity, "entity");

                break;
            case Object::Chest:
                color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
                translate = glm::vec3(0.5f, 0.5f, 0.5f);
                scale = glm::vec3(0.5f, 0.5f, 0.5f);
                
                
   

            case Object::LargeChest:
                color = glm::vec4(0.91f, 0.87f, 0.42f, 1.0f);
                translate = glm::vec3(1.0f, 0.5f, 0.5f);
                scale = glm::vec3(1.0f, 0.5f, 0.5f);
                
                
            

            case Object::Item:
                color = glm::vec4(0.91f, 0.87f, 0.42f, 1.0f);
                translate = glm::vec3(1.0f, 0.5f, 0.5f);
                scale = glm::vec3(1.0f, 0.5f, 0.5f);

                
                break;
            

            }

         

            glMatrixMode(GL_PROJECTION);
            glLoadMatrixf(glm::value_ptr(projection));

            glMatrixMode(GL_MODELVIEW);
            glLoadMatrixf(glm::value_ptr(modelview));

            

            glm::vec4 corners[8];
            glm::vec3 halfScale = scale * 0.5f;
            glm::vec3 positions[8] = {
                glm::vec3(-halfScale.x, -halfScale.y, -halfScale.z),
                glm::vec3(halfScale.x, -halfScale.y, -halfScale.z),
                glm::vec3(-halfScale.x,  halfScale.y, -halfScale.z),
                glm::vec3(halfScale.x,  halfScale.y, -halfScale.z),
                glm::vec3(-halfScale.x, -halfScale.y,  halfScale.z),
                glm::vec3(halfScale.x, -halfScale.y,  halfScale.z),
                glm::vec3(-halfScale.x,  halfScale.y,  halfScale.z),
                glm::vec3(halfScale.x,  halfScale.y,  halfScale.z)
            };

            for (int i = 0; i < 8; ++i)
                corners[i] = modelview * glm::vec4(positions[i], 1.0f);

            float minX = corners[0].x, maxX = corners[0].x;
            float minY = corners[0].y, maxY = corners[0].y;

            for (int i = 1; i < 8; ++i)
            {
                if (corners[i].x < minX) minX = corners[i].x;
                if (corners[i].x > maxX) maxX = corners[i].x;
                if (corners[i].y < minY) minY = corners[i].y;
                if (corners[i].y > maxY) maxY = corners[i].y;
            }

            float centerX = (minX + maxX) * 0.5f;
            float centerY = (minY + maxY) * 0.5f;

            int screenX = (int)((centerX + 1.0f) / 2.0f * width);
            int screenY = (int)((1.0f - (centerY + 1.0f) / 2.0f) * height);

            if (object.m_type != Object::Entity) continue;
            if (GUI::aimbotEnabled && (!GUI::aimOnlyClick || (GetAsyncKeyState(VK_LBUTTON) & 0x8000))) {
                if (GUI::aimCircleEnabled) {
                    if (distance < GUI::aimbotRange) {
                        float radius = 0.75f;
                        float speed = 1.0f;

                        auto currentTimePoint = std::chrono::high_resolution_clock::now();
                        std::chrono::duration<float> elapsed = currentTimePoint - startTime;
                        float currentTime = elapsed.count();

                        float offsetY = sin(currentTime * speed) * 0.5f;

                        glm::vec3 circlePosition = entityPosition + glm::vec3(0.0f, offsetY, 0.0f);

                        glMatrixMode(GL_PROJECTION);
                        glPushMatrix();
                        glLoadMatrixf(&projection[0][0]);

                        glMatrixMode(GL_MODELVIEW);
                        glPushMatrix();
                        glLoadMatrixf(&modelview[0][0]);

                        SetColorESP(GUI::targetespColorR, GUI::targetespColorG, GUI::targetespColorB);

                        glBegin(GL_LINE_LOOP);
                        for (int i = 0; i <= 360; i += 10) {
                            float theta = glm::radians(float(i));
                            float x = radius * cosf(theta);
                            float z = radius * sinf(theta);
                            glVertex3f(circlePosition.x + x, circlePosition.y, circlePosition.z + z);
                        }
                        glEnd();

                        glPopMatrix();
                        glMatrixMode(GL_PROJECTION);
                        glPopMatrix();
                    }
                }

                
                if (isInFOV(screenX, screenY, std::min(width, height) * std::tan(HALF_FOV_RADIANS), width / 2.0f, height / 2.0f) && distance < GUI::aimbotRange &&
                    !isMenuOpen()) {
                    const float sensitivity = GUI::aimbotSpeed;
                    POINT currentPos;
                    GetCursorPos(&currentPos);
                    int currentX = currentPos.x;
                    int currentY = currentPos.y;

                    int deltaX = (int)((screenX - currentX) * sensitivity);
                    int deltaY = (int)((screenY - currentY) * sensitivity);

                    if (!GUI::getDoDraw()) {
                        SetCursorPos(currentX + deltaX, currentY + deltaY);
                    }
                }

                
            }

            if (GUI::triggerbotEnabled) {
                if (isInFOV(screenX, screenY, std::min(width, height) * std::tan(HALF_FOV_RADIANS), width / 2.0f, height / 2.0f) && distance < GUI::triggerBotRange &&
                    !isMenuOpen()) {
                    trigger();
                }
            }

            

        }

        objects.clear();

        glPopAttrib();
        glPopMatrix();

        
    }

    fn_glOrtho(left, right, bottom, top, zNear, zFar);
}

void savePosition(Position& position, float offsetX, float offsetY, float offsetZ)
{
    float projection[16]{};
    glGetFloatv(GL_PROJECTION_MATRIX, projection);

    float modelview[16]{};
    glGetFloatv(GL_MODELVIEW_MATRIX, modelview);

    float m3[4]{};

    for (int i = 0; i < 4; ++i)
    {
        m3[i] = modelview[i] * offsetX
            + modelview[i + 4] * offsetY
            + modelview[i + 8] * offsetZ
            + modelview[i + 12];
    }

    memcpy(modelview + 12, m3, sizeof(m3));

    position.projection.insert(position.projection.end(), begin(projection), end(projection));
    position.modelview.insert(position.modelview.end(), begin(modelview), end(modelview));


    
}



void WINAPI hk_glScaleColor(float x, float y, float z, float red, float green, float blue, float alpha) {
    bool disableDepth = false;

    if (x == 0.9375f && y == 0.9375f && z == 0.9375f) {
        if (GUI::playerwallhackEnabled && GUI::wallHackEnabled) {
            disableDepth = true;
        }
        if (GUI::chamsPlayer) {
            red = 200.0f;
         
        }

        if (GUI::playerScaleEnabled) {
            x *= GUI::playerx;
            y *= GUI::playery;
            z *= GUI::playerz;
        }
        else {
            x *= 1.0f;
            y *= 1.0f;
            z *= 1.0f;
        }

    
    }
    if ((x == 0.5f && y == 0.5f && z == 0.5f) || (x == 0.25f && y == 0.25f && z == 0.25f)) {
        if (GUI::itemwallhackEnabled && GUI::wallHackEnabled) {
            disableDepth = true;
        }
        if (GUI::chamsItem) {
            red = 200.0f;
        }
        if (GUI::itemScaleEnabled) {
            x *= GUI::itemx;
            y *= GUI::itemy;
            z *= GUI::itemz;
        }
        else {
            x *= 1.0f;
            y *= 1.0f;
            z *= 1.0f;
        }
    }

    if (disableDepth) {
        glDisable(GL_DEPTH_TEST);
    }
    else {
        glEnable(GL_DEPTH_TEST);
    }

    fn_glColor4f(red, green, blue, alpha);
    fn_glScalef(x, y, z);
}


void WINAPI hk_glScalef(float x, float y, float z) {

    
    if (x == 0.9375f && y == 0.9375f && z == 0.9375f) {
   
            savePosition(entity, 0.0F, -1.0F, 0.0F);
            objects.push_back(Object::Entity);
        
        
    }
    if (!sInventory) {
        if (x == 0.5f && y == 0.5f && z == 0.5f) {
            objects.push_back(Object::Item);
         }
        
    }
    
    hk_glScaleColor(x, y, z, 1.0f, 1.0f, 1.0f, 1.0f);
}

void WINAPI hk_glDrawElements(GLenum mode, GLsizei count, GLenum type, const void* indices) {

    
    fn_glDrawElements(mode, count, type, indices); 
}

void WINAPI hk_glColor4f(float red, float green, float blue, float alpha) {

    
   
    

    hk_glScaleColor(1.0f, 1.0f, 1.0f, red, green, blue, alpha);
}



BOOL __stdcall mySwapBuffers(HDC hDC)
{
    static DWORD windowThread = 0;

    if (!windowThread)
    {
        HWND window = WindowFromDC(hDC);
        windowThread = GetWindowThreadProcessId(window, nullptr);

        if (!windowThread)
        {
            return fn_pSwapBuffers(hDC);
        }
    }

    glDepthFunc(GL_LEQUAL);

    if (GUI::xrayEnabled)
    {
        glDepthFunc(GL_ALWAYS);
    }
    
    return fn_pSwapBuffers(hDC);
}

void WINAPI hk_glTranslatef(float xt, float yt, float zt){
    

     
        if (xt == 0.5f and yt == 0.4375f and zt == 0.9375f) {
            
            objects.push_back(Object::Chest);
        }
        
            

 
        if (xt == 1.0f and yt == 0.4375f and zt == 0.9375f) {
            objects.push_back(Object::LargeChest);

            
        }
        
           


    
    

    fn_glTranslatef(xt, yt, zt);
}

void __stdcall MainThread(HINSTANCE instance)
{
    if (Hook::init())
        goto _shutdown;

    while (!GetAsyncKeyState(VK_END))
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(25));
    }

_shutdown:
    Hook::shutdown();
    FreeLibrary(instance);
}



bool __stdcall DllMain(HINSTANCE instance, DWORD reason, LPVOID p_reserved) {
    if (reason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(instance);
        AllocConsole();
        FILE* stream;
        freopen_s(&stream, "CONOUT$", "w", stdout);
        printf("Discord.gg/m8yDr5cq ");
        if (!stream) {
            MessageBox(nullptr, L"Failed to allocate console", L"Error", MB_OK | MB_ICONERROR);
            return FALSE;
        }

        if (MH_Initialize() != MH_OK) {
            std::cerr << "Failed to initialize MinHook." << std::endl;
            return FALSE;
        }

        if (MH_CreateHook(reinterpret_cast<void*>(glOrtho), hk_glOrtho, reinterpret_cast<void**>(&fn_glOrtho)) != MH_OK ||
            MH_CreateHook(reinterpret_cast<void*>(glScalef), hk_glScalef, reinterpret_cast<void**>(&fn_glScalef)) != MH_OK ||
            MH_CreateHook(reinterpret_cast<void*>(glColor4f), hk_glColor4f, reinterpret_cast<void**>(&fn_glColor4f)) != MH_OK ||
            MH_CreateHook(reinterpret_cast<void*>(glDrawElements), hk_glDrawElements, reinterpret_cast<void**>(&fn_glDrawElements)) != MH_OK ||
            MH_CreateHook(reinterpret_cast<void*>(glTranslatef), hk_glTranslatef, reinterpret_cast<void**>(&fn_glTranslatef)) != MH_OK ||
            MH_CreateHook(reinterpret_cast<void*>(SwapBuffers), mySwapBuffers, reinterpret_cast<void**>(&fn_pSwapBuffers)) != MH_OK) {
            std::cerr << "Failed to create hooks." << std::endl;
            return FALSE;
        }

        if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK) {
            std::cerr << "Failed to enable hooks." << std::endl;
            return FALSE;
        }
        static std::thread main_thread([instance] { MainThread(instance); });
        if (main_thread.joinable())
            main_thread.detach();
    }
    else if (reason == DLL_PROCESS_DETACH) {
        FreeConsole();
        fclose(stdout);
    }

    return true;
}
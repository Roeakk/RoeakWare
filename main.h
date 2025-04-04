#pragma once

#include <fstream>
#include <thread>
#include <iostream>
#include <Windows.h>
#include <list>
#include <GL/GL.h>
#include <cmath>
#include <chrono>
#include "minhook/include/MinHook.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <mutex>
#include <shellapi.h>
#include <GL/glu.h>
#include <vector>
#include "Hook.h"
#include "GUI.h"
#include "Position.h"
#include "AutoClicker.h"

Position entity, nametags, chest, largechest, item;
struct Object
{
    enum Type
    {
        Entity,
        NameTags,
        Chest,
        Mob,
        Item,
        LargeChest
    };

    Type m_type;
    glm::vec3 size;
    glm::mat4 m_projection;
    glm::mat4 m_modelview;
    glm::vec3 m_position;
    float entityDistance;
    bool isActive;
    bool isVisible;
    

    Object(Type type, const glm::vec3& position = glm::vec3(0.0f), bool active = true, bool visible = true)
        : m_type{ type }, m_position{ position }, entityDistance{ 0.0f }, isActive{ active }, isVisible{ visible }
    {
        glGetFloatv(GL_PROJECTION_MATRIX, glm::value_ptr(m_projection));
        glGetFloatv(GL_MODELVIEW_MATRIX, glm::value_ptr(m_modelview));
    }
};

struct Entity {
    std::string name;
    
};



int red = 0;
int green = 0;
int blue = 0;
int alpha = 0;


using namespace std;

std::list<Object> objects;
decltype(&glOrtho) fn_glOrtho = glOrtho;
decltype(&glScalef) fn_glScalef = glScalef;
decltype(&glTranslatef) fn_glTranslatef = glTranslatef;
decltype(&glColor4f) fn_glColor4f = glColor4f;
decltype(&glDrawElements) fn_glDrawElements = glDrawElements;
decltype(&SwapBuffers) fn_pSwapBuffers = nullptr;


std::chrono::time_point<std::chrono::steady_clock> lastTime = std::chrono::steady_clock::now();

const float baseSize = 1000.0f;

int screenWidth = 1920;
int screenHeight = 1080;
bool sInventory = false;
const float PI = 3.14159265358979323846f;
const float HALF_FOV_DEGREES = 60.0f;
const float HALF_FOV_RADIANS = HALF_FOV_DEGREES * (PI / 180.0f);
const float MAX_DISTANCE = 5.0f;

bool isInFOV(float x, float y, float radius, float centerX, float centerY) {
    float dx = x - centerX;
    float dy = y - centerY;
    float distance = sqrt(dx * dx + dy * dy);
    return distance <= radius;
}

bool isWithinDistance(float distance, float maxDistance) {
    return distance <= maxDistance;
}

void SetConsoleColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

void SetColorESP(float r, float g, float b) {
    glColor3f(r / 255.0f, g / 255.0f, b / 255.0f);
}

void SetColorTracers(float r, float g, float b, float opacity)
{
    glColor4f(r / 255.0f, g / 255.0f, b / 255.0f, opacity / 255.0f);
}

void SetColorChina(float r, float g, float b)
{
    glColor4f(r / 255.0f, g / 255.0f, b / 255.0f, 0.4f);
}

void SetColorChams(float r, float g, float b) {
    glColor3f(r / 255.0f, g / 255.0f, b / 255.0f);
}

#include <random>
#include <thread>
#include "GUI.h"

struct AutoClicker
{
	inline static float leftMaxCps = GUI::leftclickermaxcps;
	inline static float leftMinCps = GUI::leftclickermincps;
	inline static float rightMinCps = GUI::rightclickermincps;
	inline static float rightMaxCps = GUI::rightclickermaxcps;

	static void Update();
	static void UpdateLeft();
	static void UpdateRight();
};
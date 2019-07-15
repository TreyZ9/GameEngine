#pragma once

struct Config
{
	struct Display
	{
		static std::string TITLE;

		static int OPENGL_VERSION_MAJOR, OPENGL_VERSION_MINOR;
		static unsigned short int WIDTH;
		static unsigned short int HEIGHT;

		static int FPS_BUFFER_SIZE;
		static int FPS_CAP;

		static float NEAR_PLANE;
		static float FAR_PLANE;
		static float FOV;
	};

	struct Camera
	{
		static const float MOVEMENT_SPEED;
		static const float LOOK_SPEED;

		static float PITCH_MIN;
		static float PITCH_MAX;
	};
};

std::string Config::Display::TITLE("C++ OpenGL Game Engine");

unsigned short int Config::Display::WIDTH = 1280;
unsigned short int Config::Display::HEIGHT = 960;

int Config::Display::OPENGL_VERSION_MAJOR = 4;
int Config::Display::OPENGL_VERSION_MINOR = 0;

int Config::Display::FPS_BUFFER_SIZE = 20; // Set 1 for no buffer
int Config::Display::FPS_CAP = 120;

float Config::Display::NEAR_PLANE = 0.1f;
float Config::Display::FAR_PLANE = 1000.0f;
float Config::Display::FOV = 70.0f;


const float Config::Camera::MOVEMENT_SPEED = 40.0f;
const float Config::Camera::LOOK_SPEED = 20.0f;

float Config::Camera::PITCH_MIN = -90.0f;
float Config::Camera::PITCH_MAX = 90.0f;

#include "Config.h"

#include <iostream>

#include "INIReader.h"

void Config::loadConfigs(const std::string& filename)
{
	INIReader reader(filename);

	if (reader.ParseError() != 0)
	{

	}

	Config::Display::TITLE = reader.Get("Display", "Title", "Default");
	std::cout << Config::Display::TITLE << std::endl;
	
	std::string openGLVersion = reader.Get("Display", "OpenGLVersion", "Default");
	Config::Display::OPENGL_VERSION_MAJOR = std::stoi(openGLVersion.substr(0, openGLVersion.find('.')));
	Config::Display::OPENGL_VERSION_MINOR = std::stoi(openGLVersion.substr(openGLVersion.find('.') + 1));
	std::cout << Config::Display::OPENGL_VERSION_MAJOR << '.' << Config::Display::OPENGL_VERSION_MINOR << std::endl;

	std::string resolution = reader.Get("Display", "Resolution", "Default");
	Config::Display::WIDTH = std::stoi(resolution.substr(0, resolution.find('x')));
	Config::Display::HEIGHT = std::stoi(resolution.substr(resolution.find('x') + 1));
	std::cout << Config::Display::WIDTH << 'x' << Config::Display::HEIGHT << std::endl;

	Config::Display::FPS_BUFFER_SIZE = reader.GetInteger("Display", "FpsBufferSize", 0);

	Config::Display::FPS_CAP = reader.GetInteger("Display", "FpsCap", 0);

	Config::Display::NEAR_PLANE = reader.GetFloat("Display", "NearPlane", 0.0f);

	Config::Display::FAR_PLANE = reader.GetFloat("Display", "FarPlane", 0.0f);

	Config::Display::FOV = reader.GetFloat("Display", "FieldOfView", 0.0f);

	Config::Camera::MOVEMENT_SPEED = reader.GetFloat("Camera", "MovementSpeed", 0.0f);

	Config::Camera::LOOK_SPEED = reader.GetFloat("Camera", "MouseSensitivity", 0.0f);

	Config::Camera::PITCH_MIN = reader.GetFloat("Camera", "PitchMin", 0.0f);

	Config::Camera::PITCH_MAX = reader.GetFloat("Camera", "PitchMax", 0.0f);
}

std::string Config::Display::TITLE;

unsigned short int Config::Display::WIDTH;
unsigned short int Config::Display::HEIGHT;

int Config::Display::OPENGL_VERSION_MAJOR;
int Config::Display::OPENGL_VERSION_MINOR;

int Config::Display::FPS_BUFFER_SIZE;
int Config::Display::FPS_CAP;

float Config::Display::NEAR_PLANE;
float Config::Display::FAR_PLANE;
float Config::Display::FOV;

float Config::Camera::MOVEMENT_SPEED;
float Config::Camera::LOOK_SPEED;

float Config::Camera::PITCH_MIN;
float Config::Camera::PITCH_MAX;

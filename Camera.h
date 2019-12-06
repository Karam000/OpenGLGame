#include <iostream>
#include <gl/glew/glew.h>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include<gl/glm/glm.hpp>
#include<gl/glm/gtc/type_ptr.hpp>
#include<gl/glm/gtx/transform.hpp>

using namespace glm;

#pragma once
class Camera
{
private:
	vec3 Eye;
	vec3 lookVec;
	vec3 Up;

	vec3 forwardDir;
	vec3 rightDir;
	
	vec3 horizForward;
	vec3 camEye;
	vec3 camLook;

	vec4 lookDummy;

public:
	Camera() :Eye{vec3(0,0,3)}, lookVec{vec3(0,0,-2)}, Up{vec3(0,1,0)}, forwardDir{vec3(0, 0, -1)}, lookDummy{vec4(0,0,-1,0)}
	{
		camEye = Eye;
		rightDir = cross(normalize(forwardDir), normalize(Up));
		horizForward = cross(normalize(Up), normalize(rightDir));
	}

	void SetEye(vec3 );
	void SetLookVec(vec3 );
	
	vec3 GetCamEye();
	vec3 GetCamLook();
	vec3 GetUp();

	vec3 GetrightDir();
	vec3 Gethorzfor();

	void UpdateDirections();
	vec3 MouseFollow(vec3, vec3);

};


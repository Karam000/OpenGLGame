/////////////////////////////////////////////////////
////Textures //////
/////////////////////////////////////////////////////


#include <iostream>
#include <gl/glew/glew.h>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include<gl\glm\glm.hpp>
#include<gl\glm\gtc\type_ptr.hpp>
#include<gl\glm\gtx\transform.hpp>
#include<vector>
#include "Camera.h"

using namespace std;
using namespace glm;
using namespace sf;

struct vertex
{
	vec3 position;
	vec3 normal;
	vec2 uv;
	vertex() {}
	vertex(vec3 _position, vec3 _norm) :position{ _position }, normal{ _norm }
	{
	}
	vertex(vec3 _position) :position{ _position }, normal(0, 0, 0)
	{
	}
	vertex(vec3 _position, vec3 _norm, vec2 _uv) :position{ _position }, normal{ _norm }, uv{ _uv }
	{
	}
};

vertex plan[] =
{

	{vec3(20, -5,-20),vec3(0,-1,0)},
	{vec3(20, -5,20), vec3(1,-1,0)},
	{vec3(-20,-5,20),  vec3(0,-1,1)},
	{vec3(-20,-5,-20), vec3(0,1,0)}

};

const GLint WIDTH = 600, HEIGHT = 600;
GLuint VAO, CVBO, TVBO, SVBO, PVBO, programId, IBO, TextureId, TexturePlan,TextureFlower;
GLuint modelMatLoc, viewMatLoc, projMatLoc;
GLuint InitShader(const char* vertex_shader_file_name, const char* fragment_shader_file_name);

Camera cam;

vec3 camEye = cam.GetCamEye();
vec3 camLook = cam.GetCamLook();
vec3 Up = cam.GetUp();

vec3 rightDir = cam.GetrightDir();
vec3 horizForward = cam.Gethorzfor();



vertex cube_core_vertices[] = {
{vec3(-0.5, 0.5, 0.5),vec3(1,0,0),vec2(0,1)},
{vec3(-0.5, -0.5, 0.5),vec3(0,1,0),vec2(0,0) },
{vec3(0.5 , -0.5, 0.5),vec3(1,0,1),vec2(1,0)},
{vec3(0.5 , 0.5, 0.5),vec3(0,0,1),vec2(1,1)},
{vec3(0.5 , 0.5, -0.5),vec3(1,1,0)},
{vec3(0.5 , -0.5, -0.5),vec3(0,1,1)},
{vec3(-0.5, -0.5, -0.5),vec3(1,1,1)},
{vec3(-0.5, 0.5, -0.5),vec3(0,0,0)}
};


#pragma Region Sphere
vector<vertex> sphere_vertices;
vec3 Sphere_Core_vertices[4] = {
	vec3(0.0, 0.0, 1.0),
	vec3(0.0, 0.942809, -0.333333),
	vec3(-0.816497, -0.471405, -0.333333),
	vec3(0.816497, -0.471405, -0.333333)
};
void Triangle(vec3 a, vec3 b, vec3 c)
{
	// flat shading
	vec3 normal = cross((c - b), (a - b));
	sphere_vertices.push_back(vertex(a, a));
	sphere_vertices.push_back(vertex(b, b));
	sphere_vertices.push_back(vertex(c, c));
}
void dividTriangle(vec3 a, vec3 b, vec3 c, int itertions)
{
	if (itertions > 0)
	{
		vec3 v1 = normalize(a + b);
		vec3 v2 = normalize(a + c);
		vec3 v3 = normalize(b + c);

		dividTriangle(a, v1, v2, itertions - 1);
		dividTriangle(v1, b, v3, itertions - 1);
		dividTriangle(v1, v3, v2, itertions - 1);
		dividTriangle(v2, v3, c, itertions - 1);
	}
	else
	{
		Triangle(a, b, c);
	}
}
void CreateSphere(int iterations)
{
	sphere_vertices.clear();
	dividTriangle(Sphere_Core_vertices[0], Sphere_Core_vertices[1], Sphere_Core_vertices[2], iterations);
	dividTriangle(Sphere_Core_vertices[0], Sphere_Core_vertices[3], Sphere_Core_vertices[1], iterations);
	dividTriangle(Sphere_Core_vertices[0], Sphere_Core_vertices[2], Sphere_Core_vertices[3], iterations);
	dividTriangle(Sphere_Core_vertices[3], Sphere_Core_vertices[2], Sphere_Core_vertices[1], iterations);


	glGenBuffers(1, &SVBO);
	glBindBuffer(GL_ARRAY_BUFFER, SVBO);
	glBufferData(GL_ARRAY_BUFFER, sphere_vertices.size() * sizeof(vertex), sphere_vertices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, false, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, false, 6 * sizeof(float), (char*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}
void BindSphere()
{
	glBindBuffer(GL_ARRAY_BUFFER, SVBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, false, 6 * sizeof(float), (char*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}
#pragma endregion 
void CreateCube()
{
	int vertices_Indeces[] = {
		//front
		0,
		1,
		2,

		0,
		2,
		3,
		//Right
		3,
		2,
		5,

		3,
		5,
		4,
		//Back
		4,
		5,
		6,

		4,
		6,
		7,
		//Left
		7,
		6,
		1,

		7,
		1,
		0,
		//Top
		7,
		0,
		3,

		7,
		3,
		4,
		//Bottom
		2,
		1,
		6,

		2,
		6,
		5
	};
	// create VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// create VBO6
	glGenBuffers(1, &CVBO);
	glBindBuffer(GL_ARRAY_BUFFER, CVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_core_vertices), cube_core_vertices, GL_STATIC_DRAW);

	// Index Buffer
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertices_Indeces), vertices_Indeces, GL_STATIC_DRAW);

	// shader
	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(vertex), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(vertex), (char*)(sizeof(vec3)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(vertex), (char*)(2 * sizeof(vec3)));
	glEnableVertexAttribArray(2);
}

void BindCube()
{
	glBindBuffer(GL_ARRAY_BUFFER, CVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(vertex), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(vertex), (char*)(sizeof(vec3)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(vertex), (char*)(2 * sizeof(vec3)));
	glEnableVertexAttribArray(2);
}

void CreatePlan()
{

	//create VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//create VBO
	glGenBuffers(1, &PVBO);
	glBindBuffer(GL_ARRAY_BUFFER, PVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(plan), plan, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(vertex), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(vertex), ((char*)(sizeof(vec3))));
	glEnableVertexAttribArray(1);

	//This in CPU so we need to link it to GPU using VAO & VBO
}

void BindPlan()
{
	glGenBuffers(1, &PVBO);
	glBindBuffer(GL_ARRAY_BUFFER, PVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(plan), plan, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(vertex), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(vertex), ((char*)(sizeof(vec3))));
	glEnableVertexAttribArray(1);

	//This in CPU so we need to link it to GPU using VAO & VBO
}

void CreateTriangle()
{
	//interleaved buffer
	vertex vertices[] =
	{
		{vec3(-1,-1,0),vec3(0,1,0) },
		{vec3(1,-1,0),vec3(0,0,1)},
		{vec3(0,1,0),vec3(1,0,0)}
	};

	// create VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// create VBO
	glGenBuffers(1, &TVBO);
	glBindBuffer(GL_ARRAY_BUFFER, TVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// shader
	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(vertex), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(vertex), ((char*)(sizeof(vec3))));
	glEnableVertexAttribArray(1);

}

void BindTriangle()
{
	glBindBuffer(GL_ARRAY_BUFFER, TVBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(vertex), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(vertex), ((char*)(sizeof(vec3))));
	glEnableVertexAttribArray(1);
}

void CompileShader()
{
	//programId = InitShader("UnlitVS.glsl", "UnlitFS.glsl");
	programId = InitShader("VS.glsl", "FS.glsl");
	glUseProgram(programId);
}

int Init()
{
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		cout << "Error";
		getchar();
		return 1;
	}
	else
	{
		if (GLEW_VERSION_3_0)
			cout << "Driver support OpenGL 3.0\nDetails:\n";
	}
	cout << "\tUsing glew " << glewGetString(GLEW_VERSION) << endl;
	cout << "\tVendor: " << glGetString(GL_VENDOR) << endl;
	cout << "\tRenderer: " << glGetString(GL_RENDERER) << endl;
	cout << "\tVersion: " << glGetString(GL_VERSION) << endl;
	cout << "\tGLSL:" << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;

	CompileShader();
	CreateCube();
	CreateSphere(5);
	CreateTriangle();

	modelMatLoc = glGetUniformLocation(programId, "modelMat");
	viewMatLoc = glGetUniformLocation(programId, "viewMat");
	projMatLoc = glGetUniformLocation(programId, "projMat");

	//glm::mat4 viewMat = glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	/*glm::mat4 viewMat = glm::lookAt(camEye,camLook,Up);

	glUniformMatrix4fv(viewMatLoc, 1, GL_FALSE, glm::value_ptr(viewMat));

	glm::mat4 projMat = glm::perspectiveFov(60.0f, 5.0f, 5.0f, 1.0f, 10.0f);
	glUniformMatrix4fv(projMatLoc, 1, GL_FALSE, glm::value_ptr(projMat));*/


	sf::Image img;
	if (!img.loadFromFile("UV_Grid.jpg"))
		cout << "Error Loading Image";
	else
	{
		glGenTextures(1, &TextureId);
		//glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, TextureId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img.getSize().x, img.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, img.getPixelsPtr());

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	Image planeImg;
	if (!planeImg.loadFromFile("grass.jpg"))
		cout << "Error Loading Image";
	else
	{
		glGenTextures(1, &TexturePlan);
		//glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, TexturePlan);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, planeImg.getSize().x, planeImg.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, planeImg.getPixelsPtr());

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}


	Image flowerImg;
	if (!flowerImg.loadFromFile("fire.jpg"))
		cout << "Error Loading Image";
	else
	{
		glGenTextures(1, &TextureFlower);
		//glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, TextureFlower);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, flowerImg.getSize().x, flowerImg.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, flowerImg.getPixelsPtr());

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	glClearColor(0, 0, 0, 1);
	glEnable(GL_DEPTH_TEST);

	return 0;
}

float theta = 0;
float x = 0;
void Update()
{
	theta += 0.001f;
	x += 0.001f;

	cam.UpdateDirections();
	rightDir = cam.GetrightDir();
	horizForward = cam.Gethorzfor();

	camEye = cam.GetCamEye();
	camLook = cam.GetCamLook();
	Up = cam.GetUp();
}

void Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/*BindSphere();
	mat4 ModelMat = glm::translate(glm::vec3(0, 0, 0)) *
		glm::rotate(theta * 180 / 3.14f, glm::vec3(1, 1, 1)) *
		glm::scale(glm::vec3(1, 1, 1));
	glUniformMatrix4fv(modelMatLoc, 1, GL_FALSE, glm::value_ptr(ModelMat));
	glDrawArrays(GL_TRIANGLES, 0, sphere_vertices.size());*/

	glm::mat4 viewMat = glm::lookAt(camEye, camLook, Up);

	glUniformMatrix4fv(viewMatLoc, 1, GL_FALSE, glm::value_ptr(viewMat));

	glm::mat4 projMat = glm::perspectiveFov(60.0f, 5.0f, 5.0f, 1.0f, 10.0f);
	glUniformMatrix4fv(projMatLoc, 1, GL_FALSE, glm::value_ptr(projMat));

	GLint textureLocation = glGetUniformLocation(programId, "tex");

	BindCube();
	mat4 ModelMat2 = glm::translate(glm::vec3(x+1.f, 0, -0.8f)) *
		glm::rotate(0 * 180 / 3.14f, glm::vec3(1, 1, 1)) *
		glm::scale(glm::vec3(0.5, 0.5, 0.5));
	glUniformMatrix4fv(modelMatLoc, 1, GL_FALSE, glm::value_ptr(ModelMat2));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TextureId);
	glUniform1i(textureLocation, 0);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, NULL);

	BindCube();
	mat4 ModelMat = glm::translate(glm::vec3(-1.f, 0, -0.8f)) *
		glm::rotate(0 * 180 / 3.14f, glm::vec3(1, 1, 1)) *
		glm::scale(glm::vec3(0.5, 0.5, 0.5));
	glUniformMatrix4fv(modelMatLoc, 1, GL_FALSE, glm::value_ptr(ModelMat));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TextureId);
	glUniform1i(textureLocation, 0);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, NULL);

	BindPlan();
	mat4 ModelMat3 = glm::translate(glm::vec3(0, 0, 0)) *
		glm::rotate(0 * 180 / 3.14f, glm::vec3(1, 1, 1)) *
		glm::scale(glm::vec3(1, 1, 1));
	glUniformMatrix4fv(modelMatLoc, 1, GL_FALSE, glm::value_ptr(ModelMat3));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TexturePlan);
	glUniform1i(textureLocation, 0);
	
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);


}

int main()
{
	sf::ContextSettings context;
	context.depthBits = 24;
	sf::Window window(sf::VideoMode(WIDTH, HEIGHT), "SFML works!", sf::Style::Close, context);

	if (Init()) return 1;

	glm::vec3 mouseCam(Mouse::getPosition(window).x, Mouse::getPosition(window).y, camEye.z);
	glm::vec3 lastMouse = mouseCam;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{

			case sf::Event::Closed:
				window.close();
				break;

			case  sf::Event::MouseMoved:
				Vector2i mpos = Mouse::getPosition(window);
				vec3 vectorMouse = vec3(mpos.x, mpos.y, 0);
				vec3 mouseNow(Mouse::getPosition(window).x, Mouse::getPosition(window).y, camEye.z);

				camLook = cam.MouseFollow(lastMouse, mouseNow);

				lastMouse = mouseNow;

				break;
			}

			if (Keyboard::isKeyPressed(Keyboard::Up))
			{
				cam.Move('f');
			}
			if (Keyboard::isKeyPressed(Keyboard::Down))
			{
				cam.Move('b');
			}
			if (Keyboard::isKeyPressed(Keyboard::Left))
			{
				cam.Move('l');
			}
			if (Keyboard::isKeyPressed(Keyboard::Right))
			{
				cam.Move('r');
			}
		}

		Update();
		Render();

		window.display();
	}
	return 0;
}
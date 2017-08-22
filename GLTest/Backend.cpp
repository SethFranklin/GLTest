
#include <chrono>
#include <fstream>
#include <cmath>
#include <string>

#include <SDL.h>
#include <glew.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Backend.h"
#include "Read.h"
#include "Debug.h"
#include "Shader.h"

const char GameName[] = "OpenGl Testing - Seth Franklin";

bool IsRunning = false;

int FrameRate = 60;

SDL_DisplayMode MonitorInfo;

SDL_Window* Window;

SDL_GLContext GLContext;

void Run();
void StartProcedure();
void LoadShaders();
void ShutdownProcedure();
void GameLoop(float DeltaTime);
void PollEvent(SDL_Event* Event);

GLuint VertexArrayObject;
GLuint Texture1;
GLuint Texture2;

Shader* VertexColorShader;

void Backend::Start()
{

	StartProcedure();

	LoadShaders();

	Run();

	ShutdownProcedure();

}

void Backend::Shutdown()
{

	IsRunning = false;

}

void Run()
{

	IsRunning = true;

	auto LastFrameTime = std::chrono::high_resolution_clock::now();

	double TenToTheNine = 1000000000;

	std::chrono::duration<double> FrameTime = std::chrono::nanoseconds(long(TenToTheNine / float(FrameRate)));

	while (IsRunning)
	{

		auto CurrentFrameTime = std::chrono::high_resolution_clock::now();

		auto DeltaTime = CurrentFrameTime - LastFrameTime;

		if (DeltaTime >= FrameTime)
		{

			GameLoop(float(DeltaTime.count()) / float(TenToTheNine));

			LastFrameTime = std::chrono::high_resolution_clock::now();

		}

	}

}

void StartProcedure()
{

	Debug::Enable();

	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{

		Debug::Log("SDL_Init Error: " + std::string(SDL_GetError()));

		IsRunning = false;

	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);

	if (SDL_GetCurrentDisplayMode(0, &MonitorInfo) != 0)
	{

		Debug::Log("SDL_GetCurrentDisplayMode Error: " + std::string(SDL_GetError()));

		IsRunning = false;

	}

	if (MonitorInfo.refresh_rate != 0) FrameRate = MonitorInfo.refresh_rate;

	Window = SDL_CreateWindow(GameName, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, MonitorInfo.w, MonitorInfo.h, SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN);

	GLContext = SDL_GL_CreateContext(Window);

	glewExperimental = true;

	if (glewInit() != GLEW_OK)
	{

		Debug::Log("glewInit Error");

		IsRunning = false;

	}

	glEnable(GL_DEPTH_TEST);

}

void ShutdownProcedure()
{

	delete VertexColorShader;

	Debug::Disable();

	SDL_GL_DeleteContext(GLContext);
	SDL_DestroyWindow(Window);
	SDL_Quit();

}

float Timer;

void GameLoop(float DeltaTime)
{

	// Inputs

	SDL_Event Event;

	while (SDL_PollEvent(&Event))
	{

		PollEvent(&Event);

	}

	// Game Logic

	// Render

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Timer += DeltaTime;
	float Factor = float(std::sin(Timer * 2.0) + 1.0f) / 2.0f;

	glm::mat4 model;
	model = glm::rotate(model, glm::radians(Timer * 20.0f), glm::vec3(1.0f, 1.0f, 0.0f));

	glm::mat4 view;
	// note that we're translating the scene in the reverse direction of where we want to move
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

	glm::mat4 projection;
	projection = glm::perspective(glm::radians(45.0f), 1920.0f / 1080.0f, 0.1f, 100.0f);

	VertexColorShader->SetUniform(MODEL_UNIFORM, model);
	VertexColorShader->SetUniform(VIEW_UNIFORM, view);
	VertexColorShader->SetUniform(PROJECTION_UNIFORM, projection);

	VertexColorShader->SetUniform(TEXTURE0_UNIFORM, 0);
	VertexColorShader->SetUniform(TEXTURE1_UNIFORM, 1);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Texture1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, Texture2);

	glBindVertexArray(VertexArrayObject);
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	SDL_GL_SwapWindow(Window);

}

void LoadShaders()
{

	float Vertices[] =
	{
	
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	
	};

	unsigned int Indices[] =
	{
	
		0, 1, 2,
		3, 1, 2,
	
	};

	glGenVertexArrays(1, &VertexArrayObject);

	glBindVertexArray(VertexArrayObject);

	GLuint VertexBufferObject;
	glGenBuffers(1, &VertexBufferObject); // The first argument is how many buffers to make, so you could make an array of them

	glBindBuffer(GL_ARRAY_BUFFER, VertexBufferObject); // Binds the buffer object to the array buffer target

	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW); // Pushes vertex data to GPU

	// GL_STATIC_DRAW is meant for vertex data that won't change often, GL_DYNAMIC_DRAW or GL_STREAM_DRAW places it in GPU memory meant for faster writes

	// Element buffer

	GLuint ElementBufferObject;

	glGenBuffers(1, &ElementBufferObject);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBufferObject);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);

	VertexColorShader = new Shader("lighting", { { TEXTURE0_UNIFORM, "Texture" },{ MODEL_UNIFORM, "Model" },{ VIEW_UNIFORM, "View" },{ PROJECTION_UNIFORM, "Projection" } });

	VertexColorShader->Use();

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Coordinates s, t, r
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	stbi_set_flip_vertically_on_load(true);

	int Width, Height, NChannels;
	unsigned char* TextureData = stbi_load("./data/texture/aRUSHI.bmp", &Width, &Height, &NChannels, 0);

	glGenTextures(1, &Texture1);
	
	glBindTexture(GL_TEXTURE_2D, Texture1);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureData);
	glGenerateMipmap(GL_TEXTURE_2D);

	TextureData = stbi_load("./data/texture/owen.png", &Width, &Height, &NChannels, STBI_rgb_alpha);

	glGenTextures(1, &Texture2);

	glBindTexture(GL_TEXTURE_2D, Texture2);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, TextureData);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(TextureData);

}

void PollEvent(SDL_Event* Event)
{

	switch (Event->type)
	{

		case (SDL_QUIT):

			Backend::Shutdown();

			break;

		case (SDL_KEYDOWN):

			if (Event->key.keysym.sym == SDLK_1) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			else if (Event->key.keysym.sym == SDLK_2) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

			switch (Event->key.keysym.sym)
			{

				case SDLK_1:

					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
					break;

				case SDLK_2:

					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
					break;

			}

			break;

	}

}
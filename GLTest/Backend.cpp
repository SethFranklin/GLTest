
#include <chrono>
#include <fstream>
#include <cmath>
#include <string>

#include <SDL.h>
#include <glew.h>

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

Shader* SimpleShader;
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

}

void ShutdownProcedure()
{

	delete SimpleShader;
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
	glClear(GL_COLOR_BUFFER_BIT);

	Timer += DeltaTime;
	float Factor = float(std::sin(Timer * 2.0) + 1.0f) / 2.0f;

	SimpleShader->SetUniform(COLOR_UNIFORM, Factor * 1.0f, Factor * 0.5f, Factor * 0.2f, 1.0f);

	glBindVertexArray(VertexArrayObject); // Use vertex attribute data
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	SDL_GL_SwapWindow(Window);

}

void LoadShaders()
{

	float Vertices[] =
	{
	
		-0.5, -0.5, 0.0, // Position
		1.0, 0.0, 0.0, // Color

		-0.5, 0.5, 0.0,
		0.0, 1.0, 0.0,

		0.5, -0.5, 0.0,
		0.0, 0.0, 1.0,

		0.5, 0.5, 0.0,
		1.0, 1.0, 1.0
	
	};

	unsigned int Indices[] =
	{
	
		0, 1, 2,
		3, 1, 2
	
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

	SimpleShader = new Shader("singlecolor", { {COLOR_UNIFORM, "Color"} });
	VertexColorShader = new Shader("coloredvertex", {});

	VertexColorShader->Use();

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

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

				case SDLK_3:

					VertexColorShader->Use();
					break;

				case SDLK_4:

					SimpleShader->Use();
					break;

			}

			break;

	}

}
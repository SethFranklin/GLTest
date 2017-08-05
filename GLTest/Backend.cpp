
#include <chrono>
#include <fstream>
#include <cmath>

#include <SDL.h>
#include <glew.h>

#include "Backend.h"

const char GameName[] = "OpenGl Testing - Seth Franklin";

std::ofstream DebugStream;

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
GLuint ShaderProgram;
int TimeUniformLocation;

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

	DebugStream.open("debug.txt");

	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{

		DebugStream << "SDL_Init Error: " << SDL_GetError() << std::endl;

		IsRunning = false;

	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);

	if (SDL_GetCurrentDisplayMode(0, &MonitorInfo) != 0)
	{

		DebugStream << "SDL_GetCurrentDisplayMode Error: " << SDL_GetError() << std::endl;

		IsRunning = false;

	}

	if (MonitorInfo.refresh_rate != 0) FrameRate = MonitorInfo.refresh_rate;

	Window = SDL_CreateWindow(GameName, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, MonitorInfo.w, MonitorInfo.h, SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN);

	GLContext = SDL_GL_CreateContext(Window);

	glewExperimental = true;

	if (glewInit() != GLEW_OK)
	{

		DebugStream << "glewInit Error" << std::endl;

		IsRunning = false;

	}

}

void ShutdownProcedure()
{

	DebugStream.close();

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

	glUseProgram(ShaderProgram); // All rendering calls after a glUseProgram will use the program and it's shaders

	Timer += DeltaTime;
	float Factor = (std::sin(Timer * 10) + 1.0f) / 2.0f;

	glUniform4f(TimeUniformLocation, Factor * 1.0f, Factor * 0.5f, Factor * 0.2f, 1.0f);

	glBindVertexArray(VertexArrayObject); // Use vertex attribute data
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	SDL_GL_SwapWindow(Window);

}

void LoadShaders()
{

	float Vertices[] =
	{
	
		-0.5, -0.5, 0.0,
		-0.5, 0.5, 0.0,
		0.5, -0.5, 0.0,
		0.5, 0.5, 0.0
	
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

	const GLchar *VertexShaderSource = // @TODO: Read shaders from file instead of this ghetto ass way of writing
	"#version 330 core\n"
	"layout (location = 0) in vec3 Position;\n" // Location is used to link between vertex data and input for shaders
	"void main()\n"
	"{\n"
	"gl_Position = vec4(Position.x, Position.y, Position.z, 1.0);\n"
	"}\n";

	const GLchar *FragmentShaderSource =
	"#version 330 core\n"
	"out vec4 FragmentColor;\n"
	"uniform vec4 Color;\n"
	"void main()\n"
	"{\n"
	"FragmentColor = Color;\n"
	"}\n";

	// Compiling Shaders (OpenGl compiles GLSL at runtime)

	GLuint VertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(VertexShader, 1, &VertexShaderSource, NULL); // Attaches source code to shader object
	// The 1 value is how many strings are being passed on

	glCompileShader(VertexShader); // Compiles the shader

	glShaderSource(FragmentShader, 1, &FragmentShaderSource, NULL); // Compiles fragment shader
	glCompileShader(FragmentShader);

	// Compile error checking

	int Success;
	char CompileLog[512];

	glGetShaderiv(VertexShader, GL_COMPILE_STATUS, &Success); // Gets info about shader, in this case the compile status, info in form of an int

	if (!Success)
	{

		glGetShaderInfoLog(VertexShader, 512, NULL, CompileLog);

		DebugStream << "Vertex Shader Compile Error: " << CompileLog << std::endl;

	}

	glGetShaderiv(FragmentShader, GL_COMPILE_STATUS, &Success); // Gets info about shader, in this case the compile status, info in form of an int

	if (!Success)
	{

		glGetShaderInfoLog(FragmentShader, 512, NULL, CompileLog);

		DebugStream << "Fragment Shader Compile Error: " << CompileLog << std::endl;

	}

	// Linking to shader program

	ShaderProgram = glCreateProgram();

	glAttachShader(ShaderProgram, VertexShader);
	glAttachShader(ShaderProgram, FragmentShader);

	glLinkProgram(ShaderProgram); // Links the program's shaders together

	// Linking error checking

	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);

	if (!Success)
	{

		glGetProgramInfoLog(ShaderProgram, 512, NULL, CompileLog);

		DebugStream << "Shader Program Link Error: " << CompileLog << std::endl;

	}

	glDeleteShader(VertexShader); // Once linked to the shader program, the shader objects aren't needed anymore
	glDeleteShader(FragmentShader);

	TimeUniformLocation = glGetUniformLocation(ShaderProgram, "Color");

	// Vertex Attribute Linking

	// The vertex shader can input any kinds of vertex data, but you have to link between the array and the shader correctly

	// Use a vertex attribute pointer to link to locations

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	// 1: Location of vertex attribute
	// 2: How many values (so opengl knows it's a vec3)
	// 3: Type of data, float in this case
	// 4: Stride length, essentially how much data is in each vertex. In some cases you can have a color and position value for vertex, that would be 6 * sizeof(float)
	// 5: Offset for the data, you can have other data before vertex data? Has a strange cast

	glEnableVertexAttribArray(0); // Enables vertex attribute location 0

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

			break;

	}

}
#include "Engine.h"
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <stdio.h>
#include <windows.h>

// General stuff
SDL_Window* window;
SDL_Surface* window_surface;

SDL_Renderer* renderer;
SDL_Color current_clr;
TTF_Font* font;

bool is_open = false;

// Timing stuff
uint64_t clk_frequency;
uint64_t clk_begin;
uint64_t clk_previous;
float delta_time;

// Key stuff
struct KeyState
{
	bool pressed = false;
	int change_frame = 0;
};
int frame_num = 0;
KeyState key_states[(int)Key::MAX];

// Printing stuff
#define PRINT_MSG_LEN 2048
#define PRINT_MSG_MAX 16
struct PrintMessage
{
	char message[PRINT_MSG_LEN + 1];
	float print_time = -50.f; // Dirty
};

uint8_t print_start_idx = 0;
PrintMessage print_messages[PRINT_MSG_MAX];

void engInit()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();

	window = SDL_CreateWindow("Game",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		800, 600,
		0
	);
	window_surface = SDL_GetWindowSurface(window);

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	font = TTF_OpenFont("res/font.ttf", 14);

	is_open = true;
	current_clr = {0xFF, 0xFF, 0xFF, 0xFF};

	clk_frequency = SDL_GetPerformanceFrequency();
	clk_begin = clk_previous = SDL_GetPerformanceCounter();
}

void engShutdown()
{
	// :)
	is_open = false;
}

bool engBeginFrame()
{
	frame_num++;

	// Poll events
	SDL_Event event;
	while(SDL_PollEvent(&event))
	{
		switch(event.type)
		{
			case SDL_QUIT:
				is_open = false;
				break;

			case SDL_KEYDOWN:
			{
				// Don't care about repeats....
				if (event.key.repeat)
					break;

				int scancode = event.key.keysym.scancode;
				key_states[scancode].pressed = true;
				key_states[scancode].change_frame = frame_num;
				break;
			}

			case SDL_KEYUP:
			{
				int scancode = event.key.keysym.scancode;
				key_states[scancode].pressed = false;
				key_states[scancode].change_frame = frame_num;
				break;
			}
		}
	}

	// Render debug messages
	int print_y = 0;
	SDL_Color prev_clr = current_clr;

	engSetColor(0xFFCC66FF);

	for(int i = 0; i < PRINT_MSG_MAX; ++i)
	{
		int idx = (print_start_idx - i);
		// Wrap if <0
		if (idx < 0)
			idx += PRINT_MSG_MAX;

		PrintMessage* msg = &print_messages[idx];

		// 3 seconds passed, go away
		if (engElapsedTime() - msg->print_time > 3.f)
			continue;

		engText(5, 5 + print_y, msg->message);
		print_y += 16;
	}

	engSetColor(prev_clr.r, prev_clr.g, prev_clr.b, prev_clr.a);

	// Update renderer
	SDL_RenderPresent(renderer);

	// Update timing
	uint64_t clk_now = SDL_GetPerformanceCounter();
	uint64_t clk_delta = clk_now - clk_previous;

	clk_previous = clk_now;
	delta_time = (float)clk_delta / clk_frequency;

	return is_open;
}

void engSetColor(unsigned int clr)
{
	engSetColor((clr >> 24) & 0xFF, (clr >> 16) & 0xFF, (clr >> 8) & 0xFF, clr & 0xFF);
}

void engSetColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	SDL_SetRenderDrawColor(renderer, r, g, b, a);
	current_clr = { r, g, b, a };
}

void engClear()
{
	SDL_RenderClear(renderer);
}

void engRect(int x, int y, int width, int height)
{
	SDL_Rect rect = { x, y, width, height };
	SDL_RenderDrawRect(renderer, &rect);
}

void engFillRect(int x, int y, int width, int height)
{
	SDL_Rect rect = { x, y, width, height };
	SDL_RenderFillRect(renderer, &rect);
}

void drawModel(Mesh m, vector2 pos, float rot)
{
	Mesh transformed;

	// Rotate
	for (int i = 0; i < m.t.size(); i++)
	{
		transformed.t.push_back(Triangle());

		//triangle
		for (int j = 0; j < 3; j++)
		{
			transformed.t[i].point[j].x = m.t[i].point[j].x * cos(rot) - m.t[i].point[j].y * sin(rot);
			transformed.t[i].point[j].y = m.t[i].point[j].x * sin(rot) + m.t[i].point[j].y * cos(rot);
		}
	}

	// Translate
	for (int i = 0; i < transformed.t.size(); i++)
	{
		for (int j = 0; j < 3; j++)
		{
			transformed.t[i].point[j] += pos;
		}
	}

	// Drawing Triangles
	for (int i = 0; i < transformed.t.size(); i++)
	{
		drawFilledTriangle(transformed.t[i]);
	}
}

void drawFilledTriangle(Triangle t)
{
	int maxX = std::max(t.point[0].x, std::max(t.point[1].x, t.point[2].x));
	int minX = std::min(t.point[0].x, std::min(t.point[1].x, t.point[2].x));

	int maxY = std::max(t.point[0].y, std::max(t.point[1].y, t.point[2].y));
	int minY = std::min(t.point[0].y, std::min(t.point[1].y, t.point[2].y));

	for (int y = minY; y < maxY + 1; y++)
	{
		for (int x = minX; x < maxX + 1; x++)
		{
			vector2 point = vector2(x, y);

			if (pointInTriangle(point, t))
			{
				SDL_RenderDrawPoint(renderer, (int)point.x, (int)point.y);
			}
		}
	}
}

void drawCircle(vector2 pos, float radius, int circleResolution)
{
	float deltaTheta = 6.28318530718 / circleResolution;
	float theta = 0.0f;

	std::vector<vector2> points = std::vector<vector2>(circleResolution);

	for (int i = 0; i < circleResolution; i++)
	{
		points[i] = vector2(pos.x + (radius * cos(theta)), pos.y + (radius * sin(theta)));
		theta += deltaTheta;
	}

	for (int i = 0; i < circleResolution; i++)
	{
		if (i == circleResolution - 1) 
		{
			SDL_RenderDrawLine(renderer, points[i].x, points[i].y, points[0].x, points[0].y);
		}
		else 
		{
			SDL_RenderDrawLine(renderer, points[i].x, points[i].y, points[i + 1].x, points[i + 1].y);
		}
	}
}

void drawLine(vector2 pos1, vector2 pos2)
{
	SDL_RenderDrawLine(renderer, pos1.x, pos1.y, pos2.x, pos2.y);
}

float sign(vector2 a, vector2 b, vector2 c)
{
	return (a.x - c.x) * (b.y - c.y) - (b.x - c.x) * (a.y - c.y);
}

bool pointInTriangle(vector2 point, Triangle t)
{
	float d1, d2, d3;
	bool negative, positive;

	d1 = sign(point, t.point[0], t.point[1]);
	d2 = sign(point, t.point[1], t.point[2]);
	d3 = sign(point, t.point[2], t.point[0]);

	negative = (d1 < 0) || (d2 < 0) || (d3 < 0);
	positive = (d1 > 0) || (d2 > 0) || (d3 > 0);

	return !(negative && positive);
}

float lerp(float v0, float v1, float t)
{
	return (1 - t) * v0 + t * v1;
}

void engText(int x, int y, const char* str)
{
	SDL_Surface* text_surface = TTF_RenderText_Blended(font, str, current_clr);

	// Rendering text failed for some reason :S
	if (!text_surface)
		return;

	SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);

	SDL_Rect copy_rect = { x, y, text_surface->w, text_surface->h };

	SDL_RenderCopy(renderer, text_texture, nullptr, &copy_rect);
	SDL_DestroyTexture(text_texture);
	SDL_FreeSurface(text_surface);
}

void engTextf(int x, int y, const char* format, ...)
{
	// Parse format string :I
	static char* buffer = nullptr;
	static int buffer_len = 0;

	va_list vl;
	va_start(vl, format);
	int msg_len = vsnprintf(nullptr, 0, format, vl);

	if (msg_len > buffer_len)
	{
		if (buffer)
			free(buffer);

		buffer = (char*)malloc(msg_len + 1);
		buffer_len = msg_len;
	}

	vsprintf_s(buffer, msg_len + 1, format, vl);
	va_end(vl);

	engText(x, y, buffer);
}

void engPrint(const char* format, ...)
{
	va_list vl;
	va_start(vl, format);

	// Increase index
	print_start_idx = (print_start_idx + 1) % PRINT_MSG_MAX;

	PrintMessage* msg = &print_messages[print_start_idx];
	msg->print_time = engElapsedTime();
	vsprintf_s(msg->message, PRINT_MSG_LEN, format, vl);

	va_end(vl);
}

bool engKeyDown(Key key)
{
	return key_states[(int)key].pressed;
}

bool engKeyPressed(Key key)
{
	return key_states[(int)key].pressed &&
		key_states[(int)key].change_frame == frame_num;
}

bool engKeyReleased(Key key)
{
	return !key_states[(int)key].pressed &&
		key_states[(int)key].change_frame == frame_num;
}

float engDeltaTime()
{
	return delta_time;
}

float engElapsedTime()
{
	return (float)(clk_previous - clk_begin) / clk_frequency;
}

bool _engError(const char* format, ...)
{
	// Fetch message size
	va_list vl;
	va_start(vl, format);

	int msgSize = vsnprintf(nullptr, 0, format, vl);
	msgSize += 1; // Leave room for null terminator

	char* msg = (char*)alloca(msgSize);

	// Write message
	vsprintf_s(msg, msgSize, format, vl);

	va_end(vl);

	// Show error box
	MessageBoxA(NULL, msg, "Error", MB_OK);
	return true;
}
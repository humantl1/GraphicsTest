#pragma once
#include <Windows.h> // TODO : is this causing the error with renderer? add Render_State to its own header?

typedef unsigned int u32; // now instead of typing "unsigned int", just type u32

#define global_variable static
#define internal static

// color definitions
#define red 0xC70039 
#define orange 0xFF5733 
#define green 0x2ecc71 
#define blue 0x2e86c1 
#define yellow 0xFFC300 
#define white 0xFFFFFF
#define black 0x000000

global_variable float render_scale = 0.01f;

struct Render_State // data for current state of buffer
{
	int height, width;
	void* memory;

	BITMAPINFO bitmap_info; // contains the data for the buffer bitmap
};

extern Render_State render_state;

inline int clamp(int min, int val, int max);

void draw_graphics();

void clear_screen(u32 color);

void draw_point_in_pixels(int x, int y, u32 color);

void draw_point(float x, float y, u32 color);

void draw_line(int x0, int y0, int x1, int y1, u32 color);

void draw_line(int x0, int y0, float magnitude, float degrees, u32 color);

float deg_to_rad(float degrees);


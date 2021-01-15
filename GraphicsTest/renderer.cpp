struct Vector // probably will need this, right?
{
	int x, y;
};

/// <summary>
/// fills window with specified color
/// </summary>
/// <param name="color"> color to fill </param>
internal void clear_screen(u32 color)
{
	u32* pixel = (u32*)render_state.memory; // points to first pixel in memory buffer
	for (int y = 0; y < render_state.height; ++y)
	{
		for (int x = 0; x < render_state.width; ++x)
		{
			*pixel++ = color; // iterate through every pixel in memory buffer
		}
	}
}

internal void draw_point_in_pixels(int x, int y, u32 color)
{
	x = clamp(0, x, render_state.width);
	y = clamp(0, y, render_state.height);

	u32* pixel = (u32*)render_state.memory + y * render_state.width + x;

	*pixel = color;
}

internal void draw_point(float x, float y, u32 color)
{
	x *= render_state.width * render_scale;
	y *= render_state.height * render_scale;

	x += render_state.width / 2.f;
	y += render_state.height / 2.f;

	draw_point_in_pixels(x, y, color);
}


// Variation of Bresenham's line algorithm from Wikipedia
internal void draw_line(int x0, int y0, int x1, int y1, u32 color)
{
	int dx = abs(x1 - x0); // change in x
	int sx = x0 < x1 ? 1 : -1; // slope of x
	int dy = -abs(y1 - y0); // change in y (negated to calculate difference between dx and dy)
	int sy = y0 < y1 ? 1 : -1; // slope of y
	int err = dx + dy; // difference between dx and dy (dy was negated)

	while (true) // exit loop with breaks
	{
		u32* pixel = (u32*)render_state.memory + y0 * render_state.width + x0; // (x0, y0) in buffer
		*pixel = color; // draw pixel
		
		if (x0 == x1 && y0 == y1) break; // if current point is the endpoint, exit

		int e2 = 2 * err;
		if (e2 >= dy) // e_xy + e_x > 0
		{
			err += dy;
			x0 += sx;
		}
		if (e2 <= dx) // e_xy + e_y < 0
		{
			err += dx;
			y0 += sy;
		}
	}
}

/// <summary>
/// draws filled rectangle to window
/// </summary>
/// <param name="x0"> lower left x </param>
/// <param name="y0"> lower left y </param>
/// <param name="x1"> upper right x </param>
/// <param name="y1"> upper right y</param>
/// <param name="color"> draw color </param>
internal void draw_rect_in_pixels(int x0, int y0, int x1, int y1, u32 color)
{
	x0 = clamp(0, x0, render_state.width);
	y0 = clamp(0, y0, render_state.height);
	x1 = clamp(0, x1, render_state.width);
	y1 = clamp(0, y1, render_state.height);

	for (int y = y0; y < y1; ++y)
	{
		u32* pixel = (u32*)render_state.memory + y * render_state.width + x0; // points to first pixel in memory buffer
		for (int x = x0; x < x1; ++x)
		{
			*pixel++ = color; // iterate through every pixel in memory buffer
		}
	}
}

internal void draw_rect(float x, float y, float half_size_x, float half_size_y, u32 color)
{
	x *= render_state.height * render_scale;
	y *= render_state.height * render_scale;
	half_size_x *= render_state.height * render_scale;
	half_size_y *= render_state.height * render_scale;

	x += render_state.width / 2.f;
	y += render_state.height / 2.f;

	// change to pixel
	int x0 = x - half_size_x;
	int x1 = x + half_size_x;
	int y0 = y - half_size_y;
	int y1 = y + half_size_y;


	draw_rect_in_pixels(x0, y0, x1, y1, color);
}
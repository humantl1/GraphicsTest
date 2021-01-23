struct Vertice
{
	int x, y;
};

/// <summary>
/// fills window with specified color
/// </summary>
/// <param name="color"> color to fill </param>
static void clear_screen(u32 color)
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

static void draw_point_in_pixels(int x, int y, u32 color)
{
	x = clamp(0, x, render_state.width);
	y = clamp(0, y, render_state.height);

	u32* pixel = (u32*)render_state.memory + y * render_state.width + x;

	*pixel = color;
}

static void draw_point(float x, float y, u32 color)
{
	x *= render_state.width * render_scale;
	y *= render_state.height * render_scale;

	x += render_state.width / 2.f;
	y += render_state.height / 2.f;

	draw_point_in_pixels(x, y, color);
}

// Variation of Bresenham's line algorithm from Wikipedia
static void draw_line(int x0, int y0, int x1, int y1, u32 color)
{
	x0 = clamp(0, x0, render_state.width);
	y0 = clamp(0, y0, render_state.height);
	x1 = clamp(0, x1, render_state.width);
	y1 = clamp(0, y1, render_state.height);

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

		int e2 = 2 * err; // magic part of algorithm (to me)
		if (e2 >= dy) // e_xy + e_x > 0 : evaluates if x should be advanced
		{
			err += dy;
			x0 += sx;
		}
		if (e2 <= dx) // e_xy + e_y < 0 : evaluates if you should be advanced
		{
			err += dx;
			y0 += sy;
		}
	}
	// look at the graphics book for a line algorithm (or find a simpler one) and compare them
	// x0 = 0, x1 = 10
	// y0 = 0, y1 = 5
	// draw out how i think it should work, then work through it to see how it actually works
}

/// <summary>
/// draw line from start point, length, angle in degrees, and color
/// </summary>
/// <param name="x0">x from lower left</param>
/// <param name="y0">y from lower left</param>
/// <param name="magnitude">length of line</param>
/// <param name="degrees">angle of line in degrees, from horizontal right</param>
/// <param name="color">hex color</param>
/// <returns></returns>
static void draw_line(int x0, int y0, float magnitude, float degrees, u32 color)
{
	float radians = deg_to_rad(degrees);
	float x1 = cos(radians);
	x1 *= magnitude;
	float y1 = sin(radians);
	y1 *= magnitude;

	draw_line(x0, y0, (int)x1, (int)y1, color);
}

static void draw_triangle_outline(int x0, int y0, int x1, int y1, int x2, int y2, u32 color)
{
	draw_line(x0, y0, x1, y1, color);
	draw_line(x1, y1, x2, y2, color);
	draw_line(x0, y0, x2, y2, color);
}

// rasterize a flat-bottom triangle using the "standard algorithm"
// algorithm from http://www.sunshine2k.de/coding/java/TriangleRasterization/TriangleRasterization.html
static void fill_bottom_flat_triangle(Vertice v0, Vertice v1, Vertice v2, u32 color)
{
	float invslope0 = (float)(v1.x - v0.x) / (v1.y - v0.y); // x/y slope from v1 bottom to top 
	float invslope1 = (float)(v2.x - v0.x) / (v2.y - v0.y); // x/y slope from v2 bottom to top

	float curx0 = v0.x; // start point of line from top to bottom of v1 side
	float curx1 = v0.x; // start point of line from top to bottom of v2 side

	// descend line by line from top of triangle to bottom
	for (int scanlineY = v0.y; scanlineY >= v2.y; scanlineY--) 
	{
		draw_line((int)curx0, scanlineY, (int)curx1, scanlineY, color);
		curx0 -= invslope0; // expand end point according to ratio of dx over dy
		curx1 -= invslope1;
	}
}

/// <summary>
/// Rasterize a flat-bottomed triangle
/// </summary>
/// <param name="x0">x top of triangl</param>
/// <param name="y0">y top of triangle</param>
/// <param name="x1">x bottom point of triangle</param>
/// <param name="x2">x bottom point of triangle</param>
/// <param name="y1">y bottom of triangle</param>
/// <param name="color">triangle color</param>
/// <returns></returns>
static void fill_bottom_flat_triangle(int x0, int y0, int x1, int x2, int y1, u32 color)
{
	Vertice v0, v1, v2;

	v0.x = x0;
	v0.y = y0;
	v1.x = x1;
	v1.y = y1;
	v2.x = x2;
	v2.y = y1;

	fill_bottom_flat_triangle(v0, v1, v2, color);
}

// rasterize a flat-top triangle using the "standard algorithm"
// algorithm from http://www.sunshine2k.de/coding/java/TriangleRasterization/TriangleRasterization.html
static void fill_top_flat_triangle(Vertice v0, Vertice v1, Vertice v2, u32 color)
{
	float invslope0 = (float)(v2.x - v0.x) / (v2.y - v0.y); // x/y slope from bottom left to top 
	float invslope1 = (float)(v2.x - v1.x) / (v2.y - v1.y); // x/y slope from bottom right to top

	float curx0 = v2.x; // start point of line from top to lower left
	float curx1 = v2.x; // start point of line from top to lower right

	for (int scanlineY = v2.y; scanlineY <= v1.y; scanlineY++) // descend line by line from top of triangle to bottom
	{
		draw_line((int)curx0, scanlineY, (int)curx1, scanlineY, color);
		curx0 += invslope0;
		curx1 += invslope1;
	}
}

/// <summary>
/// Rasterize a flat-top triangle
/// </summary>
/// <param name="x0">x bottom point of triangl</param>
/// <param name="x1">x bottom point of triangl</param>
/// <param name="y0">y bottom of triangle</param>
/// <param name="x2">x top point of triangle</param>
/// <param name="y2">y top point of triangle</param>
/// <param name="color">triangle color</param>
/// <returns></returns>
static void fill_top_flat_triangle(int x0, int x1, int y0, int x2, int y2, u32 color)
{
	Vertice v0, v1, v2;

	v0.x = x0;
	v0.y = y0;
	v1.x = x1;
	v1.y = y0;
	v2.x = x2;
	v2.y = y2;

	fill_top_flat_triangle(v0, v1, v2, color);
}

// sort triangle vertices in ascending order, so v0 is the topmost vertice
static void sort_triangle_vertices(Vertice* v0, Vertice* v1, Vertice* v2)
{
	Vertice* temp = new Vertice;

	if (v0->y < v1->y)
	{
		std::swap(*temp, *v0);
		std::swap(*v0, *v1);
		std::swap(*v1, *temp);
	}
	if (v0->y < v2->y)
	{
		std::swap(*temp, *v0);
		std::swap(*v0, *v2);
		std::swap(*v2, *temp);
	}
	if (v1->y < v2->y)
	{
		std::swap(*temp, *v1);
		std::swap(*v1, *v2);
		std::swap(*v2, *temp);
	}
}

static void draw_triangle(Vertice v0, Vertice v1, Vertice v2, u32 color)
{
	sort_triangle_vertices(&v0, &v1, &v2); // arrange from greatest y to least y
	
	if (v1.y == v2.y) // bottom side is flat
		fill_bottom_flat_triangle(v0, v1, v2, color);
	else if (v0.y == v1.y) // top side is flat
		fill_top_flat_triangle(v0, v1, v2, color);
	else // general case, split triangles into bottom-flat and top-flat triangles
	{
		Vertice v3;
		// equation for intercepting the longer side at the y value of the shorter side's endpoint:
		v3.x = (int)(v0.x + ((float)(v1.y - v0.y) / (float)(v2.y - v0.y)) * (v2.x - v0.x));
		v3.y = v1.y;

		fill_bottom_flat_triangle(v0, v1, v3, color);
		fill_top_flat_triangle(v1, v3, v2, color);
	}
}

// convert points to vertices and send to draw_triangle function
static void draw_triangle(int x0, int y0, int x1, int y1, int x2, int y2, u32 color)
{
	Vertice v0, v1, v2;

	v0.x = x0;
	v0.y = y0;
	v1.x = x1;
	v1.y = y1;
	v2.x = x2;
	v2.y = y2;

	draw_triangle(v0, v1, v2, color);
}

/// <summary>
/// draws filled rectangle to window
/// starting at bottom left of window
/// units are in absolute pixels
/// </summary>
/// <param name="x0"> lower left x </param>
/// <param name="y0"> lower left y </param>
/// <param name="x1"> upper right x </param>
/// <param name="y1"> upper right y</param>
/// <param name="color"> draw color </param>
static void draw_rect_in_pixels(int x0, int y0, int x1, int y1, u32 color)
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

// draw rect starting at bottom left of winow, units are percentage of screen height
static void draw_rect(float x, float y, float length_x, float length_y, u32 color)
{
	x *= render_state.height * render_scale;
	y *= render_state.height * render_scale;
	length_x *= render_state.height * render_scale;
	length_y *= render_state.height * render_scale;

	// change to pixel
	int x0 = x;
	int x1 = x + length_x;
	int y0 = y;
	int y1 = y + length_y;

	draw_rect_in_pixels(x0, y0, x1, y1, color);
}
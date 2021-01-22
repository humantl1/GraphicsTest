 
static void draw_graphics()
{
	clear_screen(white);

	//draw_rect(0, 0, 150, 50, black);
	//draw_point(10, 10, black);

	float x1 = 200;
	float y1 = 200;

	float angle = 22.5;
	
	float rad = deg_to_rad(angle);

	Eigen::RowVector2f v;
	v(0) = x1;
	v(1) = y1;

	Eigen::Matrix2f t;
	t(0, 0) = cos(rad);
	t(0, 1) = sin(rad);
	t(1, 0) = -sin(rad);
	t(1, 1) = cos(rad);

	draw_line(0, 0, (int)v(0), (int)v(1), black);

	v = v * t;

	draw_line(0, 0, (int)v(0), (int)v(1), black);

	//draw_line(275, 0, 275, 500, black);
	//draw_line(500, 400, 100.5f, 22.5f, black);
	//draw_triangle_outline(0, 0, 200, 400, 400, 60, black);
	//fill_bottom_flat_triangle(200, 400, 0, 400, 0, black);
	//fill_top_flat_triangle(0, 400, 400, 200, 0, black);
	//draw_triangle(0, 75, 200, 300, 300, 0, black);
}

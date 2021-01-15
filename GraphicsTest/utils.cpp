typedef unsigned int u32; // now instead of typing "unsigned int", just type u32

#define global_variable static
#define internal static

global_variable float render_scale = 0.01f;

inline int clamp(int min, int val, int max)
{
	if (val < min) return min;
	if (val > max) return max;
	return val;
}

inline float deg_to_rad(float degrees)
{
	return degrees * 3.14159 / 180;
}
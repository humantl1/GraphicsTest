typedef unsigned int u32; // now instead of typing "unsigned int", just type u32

#define global_variable static
//#define internal static

// color definitions
#define red 0xC70039 
#define orange 0xFF5733 
#define green 0x2ecc71 
#define blue 0x2e86c1 
#define yellow 0xFFC300 
#define white 0xFFFFFF
#define black 0x000000


global_variable float render_scale = 0.01f;

inline int clamp(int min, int val, int max) // TODO: not robust enough. Still CTD when reduced, just less often
{
	if (val < min) return min;
	if (val > max) return max;
	return val;
}

inline float deg_to_rad(float degrees)
{
	return degrees * 3.14159 / 180;
}
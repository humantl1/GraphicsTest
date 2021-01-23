#include <Windows.h>
#include <vector>
#include <utility>
#include <Eigen/Dense>
#include "utils.cpp"

global_variable bool running = true; 

struct Render_State // data for current state of buffer
{
	int height, width;
	void* memory;

	BITMAPINFO bitmap_info; // contains the data for the buffer bitmap
};

global_variable Render_State render_state;


#include "shapes.cpp"
#include "mathHelper.cpp"
#include "renderer.cpp" // this inclusion of another .cpp is "Unity" style compiling
#include "graphics.cpp"



LRESULT CALLBACK window_callback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;

	switch (uMsg)
	{
		case WM_CLOSE:
		case WM_DESTROY: 
		{
			running = false; // close window if destroyed
		} break;

		case WM_SIZE: // windows resize messages
		{
			RECT rect;
			GetClientRect(hwnd, &rect); // get new rect size
			render_state.width = rect.right - rect.left; // calculate width based on left and right of rect
			render_state.height = rect.bottom - rect.top; // calculate height based on top and bottom of rect

			int buffer_size = render_state.width * render_state.height * sizeof(u32); // the total buffer size is each pixel in rect * size of pixel
			
			if (render_state.memory) // if a buffer already exists, release it
				VirtualFree(render_state.memory, 0, MEM_RELEASE);

			render_state.memory = VirtualAlloc(0, // address of memory (0 = doesn't matter)
				buffer_size, // size of memory
				MEM_COMMIT | MEM_RESERVE, // type
				PAGE_READWRITE); // read and write to memory

			render_state.bitmap_info.bmiHeader.biSize = sizeof(render_state.bitmap_info.bmiHeader); // size of bitmap info header so windows knows what version
			render_state.bitmap_info.bmiHeader.biWidth = render_state.width; // width of window
			render_state.bitmap_info.bmiHeader.biHeight = render_state.height; // height of window
			render_state.bitmap_info.bmiHeader.biPlanes = 1; // 2d?
			render_state.bitmap_info.bmiHeader.biBitCount = 32; // bits in pixel (32 bit unsigned int) 
			render_state.bitmap_info.bmiHeader.biCompression = BI_RGB; // compression of RGB

		} break;

		default: 
		{
			result = DefWindowProc(hwnd, uMsg, wParam, lParam); // window message object
		}
	}

	return result;
}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	// Create Window Class
	WNDCLASS window_class = {};
	window_class.style = CS_HREDRAW | CS_VREDRAW; // style of window, whatever that means
	window_class.lpszClassName = L"Game Window Class"; // names the window, the 'L' gives each char more bytes, I think
	window_class.lpfnWndProc = window_callback; // this passes messages about the window. calls window_callback whenever it needs to send a message

	// Register Class
	RegisterClass(&window_class);

	// Create Window
	// the 'A' stands for ansii, deleting that will make windows decide what is best (ansii or wide
	HWND window = CreateWindow(window_class.lpszClassName, // class name created above 
		L"My First Window", // name of window
		WS_OVERLAPPEDWINDOW | WS_VISIBLE, // window options
		CW_USEDEFAULT, // default x start position
		CW_USEDEFAULT, // default y start position
		1280, // nWidth
		720, // nHeight
		0, // hWndParent
		0, // hMenu
		hInstance, // this variable was recieved in entry point WinMain
		0); // lpParam

	HDC hdc = GetDC(window); // saves handle for device context window is drawn in

	float delta_time = 0.016666f; // assume 60 fps for first frame
	LARGE_INTEGER frame_begin_time;
	QueryPerformanceCounter(&frame_begin_time);

	// this returns how many cycles the cpu can perform in one second
	float performance_frequency; 
	{
		LARGE_INTEGER perf;
		QueryPerformanceFrequency(&perf);
		performance_frequency = (float)perf.QuadPart;
	}

	while (running)
	{
		// Input
		MSG message; // window message object
		// gets all messages that haven't been read yet
		while (PeekMessage(&message, // pointer to message
			window, // the window 
			0, 0, // min and max filter 
			PM_REMOVE)) // flags : remove message after it has been read once
		{
			TranslateMessage(&message);
			DispatchMessage(&message); // dispatch message to the callback
		}

		// Simulate
		draw_graphics();

		// Render
		StretchDIBits(hdc, // handle to a device context. references that windows uses to draw to 
			0, // destination x
			0, // destination y
			render_state.width, // width of whole window
			render_state.height, // height of whole window
			0, // source x (same as destination, b/c this is 1 to 1 copy)
			0, // source y (same as destination, b/c this is 1 to 1 copy)
			render_state.width, // source width
			render_state.height, // source height
			render_state.memory, // memory passed to pixels
			&render_state.bitmap_info, // pointer to bitmap info. lots of info : what does pixel look like, what is the compression, etc. lets windows know how to copy the data to video card
			DIB_RGB_COLORS, // usage
			SRCCOPY); // operation : copy from source

		LARGE_INTEGER frame_end_time;
		QueryPerformanceCounter(&frame_end_time);
		// gives time since begin, in seconds : change in cycles / cycles per second
		delta_time = (float)(frame_end_time.QuadPart - frame_begin_time.QuadPart) / performance_frequency; 
		frame_begin_time = frame_end_time;
	}
}
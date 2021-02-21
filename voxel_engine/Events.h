#include "Window.h"

class Events
{
public:
	static bool* _keys;
	static unsigned int* _frames;
	static unsigned int _current_frame;
	static float deltaX;
	static float deltaY;
	static float x;
	static float y;
	static bool _cursor_locked;
	static bool _cursor_started;

	static int initialize();
	static void pullEvents();

	static bool isPressed(int keyCode);
	static bool justPressed(int keyCode);
	static bool isClicked(int button);
	static bool justClicked(int button);

	static void toggleCursor();
};


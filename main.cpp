#include <stdio.h>
#include <GLFW/glfw3.h>
#include <core/Window.hpp>

void error_callback(int error, const char *description)
{
	printf("%s %d\n", description, __LINE__);
}


using namespace eto;

int main(void) {
	Window *w = new Window();
	Window ww;

	printf("%d %d\n", w->getPos().x, w->getPos().y);
	printf("%d\n", w->create(800, 600, "title"));
	printf("%d\n", ww.create(800, 600, "title2"));

	delete w;

	return 0;
}

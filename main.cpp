#include <iostream>
#include <GLFW/glfw3.h>
#include <core/Window.hpp>

void error_callback(int error, const char *description)
{
	printf("%d %sn", error, description);
}


using namespace eto;




int main(void) {
	Window w;
	if (!w.create(800, 600, "mega window"))
		return 1;
	
	while (!w.shouldClose())
	{
		w.pollEvents();
		GLFWevent event;
		if(w.getEvent(event)){
			 std::cout << "Event type: " << event.type << std::endl;
			 if (event.type == GLFWevent::Type::Key) {
				 std::cout << "Key ";
				 if (event.key.action == Input::Action::Press)
					 std::cout << "pressed";
				 else if (event.key.action == Input::Action::Release)
					 std::cout << "released";
				 else
					 std::cout << "held";

				 std::cout << ": " << event.key.key << std::endl;
				 if (event.key.key == Input::Key::Escape)
					 w.setShouldClose(true);
			 }
		}
	}

	return 0;
}

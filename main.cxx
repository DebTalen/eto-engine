#include <iostream>
#include <chrono>
#include <cstdio>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <core/Window.hpp>
#include <glm/glm.hpp>

#include <core/Input.hpp>
#include <resources/AssetLoader.hpp>
#include <resources/ShaderLoader.hpp>
#include <resources/ModelLoader.hpp>
#include <graphcis/core/ShaderProgram.hpp>
#include <core/EntityManager.hpp>

//temporary measure
#define ROOT_DIR std::string("${ROOT_DIR}")

using std::cout;
using std::endl;
using namespace eto;
using glm::vec3;

void print_fPS()
{
	static float last_time = glfwGetTime();
	static int num_frames = 0;
	double time = glfwGetTime();
	++num_frames;
	if ( time - last_time >= 1.0 ){ // If last prinf() was more than 1 sec ago
		// printf and reset timer
		printf("%f ms/frame\n", 1000.0/double(num_frames));
		num_frames = 0;
		last_time += 1.0;
	}
}

int main()
{

	/* 	Window w;
		w.create(1280, 720, "a");
		w.set_input_mode(GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		w.set_pos({0, 100});
		glfwMakeContextCurrent(w.get_raw_pointer());

		Input::get_instance().set_window(w);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
		}
		glEnable(GL_DEPTH_TEST);
	//	glfwSwapInterval(0); // disable vsync and fixed frame rate

	AssetLoader loader = AssetLoader::get_instance();
	std::shared_ptr<ShaderProgram> sd = std::make_shared<ShaderProgram>();
	{
	auto vs = loader.load<ShaderLoader>(ROOT_DIR + "/shaders/default.vs", VertexShader);
	if (! vs->is_compiled())
	cout << vs->get_error_message() << endl;

	auto fs = loader.load<ShaderLoader>(ROOT_DIR + "/shaders/default.fs", FragmentShader);
	if (! fs->is_compiled())
	cout << fs->get_error_message() << endl;
	sd->attach_shader(*vs);
	sd->attach_shader(*fs);
	sd->link();
	if (! sd->is_linked() )
	cout << sd->get_error_message() << endl;
	}
	auto ws = std::make_shared<ShaderProgram>();
	{
	auto vs = loader.load<ShaderLoader>(ROOT_DIR + "/shaders/no_textures.vs", VertexShader);
	if (! vs->is_compiled())
	cout << vs->get_error_message() << endl;

	auto fs = loader.load<ShaderLoader>(ROOT_DIR + "/shaders/no_textures.fs", FragmentShader);
	if (! fs->is_compiled())
	cout << fs->get_error_message() << endl;
	ws->attach_shader(*vs);
	ws->attach_shader(*fs);
	ws->link();
	if (! ws->is_linked() )
	cout << ws->get_error_message() << endl;
	}

	auto sphere_model = loader.load<ModelLoader>(ROOT_DIR + "/assets/testsphere.nff", ws);
	if (! sphere_model->is_loaded()) {
	std::cerr << sphere_model->get_error_message() << endl;
	return 31;
	}

	auto cake_model = loader.load<ModelLoader>(ROOT_DIR + "/assets/Asuka Soryu/School Uniform/Asuka School Uniform.obj", sd);
	if (! cake_model->is_loaded()) {
	std::cerr << cake_model->get_error_message() << std::endl;
	return 33;
	}

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	float d = 0;
	float dd = 0.05;
	Input &input = Input::get_instance();

	while (! w.should_close())
	{
	print_fPS();
	w.poll_events();
	if (input.is_key_release(input::Escape))
	w.set_should_close(true);

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (d < -0.5)
		dd = 0.005;
	else if (d > 0.5) 
		dd = -0.005;
	d += dd;

	w.swap_buffers();
	//std::this_thread::sleep_for(std::chrono::milliseconds(10));
}

*/

return 0;
}

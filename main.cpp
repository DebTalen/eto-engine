#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <cstdio>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <core/Window.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <core/Input.hpp>
#include <resources/AssetLoader.hpp>
#include <resources/ShaderLoader.hpp>
#include <resources/ModelLoader.hpp>
#include <graphcis/core/ShaderProgram.hpp>
#include <graphcis/core/Renderable.hpp>
#include <graphcis/core/Light.hpp>
#include <graphcis/core/Camera.hpp>


using std::cout;
using std::endl;
using namespace eto;

using glm::vec3;

 enum CameraMovement {
	 FORWARD  = input::Key::W,
	 BACKWARD = input::Key::S,
	 UPWARD   = input::Key::Space,
	 DOWNWARD = input::Key::C,
	 RIGHT    = input::Key::D,
	 LEFT 	 = input::Key::A
 };
void processKeyboard(CameraMovement direction, Camera &c)
{
	float velocity = 0.15;
	switch (direction)
	{
		case FORWARD:
			c.move(c.getTarget() * velocity); break;
		case BACKWARD:
			c.move(-(c.getTarget() * velocity)); break;
		case LEFT:
			c.move(glm::normalize(glm::cross(c.getTarget(), vec3(0,-1, 0))) * velocity); break;
		case RIGHT:
			c.move(glm::normalize(glm::cross(c.getTarget(), vec3(0, 1, 0))) * velocity); break;
		case UPWARD:
			c.move(vec3(0, 1, 0) * velocity); break;
		case DOWNWARD:
			c.move(vec3(0,-1, 0) * velocity); break;
	}
	//	_pos.y = 0; for fixed floor
}

bool firstMouse = true;
float lastX, lastY;
void processMouse(float x, float y, Camera &c)
{
	if (firstMouse)
	{
		lastX = x;
		lastY = y;
		firstMouse = 0;
	}
	float xOffset = x - lastX;
	float yOffset = lastY - y;
	lastX = x;
	lastY = y;

	float sensity = 0.05;
	xOffset *= sensity;
	yOffset *= sensity;
	c.rotate({xOffset, yOffset, 0});
}

int main()
{
	Window w;
	w.create(1280, 720, "a");
	w.setInputMode(GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	w.setPos({0, 100});
	glfwMakeContextCurrent(w.getRawPointer());

	Input::getInstance().setWindow(w);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	glEnable(GL_DEPTH_TEST);

	AssetLoader loader = AssetLoader::getInstance();
	SPtr<ShaderProgram> sd = std::make_shared<ShaderProgram>();
	{
		auto vs = loader.load<ShaderLoader>("/home/morgoth/cpp/eto/shaders/default.vs", VertexShader);
		if (! vs->isCompiled())
			cout << vs->getErrorMessage() << endl;

		auto fs = loader.load<ShaderLoader>("/home/morgoth/cpp/eto/shaders/default.fs", FragmentShader);
		if (! fs->isCompiled())
			cout << fs->getErrorMessage() << endl;
		sd->attachShader(*vs);
		sd->attachShader(*fs);
		sd->link();
		if (! sd->isLinked() )
			cout << sd->getErrorMessage() << endl;
	}
	auto ws = std::make_shared<ShaderProgram>();
	{
		auto vs = loader.load<ShaderLoader>("/home/morgoth/cpp/eto/shaders/no_textures.vs", VertexShader);
		if (! vs->isCompiled())
			cout << vs->getErrorMessage() << endl;

		auto fs = loader.load<ShaderLoader>("/home/morgoth/cpp/eto/shaders/no_textures.fs", FragmentShader);
		if (! fs->isCompiled())
			cout << fs->getErrorMessage() << endl;
		ws->attachShader(*vs);
		ws->attachShader(*fs);
		ws->link();
		if (! ws->isLinked() )
			cout << ws->getErrorMessage() << endl;
	}

	auto light_model = loader.load<ModelLoader>("/home/morgoth/cpp/eto/assets/testsphere.nff", ws);
	if (! light_model->isLoaded()) {
		std::cerr << light_model->getErrorMessage() << endl;
		return 31;
	}

	auto cake_model = loader.load<ModelLoader>("/home/morgoth/cpp/eto/assets/cadnav.com_model/Model_D0405211A19/D0405211A19.fbx", ws);
	if (! cake_model->isLoaded()) {
		std::cerr << cake_model->getErrorMessage() << std::endl;
		return 33;
	}

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	Camera camera(glm::vec2(w.getSize().x, w.getSize().y), glm::vec3(-4, 0, 1));

	Renderable cake(cake_model);
	cake.scale(glm::vec3(0.1));
	cake.translate(glm::vec3(-0, 0, -0));
	cake.rotate(90, glm::vec3(1, 0, 0));
	Renderable light(light_model);
	light.scale(glm::vec3(0.2));
	light.translate(glm::vec3(0, 0, 0));

	Light dirLight(Light::Directional, vec3(0), vec3(0, -2, 0));
	dirLight.apply(ws);
	dirLight.apply(sd);

	float d = 0;
	float dd = 0.05;
	Input &input = Input::getInstance();

	input.addCallback(GLFWevent::Type::CursorPosition, [&](const GLFWevent &e) {
			processMouse(e.data.cursorPos.x, e.data.cursorPos.y, camera);
			});

	input.addCallback(GLFWevent::Key, [&](const GLFWevent &e) {
			processKeyboard(static_cast<CameraMovement>(e.data.key.key), camera);
			});

	while (! w.shouldClose())
	{
		w.pollEvents();
		if (input.isKeyRelese(input::Escape))
			w.setShouldClose(true);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ws->use();
		ws->setMat4f("view", camera.getViewMatrix());
		ws->setVec3f("view_position", camera.getPos());

		cake.draw();
		//		cake.rotate(1, glm::vec3(1, 0, 0));
		light.draw();

		light.translate(glm::vec3(d, 0, 0));
		if (d < -0.5)
			dd = 0.005;
		else if (d > 0.5) 
			dd = -0.005;
		d += dd;

		w.swapBuffers();
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

	return 0;
}

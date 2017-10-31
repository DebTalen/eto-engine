#include <iostream>
#include <chrono>
#include <cstdio>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <core/Window.hpp>
#include <glm/glm.hpp>

#include <resources/ShaderLoader.hpp>
#include <resources/ModelLoader.hpp>
#include <graphcis/core/ShaderProgram.hpp>
#include <graphcis/core/Renderable.hpp>
#include <graphcis/core/Light.hpp>
#include <components/CFpsCamera.hpp>
#include <core/Entity.hpp>

using std::cout;
using std::endl;
using namespace eto;
using glm::vec3;

void printFPS()
{
	static float lastTime = glfwGetTime();
	static int numFrames = 0;
	double time = glfwGetTime();
	++numFrames;
	if ( time - lastTime >= 1.0 ){ // If last prinf() was more than 1 sec ago
		// printf and reset timer
		printf("%f ms/frame\n", 1000.0/double(numFrames));
		numFrames = 0;
		lastTime += 1.0;
	}
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
	//	glfwSwapInterval(0); // disable vsync and fixed frame rate

	AssetLoader loader = AssetLoader::getInstance();
	std::shared_ptr<ShaderProgram> sd = std::make_shared<ShaderProgram>();
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

	auto cake_model = loader.load<ModelLoader>("/home/morgoth/cpp/eto/assets/Asuka Soryu/School Uniform/Asuka School Uniform.obj", sd);
	if (! cake_model->isLoaded()) {
		std::cerr << cake_model->getErrorMessage() << std::endl;
		return 33;
	}

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	Entity camera;
	std::shared_ptr<CCamera> cam = camera.addComponent<CCamera>(glm::vec2(w.getSize().x, w.getSize().y), glm::vec3(-4, 0, 0));
	camera.addComponent<CFpsCamera>(camera);

	Renderable cake(cake_model);
	cake.scale(glm::vec3(0.05));
	cake.translate(glm::vec3(-0, 0, -0));
	//cake.rotate(90, glm::vec3(1, 0, 0));
	Renderable light(light_model);
	light.scale(glm::vec3(0.2));
	light.translate(glm::vec3(0, 0, 0));

	Light dirLight(Light::Directional, vec3(0), vec3(0, -2, 0));
	dirLight.apply(ws);
	dirLight.apply(sd);

	float d = 0;
	float dd = 0.05;
	Input &input = Input::getInstance();

	while (! w.shouldClose())
	{
		printFPS();
		w.pollEvents();
		if (input.isKeyRelease(input::Escape))
			w.setShouldClose(true);

	 	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ws->use();
		ws->setMat4f("view", cam->getViewMatrix());
		ws->setVec3f("view_position", cam->getPos());

		sd->use();
		sd->setMat4f("view", cam->getViewMatrix());
		sd->setVec3f("view_position", cam->getPos());
		cake.draw();

		light.translate(glm::vec3(d, 0, 0));
		if (d < -0.5)
			dd = 0.005;
		else if (d > 0.5) 
			dd = -0.005;
		d += dd;

		w.swapBuffers();
		//std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

	return 0;
}

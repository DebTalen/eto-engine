#include <iostream>
#include <memory>
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

#include <resources/AssetLoader.hpp>
#include <resources/ShaderLoader.hpp>
#include <graphcis/core/ShaderProgram.hpp>
#include <resources/ModelLoader.hpp>


using std::cout;
using std::endl;
using namespace eto;
//template <typename T>
//using SPtr = std::shared_ptr<T>;

using glm::vec3;
// default camera values
const float YAW   = -90.0f,
            PITCH = 0.0f,
	    SPEED = 13.0f,
	    SENS  = 0.07f,
	    ZOOM  = 45.0f;

class Camera
{
private:
	vec3 _pos, _front, _up, _right, _worldUp;
	float _yaw, _pitch;
	float _movSpeed, _mouseSens, _zoom;
	void updateCameraVectors()
	{
		_front = glm::normalize( vec3 { 
				cos(glm::radians(_yaw)) * cos(glm::radians(_pitch)),
				sin(glm::radians(_pitch)),
				sin(glm::radians(_yaw)) * cos(glm::radians(_pitch))
				});
		_right = glm::normalize(glm::cross(_front, _worldUp));
		_up    = glm::normalize(glm::cross(_right, _front));
	}
public:
	explicit Camera(vec3 position = vec3(0, 0, 0), vec3 up = vec3(0, 1, 0), float yaw = YAW, float pitch = PITCH)
		: _front(vec3(0, 0, -1.0)), 
		_movSpeed(SPEED),
		_mouseSens(SENS),
		_zoom(ZOOM) 
	{
		_pos = position;
		_worldUp = up;
		_yaw = yaw;
		_pitch = pitch;
		updateCameraVectors();
	}

	Camera(float posX, float posY, float posZ,
	       float upX,  float upY,  float upZ,
	       float yaw,  float pitch)
		: _front(vec3(0, 0, -1.0)),
		_movSpeed(SPEED),
		_mouseSens(SENS),
		_zoom(ZOOM)
	{
		_pos = vec3(posX, posY, posZ);
		_worldUp = vec3(upX, upY, upZ);
		_yaw = yaw;
		_pitch = pitch;
		updateCameraVectors();
	}

	glm::mat4 getView()
	{
		return glm::lookAt(_pos, _pos + _front, _up);
	}

	glm::vec3 getPos() const { return _pos; }

	enum CameraMovement {
		FORWARD  = Input::Key::W,
		BACKWARD = Input::Key::S,
		UPWARD   = Input::Key::Space,
		DOWNWARD = Input::Key::C,
		RIGHT    = Input::Key::D,
		LEFT 	 = Input::Key::A
	};
	void processKeyboard(CameraMovement direction, float deltaTime)
	{
		float velocity = _movSpeed * deltaTime;
		switch (direction)
		{
			case FORWARD:
				_pos += _front * velocity; break;
			case BACKWARD:
				_pos -= _front * velocity; break;
			case LEFT:
				_pos -= _right * velocity; break;
			case RIGHT:
				_pos += _right * velocity; break;
			case UPWARD:
				_pos += _up * velocity; break;
			case DOWNWARD:
				_pos -= _up * velocity; break;
		}
	//	_pos.y = 0; for fixed floor
	}

	void processMouse(float xOffset, float yOffset)
	{
		xOffset *= _mouseSens;
		yOffset *= _mouseSens;
		_yaw   += xOffset;
		_pitch += yOffset;

		if(_pitch > 89.0f)
			_pitch = 89.0f;
		if(_pitch < -89.0f)
			_pitch = -89.0f;
		updateCameraVectors();
	}
};

int main()
{
	glfwInit();
	Window w;
	w.create(1280, 720, "a");
	w.setInputMode(GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	w.setPos({0, 100});
	glfwMakeContextCurrent(w.getRawPointer());

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
	SPtr<ShaderProgram> ls = std::make_shared<ShaderProgram>();
	{
		auto vs = loader.load<ShaderLoader>("/home/morgoth/cpp/eto/shaders/default.vs", VertexShader);
		if (! vs->isCompiled())
			cout << vs->getErrorMessage() << endl;

		auto fs = loader.load<ShaderLoader>("/home/morgoth/cpp/eto/shaders/lightSource.fs", FragmentShader);
		if (! fs->isCompiled())
			cout << fs->getErrorMessage() << endl;
		ls->attachShader(*vs);
		ls->attachShader(*fs);
		ls->link();
		if (! ls->isLinked() )
			cout << ls->getErrorMessage() << endl;
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


	auto light = loader.load<ModelLoader>("/home/morgoth/cpp/eto/assets/testsphere.nff", ls);
	if (! light->isLoaded()) {
		std::cerr << light->getErrorMessage() << endl;
		return 31;
	}

	auto cacke1 = loader.load<ModelLoader>("/home/morgoth/cpp/eto/assets/cadnav.com_model/Model_D0405211A19/D0405211A19.fbx", ws);
	if (! cacke1->isLoaded()) {
		std::cerr << cacke1->getErrorMessage() << std::endl;
		return 33;
	}

 	auto cacke2 = loader.load<ModelLoader>("/home/morgoth/cpp/eto/assets/overwatch-dva/DVA.fbx", sd); 
	if (! cacke2->isLoaded()) {
		std::cerr << cacke2->getErrorMessage() << std::endl;
		return 33;
	}
 
	light->print();

 //	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	Camera camera(glm::vec3(0, 0, 3.0));
	bool firstMouse = GL_TRUE;
	float deltaTime = 0,
	      lastFrame = 0;
	float lastX = w.getSize().x / 2,
	      lastY = w.getSize().y / 2;

	glm::vec3 lightPos(0, 10, 0);
	glm::vec3 cacke1Pos(0, 0, -4);
	glm::vec3 cacke2Pos(1, 0, 0);
	glm::vec3 artasPos(-6, 0, 0);
	glm::mat4 projection = glm::mat4(1.0f);

	projection = glm::perspective(glm::radians(45.0f), (float)w.getSize().x / (float)w.getSize().y, 0.1f, 20000.0f);
	sd->use();
	sd->setMat4f("projection", projection);
	ls->use();
	ls->setMat4f("projection", projection);
	ws->use();
	ws->setMat4f("projection", projection);

	GLFWevent e;
	float r = 0;
	while (! w.shouldClose())
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		w.pollEvents();
		while (w.getEvent(e) ){
			if (e.type == GLFWevent::Type::WindowClose ||
					(e.type == GLFWevent::Type::Key && e.key.key == Input::Key::Escape))
				w.setShouldClose(1);
			else if (e.type == GLFWevent::Key)
				camera.processKeyboard(static_cast<Camera::CameraMovement>(e.key.key), deltaTime);
			if (e.type == GLFWevent::CursorPosition)
			{
				if (firstMouse)
				{
					lastX = e.cursorPos.x;
					lastY = e.cursorPos.y;
					firstMouse = GL_FALSE;
				}
				float xOffset = e.cursorPos.x - lastX,
				      yOffset = lastY - e.cursorPos.y;
				lastX = e.cursorPos.x;
				lastY = e.cursorPos.y;
				camera.processMouse(xOffset, yOffset);
			}
		}

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
/* 		sd->use();
		sd->setMat4f("view", view);
		sd->setVec3f("viewPos", camera.getPos());
		sd->setVec3f("light_position", lightPos);
		sd->setVec3f("light_ambient", vec3(0.5));

		lmodel = glm::mat4(1.0f);
		lmodel = glm::translate(lmodel, cacke2Pos);
		lmodel = glm::rotate(lmodel, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		lmodel = glm::rotate(lmodel, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		lmodel = glm::rotate(lmodel, glm::radians(r), glm::vec3(0.0f, 0.0f, 1.0f));
		r += 0.3;
		lmodel = glm::scale(lmodel, glm::vec3(0.02f));
		sd->setMat4f("model", lmodel);
		cacke2->draw();
 */

		glm::mat4 model = glm::mat4(1.0f);
		ls->use();
		model = glm::scale(model, glm::vec3(0.2));
		model = glm::translate(model, lightPos);
		ls->setMat4f("model", model);
		ls->setMat4f("view", camera.getView());
		light->draw();

		model = glm::mat4(1.0f);
		ws->use();
		model = glm::translate(model, cacke1Pos);
		model = glm::scale(model, glm::vec3(0.1));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1, 0, 0));
		ws->setMat4f("model", model);
		ws->setMat4f("view", camera.getView());
		ws->setVec3f("light_position", lightPos);
		ws->setVec3f("veiw_position", camera.getPos());
		cacke1->draw();

		sd->use();
		sd->setMat4f("view", camera.getView());
		sd->setVec3f("light_position", lightPos);
		sd->setVec3f("veiw_position", camera.getPos());

		model = glm::mat4(1.0f);
		model = glm::translate(model, cacke2Pos);
		model = glm::scale(model, glm::vec3(0.05));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1, 0, 0));
		model = glm::rotate(model, glm::radians(r), glm::vec3(0, 0, 1));
		r += 0.3;
		sd->setMat4f("model", model);
		cacke2->draw();

		w.swapBuffers();
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

	return 0;
}

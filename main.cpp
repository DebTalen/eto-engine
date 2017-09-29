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
	    SPEED = 200.0f,
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
				_pos.y += velocity; break;
			case DOWNWARD:
				_pos.y -= velocity; break;
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
	Window w;
	w.create(800, 800, "a");
	w.setInputMode(GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	w.setPos({200, 100});
	glfwMakeContextCurrent(w.getRawPointer());

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	glEnable(GL_DEPTH_TEST);

	AssetLoader loader;
	ShaderProgram sd;
	{
		auto vs = loader.load<ShaderLoader>("/home/morgoth/cpp/eto/shaders/default.vs", VertexShader);
		if (! vs->isCompiled())
			cout << vs->getErrorMessage() << endl;

		auto fs = loader.load<ShaderLoader>("/home/morgoth/cpp/eto/shaders/default.fs", FragmentShader);
		if (! fs->isCompiled())
			cout << fs->getErrorMessage() << endl;
		sd.attachShader(*vs);
		sd.attachShader(*fs);
		sd.link();
		if (! sd.isLinked() )
			cout << sd.getErrorMessage() << endl;
	}
 
	auto dragon = loader.load<ModelLoader>("/home/morgoth/cpp/eto/assets/Dragon/Dargon posing.obj");
	if (! dragon->isLoaded()) {
		std::cerr << dragon->getErrorMessage() << endl;
		return 31;
	}

 //	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	Camera camera(glm::vec3(0, 0, 3.0));
	bool firstMouse = GL_TRUE;
	float deltaTime = 0,
	      lastFrame = 0;
	float lastX = w.getSize().x / 2,
	      lastY = w.getSize().y / 2;

	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(45.0f), (float)w.getSize().x / (float)w.getSize().y, 0.1f, 20000.0f);
	GLFWevent e;
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

		sd.use();

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);

		float radius = 180.0f;
		float camX = sin(glfwGetTime()) * radius;
		float camZ = cos(glfwGetTime()) * radius;
		view = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));  
		model = glm::translate(model, glm::vec3(0, -30, 0));
		model = glm::scale(model, glm::vec3(30, 30, 30));

		view = glm::translate(view, glm::vec3(0, 0, -10));

		unsigned int modelLoc = sd.getUniformLocation("model");
		unsigned int viewLoc  = sd.getUniformLocation("view");
		unsigned int projLoc  = sd.getUniformLocation("projection");

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
 
		dragon->draw();

		w.swapBuffers();
	}

	return 0;
}

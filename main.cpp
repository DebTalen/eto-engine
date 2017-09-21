#include <iostream>
#include <memory>
#include <map>
#include <cstdio>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <core/Window.hpp>

#include <resources/AssetLoader.hpp>
#include <resources/ShaderLoader.hpp>

using std::cout;
using std::endl;
using namespace eto;
//template <typename T>
//using SPtr = std::shared_ptr<T>;


void error_callback(int error, const char *description)
{
	printf("%d %sn", error, description);
}


int main(void)
{	
   	Window W;
	W.setWinHint(GLFW_VISIBLE, 0);
	if (!W.create(100, 100, "roar"))
		return -2;

	glfwMakeContextCurrent(W.getRawPointer());
	if (! gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))  {
		printf("Failed to load GLAD");
		return -1;
	}

	AssetLoader loader;
	SPtr<Shader> s = loader.load<ShaderLoader>("/home/morgoth/cpp/eto/shaders/default.vs", ShaderType::Vertex);
	cout << s->isCompiled() << endl;
	if (! s->isCompiled())
		cout << s->getErrorMessage() << endl;
	return 0;
}

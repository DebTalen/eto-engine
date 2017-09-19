#include <iostream>
#include <memory>
#include <map>
#include <cstdio>
#include <resources/TextureLoader.hpp>
#include <resources/AssetLoader.hpp>
#include <util/FileStream.hpp>
#include <core/Window.hpp>

#include <stb_image_write/stb_image_write.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

void error_callback(int error, const char *description)
{
	printf("%d %sn", error, description);
}

using namespace eto;
using std::cout;
using std::endl;
/* 
template <typename T>
using SPtr = std::shared_ptr<T>;
template <typename T>
using WPtr = std::weak_ptr<T>;

 *//* struct ResourceHandle
{
	typedef int64_t Id;

	ResourceHandle() : id(++sNextID) {}
	Id id;
private:
	static Id sNextID;
};
ResourceHandle::Id ResourceHandle::sNextID = 0;

class Resource
{
public:
	ResourceHandle::Id getId() const { return m_handle.id; }
	virtual ~Resource() {}
	virtual void foo()
	{
		std::cout << "I'm base resource" << std::endl;
	}
protected:
	ResourceHandle m_handle;
};

class Texture : public Resource
{
public:
	Texture(const std::string &name) : m_name(name) {}
	void foo() override
	{
		std::cout << "I'm a texture!" << std::endl;
	}
	void bar()
	{
		std::cout << "My name is " << m_name << std::endl;
	}
private:
	std::string m_name;
};

class Dog : public Resource
{
public:
	void foo() override
	{
		std::cout << "Bork!" << std::endl;
	}
};

class Oppai : public Resource
{
public:
	void foo() override
	{
		std::cout << "Hentai! Baka!" << std::endl;
	}
	void bounce()
	{
		std::cout << "*Huge breasts bouncing sound*" << std::endl;
	}
};


class OppaiLoader
{
public:
	static SPtr<Oppai> load(const std::string &path)
	{
		return std::make_shared<Oppai>();
	}
};

class TextureLoader
{
public:
	static int count;
	static SPtr<Texture> load(const std::string &path)
	{
		std::cout<< ++count << std::endl;
		return std::make_shared<Texture>(path);
	}
};
int TextureLoader::count = 0;

class DogLoader
{
public:
	static SPtr<Dog> load(const std::string &path)
	{
		return std::make_shared<Dog>();
	}
};

class AssetLoader
{
public:
	template <typename TLoader>
	auto load(const std::string &path)
	{
		std::size_t h = std::hash<std::string>{}(path);
		auto iter = m_loaded.find(h);
		if (iter != m_loaded.end())
		{
			if (! iter->second.expired())
			{
				using type = std::remove_pointer_t<decltype(TLoader::load(path).get())>;
				return std::static_pointer_cast<type>(iter->second.lock());
			}
			auto pRes = TLoader::load(path);
			iter->second = pRes;
			return pRes;
		}
		auto pRes = TLoader::load(path);
		m_loaded.insert( std::pair<std::size_t, WPtr<Resource>>(h, std::static_pointer_cast<Resource>(pRes)) );
		return pRes;
	}
private:
	std::map<std::size_t, WPtr<Resource>> m_loaded;
};

 */

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

	const char *path = "/home/morgoth/cpp/eto/assets/tests/pngimage1.png";
	{
/* 		AssetLoader loader;

		auto t = loader.load<TextureLoader>(path);
		if (t == nullptr)
			return 108;
		
		std::vector<uchar> data;
		data.reserve(t->getDataSize());
		for (uint i = 0; i < data.capacity(); ++i)
			data.push_back( rand() % 255 );
		for (uint i = 0; i < 100; ++i)
			cout << data.at(i);
		cout << endl;

		if (t->write(data))
		{
			std::vector<uchar> data2;
			t->read(data2);
			for (int i = 0; i < 100; ++i)
				cout << data2.at(i);
			cout << endl;
		}
 */	}
	{
		int w = 64,
		    h = 64,
		    c = 4;
		int size = w * h * c;
		uchar im[size];
		
		for (int i = 0; i < size; i += c)
		{
			im[i] = 64;
			im[i + 1] = 64;
			im[i + 2] = 64;
			im[i + 3] = 255;
		}
		if (! stbi_write_png("r64g64b64a256.png", w, h, c, im, w))
			cout << "Failed to write to image" << endl;
	}

	return 0;
}

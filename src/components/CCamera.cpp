#include <components/CCamera.hpp>

using namespace eto;

CCamera::CCamera(const vec2 &size, const vec3 &position) 
	: m_active(true) 
{
	m_camera = std::make_unique<Camera>(size, position); 
}

CCamera::~CCamera()
{

}

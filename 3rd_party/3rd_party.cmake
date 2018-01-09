set(THIRD_PARTY_DIR ${CMAKE_SOURCE_DIR}/3rd_party)

# add GLFW
if(WIN32)
	set(GLFW_USE_STATIC_LIBS "1")
endif()
find_package(GLFW REQUIRED)

if ((NOT GLFW_LIBRARIES) OR (NOT EXISTS ${GLFW_INCLUDE_DIR}))
	message ("Unable to find GLFW library, trying to fetch from the github")

	execute_process(COMMAND git submodule update --init -- 3rd_party/glfw
			WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

	add_subdirectory(${THIRD_PARTY_DIR}/glfw)

	set(GLFW_ROOT_DIR "${THIRD_PARTY_DIR}/glfw")
	find_package(GLFW REQUIRED)

	if ((NOT GLFW_LIBRARIES) OR (NOT EXISTS ${GLFW_INCLUDE_DIR}))
		message("Critical error. Unable to find or download GLFW library")
	endif()
else()
	message("GLFW FOUND")
endif()

# add catch
set(CATCH_INCLUDE_DIR "${THIRD_PARTY_DIR}/Catch2")

if ((NOT CATCH_INCLUDE_DIR) OR (NOT EXISTS ${CATCH_INCLUDE_DIR}))
	message ("Unable to find CATCH library, trying to fetch from the github")

	execute_process(COMMAND git submodule update --init -- 3rd_party/Catch2
			WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

	set(CATCH_INCLUDE_DIR "${THIRD_PARTY_DIR}/Catch2/include"
		CACHE PATH "catch include directory")
else()
	message("CATCH FOUND")
endif()


# add loguru
set(LOGURU_INCLUDE_DIR "${THIRD_PARTY_DIR}/loguru")
find_path(LOGURU_INCLUDE_DIR "/usr/lib")

if((NOT LOGURU_INCLUDE_DIR) OR (NOT EXISTS ${LOGURU_INCLUDE_DIR}))
	message("Unable to find LOGURU library, trying to fetch from the github")

	execute_process(COMMAND git submodule update --init -- 3rd_party/loguru
			WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

	set(LOGURU_INCLUDE_DIR ${THIRD_PARTY_DIR}/loguru
		CACHE PATH "loguru include directory")
else()
	message("LOGURU FOUND")
endif()

#add glm

# GLM
set(GLM_ROOT_DIR "${THIRD_PARTY_DIR}/glm")
find_package(GLM REQUIRED)
if((NOT GLM_INCLUDE_DIR) OR (NOT EXISTS ${GLM_ROOT_DIR}))
	message("Unable to find GLM library, trying to fetch from the github")

	execute_process(COMMAND git submodule update --init -- 3rd_party/glm
			WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

	find_package(GLM REQUIRED)
	if ((NOT GLM_INCLUDE_DIR) OR (NOT EXISTS ${GLM_INCLUDE_DIR}))
		message("Critical error. Unable to find or download GLM library")
	endif()
else()
	message("GLM FOUND")
endif()

# add bullet 
# set(BULLET_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/external/bullet3/src")

# find_path(SPDLOG_INCLUDE_DIR "/usr/lib")

# if((NOT BULLET_INCLUDE_DIR) OR (NOT EXISTS ${BULLET_INCLUDE_DIR}))
# 	message("Unable to find bullet library")

# 	execute_process(COMMAND git submodule update --init -- external/bullet3
# 			WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
# 	set(BULLET_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external/bullet3/src
# 		CACHE PATH "bullet include directory")
# 	add_subdirectory(${CMAKE_SOURCE_DIR}/external/bullet3)

# 	set(BULLET_LIBRARIES 
#BulletMultiThreaded (optional)
#MiniCL (optional)
#BulletWorldImporter (optional)
#BulletSoftBody (optional) 
# 		BulletDynamics
# 		BulletCollision
# 		LinearMath)
# else()
# 	message("BULLET FOUND")
# endif()

# add 
unset(THIRD_PARTY_DIR)

cmake_minimum_required (VERSION 3.8)

include_directories(C:/GLAD/include C:/glfw-3.3.4/include C:/glm/glm)
include_directories(Headers)

set(GLAD "C:/GLAD/src/glad.c")
set(GLUT "C:/glutdlls36/glut.h")

set(SOURCE	"main.cpp" 
			"Source/Entities/Sphere.cpp" 
			"Source/Entities/Circle.cpp" 
			"Source/Tools/Camera.cpp" "Source/Entities/Lines.cpp")

set(HEADERS "Headers/Entities/Sphere.hpp" 
			"Headers/Entities/Circle.hpp" 
			"Headers/Tools/Camera.hpp" "Headers/Entities/Lines.hpp")

add_executable (Atom ${SOURCE} ${HEADERS} ${GLAD})

#OpenGl
target_link_libraries(Atom PRIVATE OpenGl32)

#GLFW
target_link_libraries(Atom PRIVATE C:/glfw-3.3.4/build/src/Debug/glfw3.lib)

#Glut
target_link_libraries(Atom PRIVATE C:/glutdlls36/glut.lib C:/glutdlls36/glut32.lib)
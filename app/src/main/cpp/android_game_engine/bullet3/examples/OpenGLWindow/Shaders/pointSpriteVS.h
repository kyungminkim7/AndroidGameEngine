//this file is autogenerated using stringify.bat (premake --stringify) in the build folder of this project
static const char* pointSpriteVertexShader =
	"#version 330\n"
	"precision highp float;\n"
	"layout (location = 0) in vec4 position;\n"
	"layout (location = 1) in vec4 instance_position;\n"
	"layout (location = 3) in vec2 uvcoords;\n"
	"layout (location = 4) in vec3 vertexnormal;\n"
	"layout (location = 5) in vec4 instance_color;\n"
	"layout (location = 6) in vec4 instance_scale_obUid;\n"
	"uniform float screenWidth = 700.f;\n"
	"uniform mat4 ModelViewMatrix;\n"
	"uniform mat4 ProjectionMatrix;\n"
	"out Fragment\n"
	"{\n"
	"     vec4 color;\n"
	"} fragment;\n"
	"//\n"
	"// vector rotation via quaternion\n"
	"//\n"
	"out vec3 ambient;\n"
	"void main(void)\n"
	"{\n"
	"	ambient = vec3(0.3,.3,0.3);\n"
	"		\n"
	"		\n"
	"	vec4 axis = vec4(1,1,1,0);\n"
	"	vec4 vertexPos = ProjectionMatrix * ModelViewMatrix *(instance_position);\n"
	"	vec3 posEye = vec3(ModelViewMatrix * vec4(instance_position.xyz, 1.0));\n"
	"   float dist = length(posEye);\n"
	"	float pointRadius = 1.f;\n"
	"    gl_PointSize = instance_scale_obUid.x * pointRadius * (screenWidth / dist);\n"
	"	gl_Position = vertexPos;\n"
	"	\n"
	"	fragment.color = instance_color;\n"
	"}\n";

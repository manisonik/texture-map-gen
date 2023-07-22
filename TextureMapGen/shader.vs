#version 440 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexColor;
layout(location = 2) in vec2 vertexUV;
layout(location = 3) in uint drawId;
  
// Values that stay constant for the whole mesh.
uniform mat4 MVP;

// Output data
out vec3 color;
out vec2 uv;
flat out uint drawID;

void main()
{
    gl_Position = MVP * vec4(vertexPosition_modelspace, 1.0);

	// Pass on the fragment shader
	color = vertexColor;

	uv = vertexUV;

	drawID = drawId;
}
#version 440 core

layout (binding = 0) uniform sampler2DArray textureArray;

// Input color from vertex shader
in vec3 color;
flat in uint drawID;
in vec2 uv;

void main()
{
	 gl_FragColor = vec4(color.rgb, 1.0f) * texture(textureArray, vec3(uv.x, uv.y, drawID)).rgba;
} 
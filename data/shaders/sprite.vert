#version 330 core
layout (location = 0) in vec4 vertex; // <vec2 position, vec2 texCoords>
layout (location = 1) in mat4 model;  // mat4 takes 4 locations, (4*vec4)
layout (location = 5) in vec4 color;  // so we jump to location 5

out vec2 TexCoords;
out vec4 Color;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    Color = color;
    TexCoords = vertex.zw;
    gl_Position = projection * view * model * vec4(vertex.xy, 0.0, 1.0);
}
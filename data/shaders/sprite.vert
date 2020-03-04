#version 330 core
layout (location = 0) in vec4 vertex; // <vec2 position, vec2 texCoords>
layout (location = 1) in mat4 model;  // mat4 takes 4 locations, (4*vec4)
layout (location = 5) in vec4 color;  // so we jump to location 5
layout (location = 6) in vec3 normal;  // so we jump to location 5

out vec2 TexCoords;
out vec4 Color;
out vec3 Normal;
out vec2 Position;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    Color = color;
    Normal = normal;
    TexCoords = vertex.zw;
    gl_Position = projection * view * model * vec4(vertex.xy, 0.0, 1.0);
    Position = vec2( model * vec4(vertex.xy, 0, 1.0));
}
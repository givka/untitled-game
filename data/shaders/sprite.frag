#version 330 core
in vec2 TexCoords;
in vec4 Color;

out vec4 color;

uniform sampler2D image;
uniform vec3 spriteColor;







void main()
{
    color = Color * texture(image, TexCoords);

}


//color = vec4(gl_FragCoord.xy/vec2(1280, 720), 1, 1);

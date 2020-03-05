#version 330 core
in vec2 TexCoords;
in vec4 Color;
in vec3 Normal;
in vec2 Position;

out vec4 color;

uniform sampler2D image;
uniform vec3 spriteColor;
uniform vec3 sun;
uniform float time;

vec3 getColor(float noise)
{
    if (noise < 0.)
    return vec3(0, 0, 1);
    if (noise < 0.1)
    return vec3(1, 1, 0);
    if (noise < 0.6)
    return vec3(0.5, 1, 0.5);
    if (noise < 0.6)
    return vec3(0.6, 0.1, 0.1);
    if (noise < 0.8)
    return vec3(1, 1, 1);

    return vec3(1, 1, 1);
}


vec3 calcNormal() {
    return normalize((Color.w+0.5)*Normal+sun);
}

void main()
{
    vec3 normal = Normal;
    vec3 col = getColor(Color.w);
    if (Color.w < 0.0){
        normal = calcNormal();
/*
        vec2 pos = gl_FragCoord.xy/vec2(1280, 720);
        pos = pos - vec2(0.5) - vec2(sun.x, - sun.y);
        float power = length(pos);
        col += vec3(0.01*1/power);

  */
    }

    color = vec4(dot(normalize(sun), normal)*col, 1.0) * texture(image, TexCoords);
}

//color = vec4(gl_FragCoord.xy/vec2(1280, 720), 1, 1);

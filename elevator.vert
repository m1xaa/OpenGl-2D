#version 330 core

layout(location = 0) in vec2 inPos;
layout(location = 1) in vec3 inCol;
layout(location = 2) in float isDoor;
out vec4 chCol;

uniform float uY;
uniform float doorsuY;


void main()
{
    float x = inPos.x;
    float y = inPos.y + uY;
    if (isDoor == 1) {
        y = y + doorsuY;
    }
    gl_Position = vec4(x, y, 0.0, 1.0);
    chCol = vec4(inCol, 1.0);
}
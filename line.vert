#version 330 core

layout (location = 0) in vec2 aPos;

uniform vec2 uOffset;   // pozicija pomeranja
uniform vec2 uScale;    // skala za širinu linije

void main()
{
    vec2 pos = aPos * uScale + uOffset;
    gl_Position = vec4(pos, 0.0, 1.0);
}

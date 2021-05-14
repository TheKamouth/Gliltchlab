#version 330 core

attribute vec4 aPosition;
attribute vec2 aTexCoord;
out vec2 vTexCoord;

void main(void)
{
    gl_Position = aPosition;
    vTexCoord = aTexCoord;
}

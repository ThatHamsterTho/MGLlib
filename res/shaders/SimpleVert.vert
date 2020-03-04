#version 330 core 

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec4 Color;

uniform vec4 u_Scale = vec4(1.0);

out vec2 v_TexCoord;
out vec4 v_Color;

uniform mat4 u_MVP; // model view projection

void main() {

  gl_Position = u_MVP * ((position * u_Scale) + u_Scale);
  v_TexCoord = texCoord;
  v_Color = Color;
}
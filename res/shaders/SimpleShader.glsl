#shader vertex
#version 330 core 

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec4 position;
layout(location = 1) in vec3 texCoord;
layout(location = 2) in vec4 Color;

uniform vec4 u_Scale = vec4(1.0);

out vec3 v_TexCoord;
out vec4 v_Color;

uniform mat4 u_MVP; // model view projection

void main() {

  gl_Position = (u_MVP * (position * u_Scale));
  v_TexCoord = texCoord;
  v_Color = Color;
}


#shader fragment
#version 330 core 

// Ouput data
layout(location = 0) out vec4 color;

in vec3 v_TexCoord;
in vec4 v_Color;

uniform vec4 u_Color = vec4(1.0);
uniform bool u_Use_Texture = false;
uniform sampler3D u_Texture;

void main() {
  vec4 set_color = vec4(1.0);
  // if the passed color is not white, use the passed color
  if (u_Color != vec4(1.0)) {
    set_color = u_Color;
  }
  // else use the color given through vbo
  else {
    set_color = v_Color;
  }

  // determine texture color / shape color
  vec4 texColor = texture(u_Texture, v_TexCoord);
  if (u_Use_Texture) {
    color = texColor * set_color;
  } else {
    color = set_color;
  }
}

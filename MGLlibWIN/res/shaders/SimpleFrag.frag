#version 330 core 

// Ouput data
layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in vec4 v_Color;

uniform vec4 u_Color = vec4(1.0);
uniform bool u_Use_Texture = false;
uniform sampler2D u_Texture;

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

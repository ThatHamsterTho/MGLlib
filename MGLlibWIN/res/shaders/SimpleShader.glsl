#shader vertex
#version 330 core 

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec4 position;
layout(location = 1) in vec4 Color;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in float texSlot;

uniform vec4 u_Scale = vec4(1.0);

out vec4 v_Color;
out vec2 v_TexCoord;
out float v_texSlot;

uniform mat4 u_MVP; // model view projection

void main() {

  gl_Position = (u_MVP * (position * u_Scale));
  v_Color = Color;
  v_TexCoord = texCoord;
  v_texSlot = texSlot;
}


#shader fragment
#version 330 core 

// Ouput data
layout(location = 0) out vec4 FragColor;

in vec4 v_Color;
in vec2 v_TexCoord;
in float v_texSlot;

uniform vec4 u_Color = vec4(1.0);
uniform bool u_Use_Texture = false;
uniform sampler2D u_Texture[8];

vec4 color;

void getTexColor(in int index, out vec4 TextureColor){
  if(index == 0){
    TextureColor = texture(u_Texture[0], v_TexCoord);
  }
  else if(index == 1){
    TextureColor = texture(u_Texture[1], v_TexCoord);
  }
  else if(index == 2){
    TextureColor = texture(u_Texture[2], v_TexCoord);
  }
  else if(index == 3){
    TextureColor = texture(u_Texture[3], v_TexCoord);
  }
  else if(index == 4){
    TextureColor = texture(u_Texture[4], v_TexCoord);
  }
  else if(index == 5){
    TextureColor = texture(u_Texture[5], v_TexCoord);
  }
  else if(index == 6){
    TextureColor = texture(u_Texture[6], v_TexCoord);
  }
  else if(index == 7){
    TextureColor = texture(u_Texture[7], v_TexCoord);
  }
}

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

  set_color = u_Color * v_Color;
  // determine texture color / shape color
  int index = int(v_texSlot);
  vec4 texColor;
  getTexColor(index, texColor);
  if (u_Use_Texture) {
    color = texColor * set_color;
  } else {
    color = set_color;
  }

  if (color.a == 0.0){
      discard;
  }

  FragColor = color;
}
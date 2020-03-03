#shader vertex
#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

out vec2 v_TexCoord;

uniform mat4 u_MVP;	// model view projection

void main(){

    gl_Position = u_MVP * position;
	v_TexCoord = texCoord;
}



#shader fragment
#version 330 core

// Ouput data
layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

uniform vec4 u_Color = vec4(1.0, 1.0, 1.0, 1.0);
uniform int u_Use_Texture = 0;
uniform sampler2D u_Texture;

void main()
{
	vec4 texColor = texture(u_Texture, v_TexCoord);
	if(u_Use_Texture == 1){
		color = texColor * u_Color;
	}
	else{
		color = u_Color;
	}
}


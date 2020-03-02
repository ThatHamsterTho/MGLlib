#shader vertex
#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec4 vertexPosition_modelspace;

void main(){

    gl_Position = vertexPosition_modelspace;

}



#shader fragment
#version 330 core

// Ouput data
layout(location = 0) out vec4 color;

uniform vec4 u_Color;

void main()
{

	// Output color = red 
	color = u_Color;

}


#version 130

in vec2 vertexPosition;
in vec4 vertexColor;
in vec2 vertexUV;

out vec2 fragmentPosition;
out vec4 fragmentColor;
out vec2 fragmentUV;

uniform mat4 P;

void main() {
	//set the x,y position on the screen.
	gl_Position.xy = (P * vec4(vertexPosition, 0.0, 1.0)).xy;
	//the z position is zero since this is 2D
	gl_Position.z = 0.0;

	//Indicate coordinates are normalized.
	gl_Position.w = 1.0;
	
	fragmentPosition = vertexPosition;

	fragmentColor = vertexColor;

	fragmentUV = vertexUV;
}
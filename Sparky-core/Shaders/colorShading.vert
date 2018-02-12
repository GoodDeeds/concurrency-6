// The program is in GLSL
                                           // vertex shader operates on each vertex   
#version 130

in vec2 vertexPosition;                   // grab vertexPosition from vertex buffer object, each vertex is 2 floats
					  // and it is vertexLocation = 0

in vec4 vertexColor;                       // vertexLocation = index = 1 as it is defined second
in vec2 vertexUV;

out vec4 fragmentColor;	
out vec2 fragmentPosition;
out vec2 fragmentUV;	

uniform mat4 P;					

void main(){
	gl_Position.xy = (P * vec4(vertexPosition, 0.0, 1.0)).xy;
	gl_Position.z = 0.0;
	gl_Position.w = 1.0;             // indicate the coordinates are normalized

	fragmentPosition = vertexPosition;
	fragmentColor = vertexColor;
	fragmentUV = vec2(vertexUV.x, 1.0 - vertexUV.y);
}
// The program is in GLSL
                                           // vertex shader operates on each vertex   
#version 130

in vec2 vertexPosition;                   // grab vertexPosition from vertex buffer object, each vertex is 2 floats
					  // and it is vertexLocation = 0

in vec4 vertexColor;

out vec4 fragmentColor;								

void main(){
	gl_Position.xy = vertexPosition;
	gl_Position.z = 0.0;
	gl_Position.w = 1.0;             // indicate the coordinates are normalized

	fragmentColor = vertexColor;
}
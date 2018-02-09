#version 130

in vec4 fragmentColor;
                                        // frag shader operates on each pixel in our polygon
out vec4 color;                         // 4 component float vector for each pixel

void main(){
	color = fragmentColor;	
}
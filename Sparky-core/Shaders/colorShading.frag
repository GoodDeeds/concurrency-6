#version 130

in vec2 fragmentPosition;
in vec4 fragmentColor;
in vec2 fragmentUV;
                                        // frag shader operates on each pixel in our polygon
out vec4 color;                         // 4 component float vector for each pixel


//uniform float time;


uniform sampler2D mySampler;

void main(){
	
	vec4 textureColor = texture(mySampler, fragmentUV);
	
	/*color = vec4(fragmentColor.r * (cos(fragmentPosition.x +time)+1.0) * 0.5,
		     fragmentColor.r * (cos(fragmentPosition.y +time)+1.0) * 0.5,
		     fragmentColor.r * (cos(fragmentPosition.x*0.4+time)+1.0) * 0.5,
		     fragmentColor.a) * textureColor;*/

	color = fragmentColor * textureColor;
}
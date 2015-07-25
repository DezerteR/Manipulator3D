#ifdef COMPILING_VERTEX_SHADER

layout(location=0)in vec4 mVertex;

out vec2 vUV;

void main(){
	gl_Position = (vec4(mVertex.xy,0,1));
	vUV = (mVertex.xy+vec2(1,1))/2;
}


#endif

#ifdef COMPILING_FRAGMENT_SHADER
out vec4 outColor;

uniform sampler2D uTexture;
uniform float uBlurSize;
uniform vec2 uScreenSize;

in vec2 vUV;

void main(void){

    vec4 sum = vec4(0.0);
    vec2 position = vUV;
		float radius = uBlurSize*1.0/uScreenSize.y*1;
    sum += texture(uTexture, vec2(position.x, position.y - 4.0*radius)) * 0.05;
    sum += texture(uTexture, vec2(position.x, position.y - 3.0*radius)) * 0.09;
    sum += texture(uTexture, vec2(position.x, position.y - 2.0*radius)) * 0.12;
    sum += texture(uTexture, vec2(position.x, position.y - radius)) * 0.15;
    sum += texture(uTexture, vec2(position.x, position.y)) * 0.16;
    sum += texture(uTexture, vec2(position.x, position.y + radius)) * 0.15;
    sum += texture(uTexture, vec2(position.x, position.y + 2.0*radius)) * 0.12;
    sum += texture(uTexture, vec2(position.x, position.y + 3.0*radius)) * 0.09;
    sum += texture(uTexture, vec2(position.x, position.y + 4.0*radius)) * 0.05;
    outColor = sum;
}

#endif


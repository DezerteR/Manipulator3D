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

in vec2 vUV;

uniform sampler2D uTexture;

void main(void){
	vec4 color = texture(uTexture, vUV);
	// float y = vUV.y*250*3.14;
	// color.rgb = pow(color.rgb, vec3(1.0/2.2)*((vUV.x+vUV.y)*0.5-2)*0.3);
	// vec2 uv = abs(vUV*2 - vec2(1));
	// color.rgb = vec3(1.0)*(uv.x+uv.y);
	// color.rgb = vec3(1.0)*length(uv);
	// color.rgb = pow(color.rgb, vec3(1.0/2.4))*(1-length(uv));
	// color.rgb = pow(color.rgb, vec3(1.0/2.2))*0.8;

	outColor = color;
}

#endif


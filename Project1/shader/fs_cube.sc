$input v_position, v_texcoord0

#include "common.sh"

SAMPLER2D(texColor,  0);

void main()
{	
	vec3 color = texture2D(texColor,v_texcoord0).rgb;

	gl_FragData[0] = vec4(color,1.0);
	gl_FragData[1] = vec4(1.0,1.0,0.0,1.0);
	gl_FragData[2] = vec4_splat(0.2);
}

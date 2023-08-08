$input v_position, v_texcoord0

#include "common.sh"

SAMPLER2D(texColor,  0);

void main()
{	
	vec3 color = texture2D(texColor,v_texcoord0).rgb;

	gl_FragColor = vec4(color,1.0);
}

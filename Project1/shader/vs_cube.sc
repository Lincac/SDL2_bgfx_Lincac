$input a_position, a_normal, a_tangent, a_texcoord0
$output v_position, v_texcoord0

#include "common.sh"

void main()
{
	gl_Position = mul(u_modelViewProj, vec4(a_position, 1.0));

	vec3 world_position = mul(u_model[0],vec4(a_position,1.0)).xyz;

	v_position = world_position;
	v_texcoord0 = a_texcoord0;
}

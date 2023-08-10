$input v_texcoord0

#include "common.sh"

SAMPLER2D(_MainTex, 0);

void main(){
    vec3 color = texture2D(_MainTex,v_texcoord0).rgb;
    gl_FragColor = vec4(color,1.0);
}
$input v_texcoord0

#include "common.sh"

SAMPLER2D(_MainTex, 0);

void main(){
    gl_FragColor = texture2D(_MainTex,v_texcoord0);
}
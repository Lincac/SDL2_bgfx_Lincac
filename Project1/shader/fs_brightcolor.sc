$input v_texcoord0

#include "common.sh"

SAMPLER2D(s_lightingColor, 0);

void main(){
    vec3 color = texture2D(s_lightingColor,v_texcoord0).rgb;
    float brightness = dot(color,vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0){
        gl_FragColor = vec4(color,1.0);
    }
    gl_FragColor = vec4_splat(0.0);
}
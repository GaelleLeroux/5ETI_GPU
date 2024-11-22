#version 330 core

in vec2 uv;

out vec4 color;

uniform sampler2D myTextureSampler;

const int samples = 35,
          LOD = 2,         // gaussian done on MIPmap at scale LOD
          sLOD = 1 << LOD; // tile size = 2^LOD
const float sigma = float(samples) * .25;

float gaussian(vec2 i) {
    return exp( -.5* dot(i/=sigma,i) ) / ( 6.28 * sigma*sigma );
}

vec4 blur(sampler2D sp, vec2 U, vec2 scale) {
    vec4 O = vec4(0);  
    int s = samples/sLOD;
    
    for ( int i = 0; i < s*s; i++ ) {
        vec2 d = vec2(i%s, i/s)*float(sLOD) - float(samples)/2.;
        O += gaussian(d) * texture( sp, U + scale * d , float(LOD) );
    }
    
    return O / O.a;
}


void main()
{
    color = blur(myTextureSampler,uv,vec2(0.001, 0.001));
}
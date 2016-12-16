attribute vec4 vPosition;
attribute vec4 vNorm;
attribute vec2 uv;
uniform mat4 ctm;

varying vec3 norm_old;
varying vec3 position;
varying vec2 uv_old;

void main() 
{
    gl_Position = vPosition * ctm;
    position = vPosition.xyz;
    norm_old = vNorm.xyz;
    uv_old = uv;
}

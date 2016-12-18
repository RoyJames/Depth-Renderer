uniform vec4 viewer, lightpos;
uniform vec4 light_amb, light_diff, light_spec;
uniform vec4 mat_amb, mat_diff, mat_spec;
uniform float phong;

varying vec3 norm_old;
varying vec3 position;

// product of components, which we will use for shading calculations:
vec4 product(vec4 a, vec4 b) {
    return vec4(a[0]*b[0], a[1]*b[1], a[2]*b[2], a[3]*b[3]);
}

void main() 
{    
    vec4 ambientColor = vec4(0,0,0,0),
     	 diffuseColor = vec4(0,0,0,0),
     	 specularColor = vec4(0,0,0,0);
    vec3 norm = normalize(norm_old);

    vec3 lightVector = normalize(lightpos.xyz - position);
    vec3 ray = normalize(position - viewer.xyz);
    vec3 hVector = normalize(lightVector - ray);

    float l_n = dot(lightVector, norm);
    float h_n = dot(hVector, norm);
    float phong_val = pow(max(0, h_n), phong);

    ambientColor = product(mat_amb, light_amb);
	diffuseColor = product(vec4(1,1,1,0), light_diff) * max(0, l_n);
    specularColor = product(mat_spec, light_spec) * phong_val;

    gl_FragColor = ambientColor + diffuseColor + specularColor;
} 


//Source borrowed from:
//http://pastebin.com/WbVNtMg7

attribute vec4 vPosition;
attribute vec4 vNorm;

uniform vec4 viewer, lightpos;
uniform vec4 light_amb, light_diff, light_spec;
uniform vec4 mat_amb, mat_diff, mat_spec;
uniform float phong;

uniform mat4 ctm;

varying vec4 color;

uniform int worldTime;

// product of components, which we will use for shading calculations:
vec4 product(vec4 a, vec4 b)
{
    return vec4(a[0]*b[0], a[1]*b[1], a[2]*b[2], a[3]*b[3]);
}
 
const float WORLD_RADIUS         = 250.0;
const float WORLD_RADIUS_SQUARED = 62500.0;
 
void main() {
        vec4 ambientColor = vec4(0,0,0,0),
        diffuseColor = vec4(0,0,0,0),
        specularColor = vec4(0,0,0,0);

        vec3 position = vec3(vPosition[0], vPosition[1], vPosition[2]);
        vec3 norm = normalize(vec3(vNorm[0], vNorm[1], vNorm[2]));
        vec3 viewer_ = vec3(viewer[0], viewer[1], viewer[2]);
        vec3 lightpos_ = vec3(lightpos[0], lightpos[1], lightpos[2]);

        vec3 lightVector = normalize(lightpos_ - position);
        vec3 ray = normalize(position - viewer_);
        vec3 hVector = normalize(lightVector - ray);

        float l_n = dot(lightVector, norm);
        float h_n = dot(hVector, norm);
        float phong_val = pow(max(0, h_n), phong);

        ambientColor = product(mat_amb, light_amb);
        diffuseColor = product(mat_diff, light_diff) * max(0, l_n);
        specularColor = product(mat_spec, light_spec) * phong_val;

        color = ambientColor + diffuseColor + specularColor;

        //Modify distance
        vec4 position2 = vPosition;
        float distanceSquared = position2.x * position2.x + position2.z * position2.z;
        distanceSquared *= 2.0f;
        position2.y += 20.0f*sin(distanceSquared*sin(float(worldTime)/143.0)/1000.0f);
        float y = position.y;
        float x = position.x;
        float om = sin(distanceSquared*sin(float(worldTime)/256.0)/5000.0) * sin(float(worldTime)/200.0);
        position2.y = x*sin(om)+y*cos(om);
        position2.x = x*cos(om)-y*sin(om);

        gl_Position = position2 * ctm;
                      
        gl_FogFragCoord = gl_Position.z;
}
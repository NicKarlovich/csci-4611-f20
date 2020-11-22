#version 330

in vec3 position_in_eye_space;  // v = (model_view_matrix * vec4(vertex, 1)).xyz
in vec3 normal_in_eye_space;    // n = normalize((normal_matrix * vec4(normal,0)).xyz)

out vec4 color;

uniform vec3 light_in_eye_space;
uniform vec4 Ia, Id, Is;

uniform vec4 ka, kd, ks;
uniform float s;


void main() {
    vec3 v = (position_in_eye_space);
    vec3 n = normalize(normal_in_eye_space);
    vec3 l = normalize(light_in_eye_space - v);
    
    //eye vector
    vec3 e = normalize(vec3(0, 0, 0) - v);

    //halfway
    vec3 h = normalize(l + e);

    vec4 ambient = ka * Ia;

    vec4 diffuse = kd * Id * max(dot(n, l), 0);
    vec4 specular = ks * Is * pow(max(dot(h, n), 0), s);

    color = ambient + diffuse + specular;
}

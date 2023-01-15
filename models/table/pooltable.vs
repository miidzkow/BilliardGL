#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 FragPos;
out vec3 Normal;
out float intensity;  // new varying variable for intensity

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightPos; // new uniform variable for light position

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
    vec3 norm = normalize(mat3(view) * Normal);  // transforming normal to eye space
    vec3 toLight = normalize(vec3(view * vec4(lightPos, 1.0)) - FragPos);
    intensity = dot(norm, toLight);  // calculate dot product for intensity
    gl_Position = projection * view * vec4(FragPos, 1.0);
}

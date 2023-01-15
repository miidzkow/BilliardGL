#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in float intensity;  // new input variable

uniform sampler2D texture_diffuse1;
uniform vec3 lightColor;
uniform vec3 objectColor;

void main()
{
    vec4 texel = texture(texture_diffuse1, TexCoords);
    vec3 diffuse = lightColor * objectColor * intensity;
    FragColor = vec4(texel.rgb * diffuse, texel.a);
}

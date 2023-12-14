#version 410

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord; // Received from the vertex shader

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor = vec3(1.0, 1.0, 1.0);
uniform float ambientStrength = 0.1;
uniform float specularStrength = 0.5;
uniform float shininess = 32.0;

out vec4 fragment_color;

uniform sampler2D texture00; // New uniform for the diffuse albedo texture

void main() {
    // Ambient lighting
    vec3 ambient = ambientStrength * lightColor;

    // Diffuse lighting
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
vec3 diffuse = diff * lightColor;

    // Specular reflection
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = specularStrength * spec * lightColor;

    // Sample the texture
    vec3 textureColor = texture(texture00, TexCoord).xyz;

    // Final color calculation
    vec3 result = ambient + (diffuse * textureColor) + specular;
    fragment_color = vec4(textureColor, 1.0);
}

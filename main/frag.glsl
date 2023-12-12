#version 410

in vec3 FragPos; // Received from the vertex shader
in vec3 Normal;  // Received from the vertex shader

uniform vec3 lightPos;    // Light position
uniform vec3 viewPos;     // Camera position (viewer position)

uniform vec3 lightColor = vec3(1.0, 1.0, 1.0);  // Light color
uniform float ambientStrength = 0.1;              // Ambient light strength
uniform float specularStrength = 0.5;             // Specular reflection strength
uniform float shininess = 32.0;                   // Specular shininess exponent

out vec4 fragment_color; // RGBA color

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

    vec3 result = ambient + diffuse + specular;
    fragment_color = vec4(result, 1.0);
}

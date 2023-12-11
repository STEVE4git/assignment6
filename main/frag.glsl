#version 330 core

in vec3 frag_normal;
in vec2 frag_texcoord;

out vec4 frag_colour;

uniform vec3 light_position;
uniform vec3 light_color;
uniform vec3 view_position;
uniform vec3 ambient_color;

uniform sampler2D texture_diffuse;
uniform sampler2D texture_normal;
uniform bool use_normal_map; // New uniform for toggling the normal map

void main() {
    vec3 normal;
    
    if (use_normal_map) {
        // Sample the normal map and convert the values to a normal vector
        normal = normalize(texture(texture_normal, frag_texcoord).xyz * 2.0 - 1.0);
    } else {
        // Use the vertex normal if the normal map is not enabled
        normal = normalize(frag_normal);
    }

    vec3 light_direction = normalize(light_position - frag_position);
    vec3 view_direction = normalize(view_position - frag_position);
    vec3 half_vector = normalize(light_direction + view_direction);

    float diff = max(dot(normal, light_direction), 0.0);
    float spec = pow(max(dot(normal, half_vector), 0.0), 32.0); // You can adjust the exponent for shininess

    // Diffuse lighting with texture
    vec3 diffuse_color = texture(texture_diffuse, frag_texcoord).xyz;
    vec3 diffuse_term = diff * diffuse_color * light_color;

    // Specular reflection
    vec3 specular_term = spec * vec3(1.0, 1.0, 1.0); // Set a fixed specular color

    // Ambient lighting
    vec3 ambient_term = ambient_color * diffuse_color;

    // Final color
    vec3 final_color = ambient_term + diffuse_term + specular_term;
    frag_colour = vec4(final_color, 1.0);
}

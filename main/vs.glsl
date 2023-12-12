#version 410

layout (location = 0) in vec3 vtxPosition;
layout (location = 1) in vec3 vtxNormal; // Add normal attribute

uniform mat4 proj_mat, view_mat, model_mat;

out vec3 FragPos; // Pass the fragment position to the fragment shader
out vec3 Normal;  // Pass the normal to the fragment shader

void main() {
    mat4 modelViewProj = proj_mat * view_mat * model_mat;
    
    FragPos = vec3(model_mat * vec4(vtxPosition, 1.0));
    Normal = normalize(mat3(transpose(inverse(model_mat))) * vtxNormal); // Transform normal to world space

    gl_Position = modelViewProj * vec4(vtxPosition, 1.0);
}

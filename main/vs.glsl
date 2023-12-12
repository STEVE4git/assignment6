#version 410

layout (location = 0) in vec3 vtxPosition;
layout (location = 1) in vec3 vtxNormal;
layout (location = 2) in vec2 vtxTexCoord; // New attribute for texture coordinates

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoord; // Pass the texture coordinates to the fragment shader

uniform mat4 proj_mat, view_mat, model_mat;

void main() {
    mat4 modelViewProj = proj_mat * view_mat * model_mat;
    
    FragPos = vec3(model_mat * vec4(vtxPosition, 1.0));
    Normal = normalize(mat3(transpose(inverse(model_mat))) * vtxNormal);
    TexCoord = vtxTexCoord; // Pass the texture coordinates to the fragment shader

    gl_Position = modelViewProj * vec4(vtxPosition, 1.0);
}

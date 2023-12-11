#version 410

layout (location = 0) in vec3 vtxPosition;


uniform mat4 proj_mat, view_mat, model_mat;


void main () {
    mat4 modelViewProj = proj_mat * view_mat * model_mat;
    gl_Position = modelViewProj * vec4(vtxPosition, 1.0);
}

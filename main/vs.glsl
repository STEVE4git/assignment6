#version 410

layout (location = 0) in vec3 vtxPosition;


uniform mat4 proj_mat, view_mat, model_mat;


void main () {
	gl_Position = vec4(vtxPosition, 1.0);
}

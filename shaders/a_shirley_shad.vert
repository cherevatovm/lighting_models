#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal_in;
layout (location = 2) in vec3 tangent_in;
out vec3 normal;
out vec3 tangent;
out vec3 bitangent;
out vec3 frag_pos;
uniform float scale;

uniform struct Transform {
	mat4 model;
	mat4 view;
	mat4 projection;
	mat3 normal_mat;
} transform;

void main(void)
{	
	normal = normalize(transform.normal_mat * normal_in);
	tangent = normalize(transform.normal_mat * tangent_in);
	bitangent = normalize(transform.normal_mat * cross(normal_in, tangent_in));
	
	frag_pos = vec3(transform.model * vec4(scale * position, 1.0f));
	gl_Position = transform.projection * transform.view * vec4(frag_pos, 1.0);
}
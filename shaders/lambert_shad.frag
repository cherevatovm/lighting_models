#version 330 core
in vec3 normal;
in vec3 frag_pos;
out vec4 frag_color;

uniform struct Light {
	vec3 diffuse;
    vec3 position;
} light;

void main(void)
{
    vec3 light_dir = normalize(light.position - frag_pos);
    float norm_d_light = max(dot(normal, light_dir), 0.0f); 
    frag_color = vec4(light.diffuse * norm_d_light, 1.0f);
}
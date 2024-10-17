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
    float diffuse = 0.2f + max(dot(normal, light_dir), 0.0f);
    vec3 fr_color = light.diffuse;
    if (diffuse < 0.4f)
        fr_color *= 0.4f;
    else if (diffuse >= 0.8f)
        fr_color *= 1.2f;
    frag_color = vec4(fr_color, 1.0f);
}
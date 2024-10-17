#version 330 core
in vec3 normal;
in vec3 frag_pos;
out vec4 frag_color;
uniform vec3 view_pos;

uniform struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
    float sheen_coef;
} material;

uniform struct Light {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
    vec3 position;
} light;

void main(void)
{
    vec3 light_dir = normalize(light.position - frag_pos);
    vec3 view_dir = normalize(view_pos - frag_pos);
    vec3 refl_dir = reflect(-light_dir, normal);

    float norm_d_light = max(dot(normal, light_dir), 0.0f);
    float view_d_refl = max(dot(view_dir, refl_dir), 0.0f);

    vec3 ambient = light.ambient * material.ambient; 
    vec3 diffuse = light.diffuse * (norm_d_light * material.diffuse);
    vec3 specular = light.specular * (pow(view_d_refl, material.sheen_coef) * material.specular);
    
    frag_color = vec4(ambient + diffuse + specular, 1.0f);
}
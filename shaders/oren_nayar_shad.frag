#version 330 core
in vec3 normal;
in vec3 frag_pos;
out vec4 frag_color;
uniform float roughness;
uniform vec3 view_pos;

uniform struct Light {
	vec3 diffuse;
    vec3 position;
} light;

void main(void)
{
    float roughness_squared = roughness * roughness;
    float a = 1.0f - 0.5f * (roughness_squared / (roughness_squared + 0.57f));
    float b = 0.45f * (roughness_squared / (roughness_squared + 0.09f));

    vec3 light_dir = normalize(light.position - frag_pos);      
    vec3 view_dir = normalize(view_pos - frag_pos);
    
    float angle_light_norm = acos(clamp(dot(normal, light_dir), 0.0f, 1.0f));
    float angle_view_norm = acos(clamp(dot(normal, view_dir), 0.0f, 1.0f));
    float alpha = max(angle_light_norm, angle_view_norm);
    float beta = min(angle_light_norm, angle_view_norm);
    float c = max(0.0f, cos(angle_light_norm - angle_view_norm)) * sin(alpha) * tan(beta);
    
    frag_color = vec4(max(0.0f, dot(normal, light_dir)) * (a + b * c) * light.diffuse, 1.0f);
}
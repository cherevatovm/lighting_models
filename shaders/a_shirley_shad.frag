#version 330 core
in vec3 normal;
in vec3 tangent;
in vec3 bitangent;
in vec3 frag_pos;
out vec4 frag_color;
uniform float refr_index;
uniform vec3 view_pos;
uniform vec2 ashikhmin_anisotropy;

uniform struct Light {
	vec3 diffuse;
	vec3 specular;
    vec3 position;
} light;

void main(void)
{
    const float PI = 3.1415926;

    vec3 light_dir = normalize(light.position - frag_pos);      
    vec3 view_dir = normalize(view_pos - frag_pos);
    vec3 half_dir = normalize(light_dir + view_dir);

    float norm_d_view = max(dot(normal, view_dir), 0.0f);
    float norm_d_light = max(dot(normal, light_dir), 0.0f);
    float norm_d_half = max(dot(normal, half_dir), 0.0f);
    float half_d_light = max(dot(half_dir, light_dir), 0.0f);
    float tang_d_half = dot(tangent, half_dir);
    float bitang_d_half = dot(bitangent, half_dir);  

    float r_diffuse = (28.0f / (23.0f * PI)) * (1.0f - pow(1.0f - 0.5f * norm_d_light, 5.0f)) * (1.0f - pow(1.0f - 0.5f * norm_d_view, 5.0f));

    float a = sqrt((ashikhmin_anisotropy.x + 1) * (ashikhmin_anisotropy.y + 1)) / (8 * PI * half_d_light * max(norm_d_light, norm_d_view));
    float exp_of_b = ashikhmin_anisotropy.x * tang_d_half * tang_d_half + ashikhmin_anisotropy.y * bitang_d_half * bitang_d_half;
    exp_of_b /= (1.0f - norm_d_half * norm_d_half);
    float b = pow(norm_d_half, exp_of_b);
    float f0 = pow((refr_index - 1.0f) / (refr_index + 1.0f), 2.0f);
    float f = f0 + (1.0f - f0) * pow(1.0f - half_d_light, 5.0f);
    float r_specular = a * b * f;

    frag_color = vec4(light.diffuse * (1.0f - f0) * r_diffuse + light.specular * r_specular, 1.0f);
}
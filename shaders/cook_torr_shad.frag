#version 330 core
in vec3 normal;
in vec3 frag_pos;
out vec4 frag_color;
uniform float roughness;
uniform float metal_prop;
uniform float refr_index;
uniform vec3 view_pos;
uniform vec3 material_diffuse;

uniform struct Light {
	vec3 color;
    vec3 position;
} light;

void main(void)
{
    const float e = 2.7182818;
    const float PI = 3.1415926;

    vec3 light_dir = normalize(light.position - frag_pos);      
    vec3 view_dir = normalize(view_pos - frag_pos);
    vec3 half_dir = normalize(light_dir + view_dir);

    float norm_d_view = max(dot(normal, view_dir), 0.0f);
    float norm_d_light = max(dot(normal, light_dir), 0.0f);
    float norm_d_half = max(dot(normal, half_dir), 0.0f);
    float half_d_view = max(dot(half_dir, view_dir), 0.0f);

    float roughness_squared = roughness * roughness;
    if (roughness_squared == 0)
        roughness_squared += 0.001f;
    float n_d_h_squared = norm_d_half * norm_d_half;
    float d = pow(e, (n_d_h_squared - 1) / (roughness_squared * n_d_h_squared));
    d /= (4.0f * roughness_squared * n_d_h_squared * n_d_h_squared);

    float g = min(1.0f, min((2.0f * norm_d_half * norm_d_view) / half_d_view, (2.0f * norm_d_half * norm_d_light) / half_d_view));
    
    float f0 = pow((refr_index - 1.0f) / (refr_index + 1.0f), 2.0f);
    float f = f0 + (1.0f - f0) * pow(1.0f - half_d_view, 5.0f);
    
    float r_cook_torr = (f * g * d) / (4.0f * norm_d_light * norm_d_view + 0.0001f); 

    float diff_coef = 1.0f - f;
    diff_coef *= 1.0f - metal_prop;

    frag_color = vec4((diff_coef * material_diffuse + r_cook_torr) * norm_d_light * light.color, 1.0f);
}
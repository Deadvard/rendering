#version 430

in vertex_data
{
	vec3 world_pos;
	vec2 tex_coord;
	vec3 world_normal;
};

out vec4 frag_color;

uniform sampler2D albedo_map;
uniform sampler2D normal_map;
uniform sampler2D metallic_map;
uniform sampler2D roughness_map;
uniform sampler2D ambient_occlusion_map;

uniform bool is_lambert;

const int num_point_lights = 16;

uniform vec3 point_light_positions[num_point_lights];
uniform vec3 point_light_colors[num_point_lights];

uniform vec3 cam_pos;

const float PI = 3.14159265;

vec3 generate_tangent_normal()
{
    vec3 tangent_normal = texture(normal_map, tex_coord).xyz * 2.0 - 1.0;

    vec3 Q1  = dFdx(world_pos);
    vec3 Q2  = dFdy(world_pos);
    vec2 st1 = dFdx(tex_coord);
    vec2 st2 = dFdy(tex_coord);

    vec3 N   = normalize(world_normal);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangent_normal);
}

float distribution_ggx(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}

float geometry_schlick_ggx(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}

float geometry_smith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = geometry_schlick_ggx(NdotV, roughness);
    float ggx1 = geometry_schlick_ggx(NdotL, roughness);

    return ggx1 * ggx2;
}

vec3 fresnel_schlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

vec3 Diffuse_Burley( vec3 DiffuseColor, float Roughness, float NoV, float NoL, float VoH )
{
    float FD90 = 0.5 + 2 * VoH * VoH * Roughness;
    float FdV = 1 + (FD90 - 1) * pow( 1 - NoV, 5 );
    float FdL = 1 + (FD90 - 1) * pow( 1 - NoL, 5 );
    return DiffuseColor * ( (1 / PI) * FdV * FdL );
}

void main()
{
	vec3 albedo     = pow(texture(albedo_map, tex_coord).rgb, vec3(2.2));
    float metallic  = texture(metallic_map, tex_coord).r;
    float roughness = texture(roughness_map, tex_coord).r;
    float ao        = texture(ambient_occlusion_map, tex_coord).r;

    vec3 N = generate_tangent_normal();
    vec3 V = normalize(cam_pos - world_pos);

    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);

    vec3 Lo = vec3(0.0);
    for(int i = 0; i < num_point_lights; ++i) 
    {
        // calculate per-light radiance
        vec3 L = normalize(point_light_positions[i] - world_pos);
        vec3 H = normalize(V + L);
        float distance = length(point_light_positions[i] - world_pos);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance = point_light_colors[i] * attenuation;

        // Cook-Torrance BRDF
        float NDF = distribution_ggx(N, H, roughness);   
        float G   = geometry_smith(N, V, L, roughness);      
        vec3 F    = fresnel_schlick(max(dot(H, V), 0.0), F0);
           
        vec3 nominator    = NDF * G * F; 
        float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001; // 0.001 to prevent divide by zero.
        vec3 specular = nominator / denominator;
        
        // kS is equal to Fresnel
        vec3 kS = F;
        // for energy conservation, the diffuse and specular light can't
        // be above 1.0 (unless the surface emits light); to preserve this
        // relationship the diffuse component (kD) should equal 1.0 - kS.

		vec3 kD;

		if(is_lambert)
		{
			kD = vec3(1,1,1) - kS;

		}
		else // burley
		{
			float NoV = max(dot(N, V), 0.0);
			float NoL = max(dot(N, L), 0.0);
		    float NoH = max(dot(N, H), 0.0);

            kD = Diffuse_Burley(albedo, roughness, NoV, NoL, NoH) - kS;
		}

        // multiply kD by the inverse metalness such that only non-metals 
        // have diffuse lighting, or a linear blend if partly metal (pure metals
        // have no diffuse light).
        kD *= 1.0 - metallic;

        // scale light by NdotL
        float NdotL = max(dot(N, L), 0.0);

        // add to outgoing radiance Lo
        Lo += (kD * albedo / PI + specular) * radiance * NdotL;  // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
    }   
    
    // ambient lighting (note that the next IBL tutorial will replace 
    // this ambient lighting with environment lighting).
    vec3 ambient = vec3(0.03) * albedo * ao;
    
    vec3 color = ambient + Lo;

    // HDR tonemapping
    color = color / (color + vec3(1.0));
    // gamma correct
    color = pow(color, vec3(1.0/2.2));

    frag_color = vec4(color, 1.0);
}
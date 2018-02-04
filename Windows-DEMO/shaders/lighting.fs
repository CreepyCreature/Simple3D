#version 330
#define M_PI 3.1415926535897932384626433832795

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;
in vec3 Tangent;
//in vec3 Color;

out vec4 color;

uniform bool normal_only = false;
uniform bool phong_spec = true;

struct Material 
{
	sampler2D diffuse;
	sampler2D specular;

	vec3 color_ambient;
	vec3 color_diffuse;
	vec3 color_specular;

	float shininess;
	float opacity;	
};
uniform Material material;

const int kMaxDirLights = 16;
struct DirLight 
{
	vec3 direction;

	// Light colors
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	bool is_active;
};
uniform DirLight u_dir_lights[kMaxDirLights];
uniform int u_nrdirlights = 0;

const int kMaxPointLights = 16;
struct PointLight 
{
	vec3 position;

	// Light colors
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	// Attenuation values
	float constant;
	float linear;
	float quadratic;

	bool is_active;
};
uniform PointLight u_point_lights[kMaxPointLights];
uniform int u_nrpointlights = 0;

const int kMaxSpotLights = 16;
struct SpotLight 
{
	vec3 position;
	vec3 direction;

	float inner_cutoff;
	float outer_cutoff;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;

	bool is_active;
};
uniform SpotLight u_spot_lights[kMaxSpotLights];
uniform int u_nrspotlights = 0;

uniform vec3 u_viewpos;

vec3 CalculateDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalculateSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

vec3 ComputeCookTorranceReflection(vec3 lightDir, vec3 viewDir, vec3 normal, vec3 light_specular);
vec3 ComputeFresnelFactor(float LdotH, vec3 specularColor);
float ComputeMicrofacetDistFunc(float NdotH);
float ComputeGeometricalAttenuation(vec3 lightDir, vec3 viewDir, vec3 normal, float NdotH, float LdotH);

float ComputeAttenuation(float distance, float constant, float linear, float quadratic);
float ShininessToRoughness(float shininess);

bool DirtyEquals(float lhs, float rhs);

void main()	
{	
	vec3 norm 		= normalize(Normal);
	vec3 viewDir 	= normalize(u_viewpos - FragPos);

	// Resulting color, starts off as (0, 0, 0)
	vec3 result = vec3(0.0f);

	// Process each is_active directional light
	for (int i = 0; i < u_nrdirlights; ++i)
	{		
		if (u_dir_lights[i].is_active) 
		{
			result += CalculateDirLight(u_dir_lights[i], norm, viewDir);
		}
		
	}

	// Process each is_is_active point light
	for (int i = 0; i < u_nrpointlights; ++i) 
	{
		if (u_point_lights[i].is_active) 
		{
			result += CalculatePointLight(u_point_lights[i], norm, FragPos, viewDir);
		}
	}

	// Process each is_active spot light
	for (int i = 0; i < u_nrspotlights; ++i) 
	{
		if (u_spot_lights[i].is_active)
		{
			result += CalculateSpotLight(u_spot_lights[i], norm, FragPos, viewDir);
		}
	}

	if (!normal_only) {
		color = vec4(result, material.opacity); // <-- Default
	}
	else 
		color = vec4(norm, 1.0f);
}

vec3 CalculateDirLight(DirLight light, vec3 normal, vec3 viewDir) 
{
	vec3 lightDir = normalize(-light.direction);

	// Ambient
	vec3 ambientOut = light.ambient * vec3(texture(material.diffuse, TexCoords));

	// Diffuse
	float diff = max(dot(normal, lightDir), 0.0f);
	vec3 diffuseOut = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

	// Specular
	vec3 specularOut = vec3(0.0f);
	if (phong_spec)
	{
		float spec = 0.0f;
		if (dot(lightDir, normal) > 0.0f) 
		{
			vec3 reflectDir = reflect(-lightDir, normal);
			spec = pow(max(dot(reflectDir, viewDir), 0.0f), material.shininess);	
		}
		specularOut = light.specular * spec * max(vec3(texture(material.specular, TexCoords)), 0.0f);
	}
	else
	{
		// Cook Torrance BRDF with Beckmann Distribution and Geometrical Attenuation		
		specularOut = ComputeCookTorranceReflection(lightDir, viewDir, normal, light.specular);
	}

	// Material colors (assuming the model implements them correctly)
	ambientOut 	*= material.color_ambient	;
	diffuseOut 	*= material.color_diffuse	;
	specularOut	*= material.color_specular	;

	//return specularOut;
	return (ambientOut + diffuseOut + specularOut);
}	

vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) 
{
	vec3 lightDir = normalize(light.position - fragPos);

	// Ambient
	vec3 ambientOut = light.ambient * vec3(texture(material.diffuse, TexCoords));

	// Diffuse
	float diff = max(dot(lightDir, normal), 0.0f);
	vec3 diffuseOut = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

	vec3 specularOut = vec3(0.0f);
	// Specular	
	if (phong_spec)
	{
		// *** Phong ***
		float spec = 0.0f;
		if (dot(lightDir, normal) > 0.0f) 
		{
			vec3 reflectDir = reflect(-lightDir, normal);
			spec = pow(max(dot(reflectDir, viewDir), 0.0f), material.shininess);		
		}
		specularOut = light.specular * spec * vec3(texture(material.specular, TexCoords));
	}
	else 
	{
		// Cook Torrance BRDF with Beckmann Distribution and Geometrical Attenuation		
		specularOut = ComputeCookTorranceReflection(lightDir, viewDir, normal, light.specular);
	}

	// Attenuation
	float distance = length(light.position - fragPos);
	float attenuation = ComputeAttenuation(distance, light.constant, light.linear, light.quadratic);
	ambientOut  *= attenuation;
	diffuseOut  *= attenuation;
	specularOut *= attenuation;

	// Material colors (assuming the model implements them correctly)
	ambientOut 	*= material.color_ambient	;
	diffuseOut 	*= material.color_diffuse	;
	specularOut	*= material.color_specular	;

	return (ambientOut + diffuseOut + specularOut);
}

vec3 CalculateSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir) 
{
	vec3 lightDir = normalize(light.position - fragPos);

	// Ambient
	vec3 ambientOut = light.ambient * vec3(texture(material.diffuse, TexCoords));

	// Diffuse
	float diff = max(dot(lightDir, normal), 0.0f);
	vec3 diffuseOut = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

	// Specular
	vec3 specularOut = vec3(0.0f);
	if (phong_spec)
	{
		float spec = 0.0f;
		if (dot(normal, lightDir) > 0.0f) 
		{
			vec3 reflectDir = reflect(-lightDir, normal);
			spec = pow(max(dot(reflectDir, viewDir), 1.0f), material.shininess);
		}
		vec3 specularOut = light.specular * spec *  vec3(texture(material.specular, TexCoords));
	}
	else
	{
		// Cook Torrance BRDF with Beckmann Distribution (for microfacets)
		//specularOut = ComputeCookTorranceReflection(lightDir, viewDir, normal, light.specular); <-- Default
		specularOut = ComputeCookTorranceReflection(lightDir, viewDir, normal, light.specular);

		// DEBUG
		//return specularOut;
	}

	// Attenuation
	float distance = length(light.position - fragPos);
	float attenuation = ComputeAttenuation(distance, light.constant, light.linear, light.quadratic);
	ambientOut *= attenuation;
	diffuseOut *= attenuation;
	specularOut *= attenuation;

	// Intensity
	float theta = max(dot(-lightDir, normalize(light.direction)), 0.0f);
	float epsilon = light.inner_cutoff - light.outer_cutoff;
	float intensity = clamp((theta - light.outer_cutoff) / epsilon, 0.0f, 1.0f);
	// if (intensity > 0.999999f) intensity = 1000.f; // Use this to clearly see the inner cutoff
	// if (intensity < 0.999999f && intensity > 0.000001) intensity = 1000.f; // Use this to clearly see the outer cutoff
	diffuseOut *= intensity;
	specularOut *= intensity;

	// Material colors (assuming the model implements them correctly)
	ambientOut 	*= material.color_ambient	;
	diffuseOut 	*= material.color_diffuse	;
	specularOut	*= material.color_specular	;

	return (ambientOut + diffuseOut + specularOut);
}

vec3 ComputeCookTorranceReflection(vec3 lightDir, vec3 viewDir, vec3 normal, vec3 light_specular)
{
	vec3 specular_reflection = vec3(0.0f);

	if (dot(lightDir, normal) > 0.0f)
	{
		vec3 H = normalize(lightDir + viewDir);
		// float LdotH = max(dot(lightDir, H), 0);
		// float NdotH = max(dot(normal, H), 0);
		// //float NdotV = dot(normal, viewDir);
		// //float NdotL = dot(normal, lightDir);
		// float NdotV = max(dot(normal, viewDir), 0);
		// float NdotL = max(dot(normal, lightDir), 0);

		// float LdotH = max(dot(lightDir, H), 0);
		// float NdotH = max(dot(normal, H), 0);
		// float NdotV = (dot(normal, viewDir));
		// float NdotL = (dot(normal, lightDir));

		float LdotH = (dot(lightDir, H));
		float NdotH = (dot(normal, H));
		float NdotV = (dot(normal, viewDir));
		float NdotL = (dot(normal, lightDir));

		vec3 spec_color = vec3(texture(material.specular, TexCoords)) * light_specular * material.color_specular;

		vec3 F = ComputeFresnelFactor(LdotH, spec_color);
		float D = ComputeMicrofacetDistFunc(NdotH);
		float G = ComputeGeometricalAttenuation(lightDir, viewDir, normal, NdotH, LdotH);

		//specular_reflection = spec_color * G * F * vec3(D / (M_PI * NdotV * NdotL));
		specular_reflection = spec_color * G * F * vec3(D / (4 * NdotV * NdotL));
		//specular_reflection = spec_color * G * vec3(D / (M_PI * NdotV * NdotL));
	}

	return specular_reflection;
}

vec3 ComputeFresnelFactor(float LdotH, vec3 specularColor)
{
	if (DirtyEquals(LdotH, 1.0f)) { return specularColor; }

	// Approximate the refractance factor
	vec3 eta;
	eta.r = (1 + sqrt(specularColor.r)) / (1 - sqrt(specularColor.r));
	eta.g = (1 + sqrt(specularColor.g)) / (1 - sqrt(specularColor.g));
	eta.b = (1 + sqrt(specularColor.b)) / (1 - sqrt(specularColor.b));

	vec3 g;
	g.r = sqrt(pow(eta.r, 2) - 1 + pow(LdotH, 2));
	g.g = sqrt(pow(eta.g, 2) - 1 + pow(LdotH, 2));
	g.b = sqrt(pow(eta.b, 2) - 1 + pow(LdotH, 2));

	// Calculate the Fresnel factor
	vec3 F;
	F.r = (0.5f) * (pow(g.r - LdotH, 2) / pow(g.r + LdotH, 2))
			* ((pow((LdotH * (g.r + LdotH)) - 1, 2) / pow((LdotH * (g.r - LdotH)) + 1, 2)) + 1);
	F.g = (0.5f) * (pow(g.g - LdotH, 2) / pow(g.g + LdotH, 2))
			* ((pow((LdotH * (g.g + LdotH)) - 1, 2) / pow((LdotH * (g.g - LdotH)) + 1, 2)) + 1);
	F.b = (0.5f) * (pow(g.b - LdotH, 2) / pow(g.b + LdotH, 2))
			* ((pow((LdotH * (g.b + LdotH)) - 1, 2) / pow((LdotH * (g.b - LdotH)) + 1, 2)) + 1);

	//return vec3(1.0f);
	return F;
}

float ComputeMicrofacetDistFunc(float NdotH)
{
	// Using an isotropic material, same roughness in every angle
	float m = ShininessToRoughness(material.shininess);
	float m_2 = m * m;
	float NoH_2 = NdotH * NdotH;
	float NoH_4 = NoH_2 * NoH_2;
	// Beckmann Distribution
	float D = (1 / (4 * m_2 * NoH_4)) * exp( (NoH_2 - 1) / (m_2 * NoH_2) );

	return D;
}

float ShininessToRoughness(float shininess)
{
	return sqrt(2 / (shininess + 2));
}

float ComputeGeometricalAttenuation(vec3 lightDir, vec3 viewDir, vec3 normal, float NdotH, float LdotH)
{
	float NdotV = dot(normal, viewDir);
	float NdotL = dot(normal, lightDir);
	float G1 = 2 * NdotH * NdotV / LdotH;
	float G2 = 2 * NdotH * NdotL / LdotH;
	float G  = min(G1, G2);
	G = min(1, G);

	//return 1.0f;
	return G;
}

float ComputeAttenuation(float distance, float constant, float linear, float quadratic)
{
	return 1.0f / (constant
		+ linear * distance
		+ quadratic * (distance * distance)
		);
}

// Maybe make a branchless version
bool DirtyEquals(float lhs, float rhs)
{
	return (abs(lhs - rhs) < 0.001f);
}
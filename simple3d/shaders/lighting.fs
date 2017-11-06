#version 330

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;
//in vec3 Color;

out vec4 color;

uniform bool normal_only = false;

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

	// Overriding result color
	//result += vec3(texture(material.diffuse, TexCoords)) * 0.00001 + shininess * 0.00001;
	//result += vec3(texture(material.specular, TexCoords));
	//color = vec4(result, 1.0f);
	if (!normal_only)
		color = vec4(result, material.opacity); // <-- Default
	else 
		color = vec4(norm, 1.0f);
	//color = texture(material.specular, TexCoords);

	//color = texture(material.specular, TexCoords);
	//color = vec4(material.color_specular, 1.0f);

	//if (material.shininess < 0.0f) color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
	//color = vec4(vec3(material.shininess), 1.0f);

	//color = texture(material.diffuse, TexCoords);
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
	float spec = 0.0f;
	if (dot(lightDir, normal) > 0.0f) 
	{
		vec3 reflectDir = reflect(-lightDir, normal);
		spec = pow(max(dot(reflectDir, viewDir), 0.0f), material.shininess);	
	}
	vec3 specularOut = light.specular * spec * max(vec3(texture(material.specular, TexCoords)), 0.0f);

	// Material colors (assuming the model implements them correctly)
	ambientOut 	*= material.color_ambient	;
	diffuseOut 	*= material.color_diffuse	;
	specularOut	*= material.color_specular	;

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

	// Specular
	float spec = 0.0f;
	if (dot(lightDir, normal) > 0.0f) 
	{ // (diff > 0.0f) -- small optimisation perhaps
		vec3 reflectDir = reflect(-lightDir, normal);
		spec = pow(max(dot(reflectDir, viewDir), 0.0f), material.shininess);		
	}
	vec3 specularOut = light.specular * spec * vec3(texture(material.specular, TexCoords));

	// Attenuation
	float distance = length(light.position - fragPos);
	float attenuation = 1.0f / (light.constant
		+ light.linear * distance
		+ light.quadratic * (distance * distance)
		);
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
	float spec = 0.0f;
	if (dot(normal, lightDir) > 0.0f) 
	{
		vec3 reflectDir = reflect(-lightDir, normal);
		spec = pow(max(dot(reflectDir, viewDir), 1.0f), material.shininess);
	}
	vec3 specularOut = light.specular * spec *  vec3(texture(material.specular, TexCoords));

	// Attenuation
	float distance = length(light.position - fragPos);
	float attenuation = 1.0f / (light.constant
		+ light.linear * distance
		+ light.quadratic * (distance * distance)
		);
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
#version 330 core

in vec3 obj_vertex;
in vec3 obj_normal;
in vec2 obj_texcoord;

out vec2 UV;
out float lightIntensity;
out float specular_factor;

uniform mat4 model_mat;
uniform mat4 VP;
uniform vec3 light_pos;
uniform vec3 eye_pos;

void main ()
{
	vec4 model_space;
	vec3 light_vector, reflect_vector, view_vector;
	
	model_space = model_mat * vec4(obj_vertex, 1.0);
	
	light_vector = normalize(light_pos - model_space.xyz);
	view_vector = normalize(eye_pos - model_space.xyz);
		
	lightIntensity = dot(light_vector,obj_normal);
	
	reflect_vector = normalize(2*lightIntensity*obj_normal - light_vector);
	specular_factor = dot(reflect_vector,view_vector);
	
	lightIntensity = (lightIntensity>0.0)?lightIntensity:0.0;
	specular_factor = (specular_factor>0.0)?specular_factor:0.0;
	specular_factor = pow(specular_factor,10);
		
	lightIntensity = lightIntensity + 0.15f;
	
	gl_Position =  VP * model_space;
	UV = obj_texcoord;
}
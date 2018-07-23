out vec3 AMG_ReflectedVector;
out vec3 AMG_RefractedVector;

uniform vec3 AMG_CamPosition;

void AMG_PassTexcoordsReflection(){
	vec3 pos = (AMG_M * vec4(AMG_Position, 1)).xyz;
	vec3 normal = mat3(transpose(inverse(AMG_M))) * AMG_Normal;
	vec3 viewVector = normalize(pos - AMG_CamPosition);
	AMG_ReflectedVector = reflect(viewVector, normal);
}

void AMG_PassTexcoordsRefraction(){
	vec3 pos = (AMG_M * vec4(AMG_Position, 1)).xyz;
	vec3 normal = mat3(transpose(inverse(AMG_M))) * AMG_Normal;
	vec3 viewVector = normalize(pos - AMG_CamPosition);
    AMG_RefractedVector = refract(viewVector, normal, 1.0/1.33);
}

void AMG_PassTexcoordsReflectionRefraction(){
	vec3 pos = (AMG_M * vec4(AMG_Position, 1)).xyz;
	vec3 normal = mat3(transpose(inverse(AMG_M))) * AMG_Normal;
	vec3 viewVector = normalize(pos - AMG_CamPosition);
	AMG_ReflectedVector = reflect(viewVector, normal);
    AMG_RefractedVector = refract(viewVector, normal, 1.0/1.33);
}

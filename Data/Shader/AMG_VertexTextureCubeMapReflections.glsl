out vec3 AMG_ReflectedVector;
out vec3 AMG_RefractedVector;

void AMG_PassTexcoordsReflection(){
	vec3 viewVector = normalize((AMG_MV * vec4(AMG_Position, 1)).xyz);
	vec3 normal = normalize((AMG_MV * vec4(AMG_Normal, 0)).xyz);
	AMG_ReflectedVector = reflect(viewVector, normal);
}

void AMG_PassTexcoordsRefraction(){
	vec3 viewVector = normalize((AMG_MV * vec4(AMG_Position, 1)).xyz);
	vec3 normal = normalize((AMG_MV * vec4(AMG_Normal, 0)).xyz);
    AMG_RefractedVector = refract(viewVector, normal, 1.0/1.33);
}

void AMG_PassTexcoordsReflectionRefraction(){
	vec3 viewVector = normalize((AMG_MV * vec4(AMG_Position, 1)).xyz);
	vec3 normal = normalize((AMG_MV * vec4(AMG_Normal, 0)).xyz);
	AMG_ReflectedVector = reflect(viewVector, normal);
    AMG_RefractedVector = refract(viewVector, normal, 1.0/1.33);
}

/**
 * @brief Compute a deferred scene (using a normal map)
 * @param normalMap Normal map texture unit
 * @param albedo Diffuse texture value for this fragment
 * @param specular Specular value
 * Output: AMG_GPosition, AMG_GNormal, AMG_GAlbedo
 */
void AMG_ComputeDeferredNormalMap(int normalMap, vec3 albedo, float specular){
	vec4 normalColor = 2 * texture(AMG_TextureSampler[normalMap], AMG_OutUV) - 1;

	AMG_GPosition = AMG_OutGPosition;
	AMG_GNormal.xyz = normalize(AMG_OutGNormalMatrix * normalColor.rgb);
	AMG_GAlbedo.rgb = albedo;
	AMG_GAlbedo.a = specular;
}

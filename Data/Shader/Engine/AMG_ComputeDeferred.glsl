/**
 * @brief Compute a deferred scene
 * @param albedo Diffuse texture value for this fragment
 * @param specular Specular value
 * Output: AMG_GPosition, AMG_GNormal, AMG_GAlbedo
 */
void AMG_ComputeDeferred(vec3 albedo, float specular){
	AMG_GPosition = AMG_OutGPosition;
	AMG_GNormal.xyz = normalize(AMG_OutGNormal);
	AMG_GAlbedo.rgb = albedo;
	AMG_GAlbedo.a = specular;
}

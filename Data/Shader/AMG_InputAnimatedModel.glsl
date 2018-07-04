layout(location = 0) in vec3 AMG_Position;			/**< Vertex position, in model space */
layout(location = 1) in vec2 AMG_UV;				/**< Vertex texture coordinates */
layout(location = 2) in vec3 AMG_Normal;			/**< Vertex outside normal vector */
layout(location = 3) in vec4 AMG_Weight;			/**< Vertex weight for 4 bones */
layout(location = 4) in ivec4 AMG_WeightBoneID;		/**< Bones used for this vertex weights */

float3 calcWorldNormals(float3 normalT, float3 tangentW, float3 normalW)
{
	// The normal map range must also be rescaled: from [0,1](done by the sampler) to [-1,1]
	normalT = 2.0f*normalT - 1.0f;

	// Create the orthonormal basis(TBN) for this pixel to transform the
	// the normal in the normal map from tangent space to world space
	float3 N = normalize(normalW); 
	float3 T = normalize(tangentW - dot(tangentW, N)*N); // Makes sure that T is orthonormal to N, subract all components from T that is shifting it in the direction of N.
	float3 B = cross(N,T);				       	     	 // Vector orthogonal to N and T.

	// Create the matrix
	float3x3 TBN = float3x3(T, B, N);

	// Now we can transform the normal from Tangent space to World space
	return normalize(mul(normalT, TBN));
}
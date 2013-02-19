//Output from the patch constant function 
struct patchConstant
{
	float edges[3] 	: SV_TessFactor;
	float inside 	: SV_InsideTessFactor;
};
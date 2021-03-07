#include "iostream"


void func(int* pixels, float bright, float contrast)
{

	__asm__ __volatile__ (
	 	"vbroadcastss (%0), %%xmm2\n\t"
        	"vbroadcastss (%1), %%xmm1\n\t"
        	"vmovdqa (%2), %%xmm0\n\t"
        	"vcvtdq2ps %%xmm0, %%xmm0\n\t"
		// "vfmadd132ps %%xmm1, %%xmm2, %%xmm0\n\t"
        	"vfmadd231ps %%xmm1, %%xmm2, %%xmm0\n\t" //zmm1 * zmm2 + zmm0 = zmm0 
		"vcvtps2dq %%xmm0, %%xmm0\n\t"
	    	"vmovdqa %%xmm0, (%2)\n\t"
		::
		"S"(&bright), "D"(&contrast), "b"(pixels)
		:
	    	"%xmm0", "%xmm1", "%xmm2"	
	);	
}

int main(int argc, char** argv)
{
	int a[] = {2,3,4,6};
	float b = 5.0f;
	float c = 10.0f;

	func(a, b, c);
	
	// for(int i = 0; i < 4; ++i)
	// {
	// 	std::cout << "a[i]*c + b = " << a[i] << std::endl;
	//}

	std::cout << "b*c = " << a[0] << std::endl;
	return 0;
}

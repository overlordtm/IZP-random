/************TASK1*************/
//Create the OpenCL kernel here

// OpenCL Kernel
__kernel void symm(__global float* C, __global float* A, __global float* B, int wA, int wB) { 
   
	int i = get_global_id(0); //up to M
	int j = get_global_id(1);  // up to N
   	
	float tmp = 0;

   	for (int k = 0; k < wA; k++) {
      		tmp += A[i * wA + k] * B[k * wB + j];
   	}
 
   	C[i * wA + j] = tmp;

}

/***********ENDTASK************/

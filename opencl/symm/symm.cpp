#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <math.h>
#include <CL/cl.h>
#include <string.h>

#define A(matr,i,j) matr[(i)*M+(j)]
#define BC(matr,i,j) matr[(i)*N+(j)]

/* Problem size */
#define M 2048
#define N 2048

#define DEVICEID 0

/* Divide the problem into local work sizes of LOCALWSX x LOCALWSY */
#define LOCALWSX 8
#define LOCALWSY 8

double walltime (void); //Get system time

void host_symm (float *c, const float *a, const float *b)
{
	float dotproduct;
	for (int i = 0; i < M; i++)
	{
		for (int j = 0; j < N; j++)
		{
			dotproduct = 0.0;
			for (int k = 0; k < M; k++)
				dotproduct += A(a,i,k) * BC(b,k,j);
			BC(c,i,j) = dotproduct;
		}
	}
}

//Prints out the error string for a error code.
char *print_cl_errstring(cl_int err) {
    switch (err) {
        case CL_SUCCESS:                          return strdup("Success!");
        case CL_DEVICE_NOT_FOUND:                 return strdup("Device not found.");
        case CL_DEVICE_NOT_AVAILABLE:             return strdup("Device not available");
        case CL_COMPILER_NOT_AVAILABLE:           return strdup("Compiler not available");
        case CL_MEM_OBJECT_ALLOCATION_FAILURE:    return strdup("Memory object allocation failure");
        case CL_OUT_OF_RESOURCES:                 return strdup("Out of resources");
        case CL_OUT_OF_HOST_MEMORY:               return strdup("Out of host memory");
        case CL_PROFILING_INFO_NOT_AVAILABLE:     return strdup("Profiling information not available");
        case CL_MEM_COPY_OVERLAP:                 return strdup("Memory copy overlap");
        case CL_IMAGE_FORMAT_MISMATCH:            return strdup("Image format mismatch");
        case CL_IMAGE_FORMAT_NOT_SUPPORTED:       return strdup("Image format not supported");
        case CL_BUILD_PROGRAM_FAILURE:            return strdup("Program build failure");
        case CL_MAP_FAILURE:                      return strdup("Map failure");
        case CL_INVALID_VALUE:                    return strdup("Invalid value");
        case CL_INVALID_DEVICE_TYPE:              return strdup("Invalid device type");
        case CL_INVALID_PLATFORM:                 return strdup("Invalid platform");
        case CL_INVALID_DEVICE:                   return strdup("Invalid device");
        case CL_INVALID_CONTEXT:                  return strdup("Invalid context");
        case CL_INVALID_QUEUE_PROPERTIES:         return strdup("Invalid queue properties");
        case CL_INVALID_COMMAND_QUEUE:            return strdup("Invalid command queue");
        case CL_INVALID_HOST_PTR:                 return strdup("Invalid host pointer");
        case CL_INVALID_MEM_OBJECT:               return strdup("Invalid memory object");
        case CL_INVALID_IMAGE_FORMAT_DESCRIPTOR:  return strdup("Invalid image format descriptor");
        case CL_INVALID_IMAGE_SIZE:               return strdup("Invalid image size");
        case CL_INVALID_SAMPLER:                  return strdup("Invalid sampler");
        case CL_INVALID_BINARY:                   return strdup("Invalid binary");
        case CL_INVALID_BUILD_OPTIONS:            return strdup("Invalid build options");
        case CL_INVALID_PROGRAM:                  return strdup("Invalid program");
        case CL_INVALID_PROGRAM_EXECUTABLE:       return strdup("Invalid program executable");
        case CL_INVALID_KERNEL_NAME:              return strdup("Invalid kernel name");
        case CL_INVALID_KERNEL_DEFINITION:        return strdup("Invalid kernel definition");
        case CL_INVALID_KERNEL:                   return strdup("Invalid kernel");
        case CL_INVALID_ARG_INDEX:                return strdup("Invalid argument index");
        case CL_INVALID_ARG_VALUE:                return strdup("Invalid argument value");
        case CL_INVALID_ARG_SIZE:                 return strdup("Invalid argument size");
        case CL_INVALID_KERNEL_ARGS:              return strdup("Invalid kernel arguments");
        case CL_INVALID_WORK_DIMENSION:           return strdup("Invalid work dimension");
        case CL_INVALID_WORK_GROUP_SIZE:          return strdup("Invalid work group size");
        case CL_INVALID_WORK_ITEM_SIZE:           return strdup("Invalid work item size");
        case CL_INVALID_GLOBAL_OFFSET:            return strdup("Invalid global offset");
        case CL_INVALID_EVENT_WAIT_LIST:          return strdup("Invalid event wait list");
        case CL_INVALID_EVENT:                    return strdup("Invalid event");
        case CL_INVALID_OPERATION:                return strdup("Invalid operation");
        case CL_INVALID_GL_OBJECT:                return strdup("Invalid OpenGL object");
        case CL_INVALID_BUFFER_SIZE:              return strdup("Invalid buffer size");
        case CL_INVALID_MIP_LEVEL:                return strdup("Invalid mip-map level");
        default:                                  return strdup("Unknown");
    }
}

char * load_program_source(const char *filename)
{ 
	
	struct stat statbuf;
	FILE *fh; 
	char *source; 
	
	fh = fopen(filename, "r");
	if (fh == 0)
		return 0; 
	
	stat(filename, &statbuf);
	source = (char *) malloc(statbuf.st_size + 1);
	fread(source, statbuf.st_size, 1, fh);
	source[statbuf.st_size] = '\0'; 
	
	return source; 
}

int
main()
{

	enum { HOST=0, DEVICE=1 };
	enum { SETUP=0, COMPUTE=1, REPORT=2 };
    
   
    // Platform info
    cl_int err;
    cl_uint numPlatforms = 0;
	cl_platform_id *platforms = NULL;
    cl_device_id *devices = NULL;
    cl_uint numDevices = 0;
    int dM = M;
    int dN = N;

    printf("Calling clGetPlatformIDs\n");
	err = clGetPlatformIDs(0, NULL, &numPlatforms);
	if (err != CL_SUCCESS) {
        printf( "clGetPlatformIDs failed\n" );
        return 1;
    }

    printf("Calling clGetPlatformIDs\n");
    platforms = (cl_platform_id*)malloc(numPlatforms*sizeof(cl_platform_id));
    err = clGetPlatformIDs(numPlatforms, platforms, NULL);
    if (err != CL_SUCCESS) {
        printf( "clGetPlatformIDs failed\n" );
        return 1;
    }

	printf("Calling clGetDeviceIDs\n");
	err = clGetDeviceIDs(platforms[0], CL_DEVICE_TYPE_ALL, 0, NULL, &numDevices);
	if (err != CL_SUCCESS) {
        printf( "clGetDeviceIDs failed\n" );
        return 1;
    }

    printf("Calling clGetDeviceIDs\n");
    devices = (cl_device_id*)malloc(numDevices * sizeof(cl_device_id));
    err = clGetDeviceIDs(platforms[0], CL_DEVICE_TYPE_ALL, numDevices, devices, NULL);
    if (err != CL_SUCCESS) {
        printf( "clGetDeviceIDs failed\n" );
        return 1;
    }
    
    printf("Calling clCreateContext\n");
	cl_context ctx = clCreateContext(NULL, 1, &devices[DEVICEID], NULL, NULL, &err);
	if (err != CL_SUCCESS) {
        printf( "clCreateContext failed\n" );
        return 1;
    }
    
    printf("Calling clCreateCommandQueue\n");
	cl_command_queue myqueue = clCreateCommandQueue(ctx, devices[DEVICEID], 0, &err);
    if (err != CL_SUCCESS) {
        printf( "clCreateCommandQueue failed\n" );
        return 1;
    }

    /* Loading the kernel from file */
    printf("Loading program '%s'\n\n", "symm.cl");
	char *program_source = load_program_source("symm.cl");

	/* Collected time values, indexed using above constants */
	double t[2][3];
	memset(t, 0, 6*sizeof(double));

	float
		*ha = (float *)malloc(M*M * sizeof(float)),
		*hb = (float *)malloc(M*N * sizeof(float)),
		*hc,
		*hv = (float *)malloc(M*N * sizeof(float));
	for (int i = 0; i < M; i++)
		for (int j = 0; j < M; j++)
			if(i<j)
				A(ha,j,i) = A(ha,i,j) = 2.0/(7*i+5*j+1);
			else if(i==j)
				A(ha,i,i) = 2.0 / (7*i+5*j+1);
	for (int i = 0; i < M; i++)
		for (int j = 0; j < N; j++)
				BC(hb,i,j) = 5.0/(3*i+11*j+2);

	t[HOST][SETUP] = -walltime();
	hc = (float *) malloc (M*N * sizeof(float));
	t[HOST][SETUP] += walltime();

	t[DEVICE][SETUP] = -walltime ();

	/***************SUBTASK2: Set up the device memory and copy the input matrices *******/

	printf("Setting up device memory\n");	
	cl_mem da = clCreateBuffer(ctx,CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, M*M*sizeof(float), ha, &err);
	if(err != CL_SUCCESS) {
		printf("%s\n", print_cl_errstring(err));	
   	}
	cl_mem db = clCreateBuffer(ctx,CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, M*N*sizeof(float), hb, &err);
	if(err != CL_SUCCESS) {
		printf("%s\n", print_cl_errstring(err));	
   	}
	cl_mem dc = clCreateBuffer(ctx,CL_MEM_WRITE_ONLY, M*N*sizeof(float), NULL, &err);
	if(err != CL_SUCCESS) {
		printf("%s\n", print_cl_errstring(err));	
   	}
	

	/******************END SUBTASK***********/

	t[DEVICE][SETUP] += walltime ();

	/* Host checks answer - compute the key answer and make a copy of it */
	t[HOST][COMPUTE] = -walltime ();
	host_symm(hc, ha, hb);
	t[HOST][COMPUTE] += walltime ();
	t[HOST][REPORT] = -walltime();
	memcpy(hv, hc, M*N * sizeof(float));
	t[HOST][REPORT] += walltime();
	memset(hc, 0, M*N * sizeof(float));

	t[DEVICE][COMPUTE] = -walltime ();


    printf("Calling clCreateProgramWithSource\n");
	cl_program myprogram = clCreateProgramWithSource(ctx, 1, (const char**)&program_source, NULL, &err);
     if (err != CL_SUCCESS) {
        printf( "clCreateProgramWithSource failed\n" );
        return 1;
    }
    
    // Building the program 
    printf("Calling clBuildProgram\n");
    err = clBuildProgram(myprogram, 1, &devices[DEVICEID], NULL, NULL, NULL);
    if (err != CL_SUCCESS) {
        printf( "clBuildProgram failed\n" );
        return 1;
    }   

	
    // Creating the OpenCL kernel
    printf("Calling clCreateKernel\n");
    cl_kernel symm = clCreateKernel(myprogram, "symm", &err);
    if (err != CL_SUCCESS) {
        printf( "clCreateKernel failed\n" );
        return 1;
    }

    size_t localws[2] = {LOCALWSX, LOCALWSY};   //The local worksize
	size_t globalws[2] = {M, N};                //The global worksize

    /********** SUBTASK3: Execute the kernel on the device *******************/
    //Insert code here    
    //Set the kernel arguments 
    //Run the kernel


	err = clSetKernelArg(symm, 0, sizeof(cl_mem), (void *)&dc);
	err |= clSetKernelArg(symm, 1, sizeof(cl_mem), (void *)&da);
	err |= clSetKernelArg(symm, 2, sizeof(cl_mem), (void *)&db);
	err |= clSetKernelArg(symm, 3, sizeof(int), (void *)&dM);
	err |= clSetKernelArg(symm, 4, sizeof(int), (void *)&dN);

	if( err != CL_SUCCESS ) {
		printf("Error setting up arguments!\n%s\n", print_cl_errstring(err));
	}

	printf("Running kernel!\n");
	err = clEnqueueNDRangeKernel( myqueue, symm, 2, NULL, globalws, localws, 0, NULL, NULL );
        if ( err != CL_SUCCESS ) {
                printf("Error lanuching kernel\n");
		printf("%s\n", print_cl_errstring(err));
	}
   
	/********** SUBTASK3 END *************************************************/

    clFinish(myqueue);

    t[DEVICE][COMPUTE] += walltime ();
	t[DEVICE][REPORT] = -walltime ();

	/********** SUBTASK4: Transfer the result (in d_C) to hc on the host ******/
	
   	printf("Copying result back to host\n");
	err = clEnqueueReadBuffer( myqueue, dc, CL_TRUE, 0, N*M*sizeof(float), hc, 0, NULL, NULL);
	if( err != CL_SUCCESS ) {
		printf("%s\n", print_cl_errstring(err));
	}

	/********** SUBTASK4 END *************************************************/

	t[DEVICE][REPORT] += walltime ();

	/* The host can now check the device's answer for correctness */
	int num_err = 0;
	for (int i = 0; i < M; i++)
    {
		for (int j = 0; j < N; j++)
        {
			if(fabsf(BC(hc, i, j) - BC(hv, i, j) ) > 1E-6) 
				num_err++;
        }
    }

	printf("Number of errors: %d\n", num_err);

	/* Report overall execution time */
	printf(
		"Initialize workspace:\n\thost   %8.3f ms\n\tdevice %8.3f ms\n"
		"Compute the matrix:\n\thost   %8.3f ms\n\tdevice %8.3f ms\n"
		"Copy result:\n\thost   %8.3f ms\n\tdevice %8.3f ms\n"
		"Total time:\n\thost   %8.3f ms\n\tdevice %8.3f ms\n",
		t[HOST][SETUP] * 1E3, t[DEVICE][SETUP] * 1E3,
		t[HOST][COMPUTE]* 1E3, t[DEVICE][COMPUTE] * 1E3,
		t[HOST][REPORT] * 1E3, t[DEVICE][REPORT] * 1E3,
		(t[HOST][SETUP] + t[HOST][COMPUTE] + t[HOST][REPORT]) * 1E3,
		(t[DEVICE][SETUP] + t[DEVICE][COMPUTE] + t[DEVICE][REPORT]) * 1E3
	);

	/* Clean up and go home */
	free (ha), free (hb), free(hc), free(hv), free(platforms), free(devices);

	/********** SUBTASK5: Free the device memory objects ************************/
	//Insert code here
	/********** SUBTASK5 END *************************************************/
    clReleaseKernel(symm);
	clReleaseProgram(myprogram);
	clReleaseCommandQueue(myqueue);
    clReleaseContext(ctx);
	exit (EXIT_SUCCESS);

}

/*
 * Get system time to microsecond precision (ostensibly, the same as MPI_Wtime)
 */

double
walltime ( void )
{
	static struct timeval t;
	gettimeofday ( &t, NULL );
	return ( t.tv_sec + 1e-6 * t.tv_usec );
}



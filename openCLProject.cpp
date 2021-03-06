#include <iostream>
#include <fstream>

using namespace std;

#include "types.h"
#include "fileHandler.h"
#include "CL\cl.h"

using namespace std;

int main(void)
{
	cl_int         status = 0;
	cl_uint        numOfPlatforms;
	cl_platform_id platform = NULL;

	size_t         deviceListSize = 0;

	fileHandler    clKernelFileHandler;

	status = clGetPlatformIDs(0, NULL, &numOfPlatforms);

	if (status != CL_SUCCESS)
	{
		cout << "Error: Getting Platform IDs." << endl;

		return EXIT_FAILURE;
	}


	if (numOfPlatforms > 0)
	{
		cl_platform_id* platforms = (cl_platform_id*) new uint8[numOfPlatforms * sizeof(cl_platform_id)];
		
		//Get Platform Specific information
		status = clGetPlatformIDs(numOfPlatforms, platforms, NULL);
		
		if (status != CL_SUCCESS)
		{
			cout << "Error: Getting 0 platform." << endl;

			return EXIT_FAILURE;
		}

		for (uint8 i = 0; i < numOfPlatforms; i++)
		{
			char platformName[100];

			status = clGetPlatformInfo(platforms[i], CL_PLATFORM_VENDOR, sizeof(platformName), platformName, NULL);
		
			if (status != CL_SUCCESS)
			{
				cout << "Error: Getting Platform Information." << endl;

				return EXIT_FAILURE;
			}

			platform = platforms[i];

			if (!strcmp(platformName, "Advanced Micro Devices, Inc."))
			{
				break;
			}
		}

		cl_context_properties contextProperties[3] = {
			CL_CONTEXT_PLATFORM,
			(cl_context_properties)platform,
			0
		};

		cl_context context = clCreateContextFromType(contextProperties, CL_DEVICE_TYPE_CPU, NULL, NULL, &status);

		if (status != CL_SUCCESS)
		{
			cout << "Error: Creating Context." << endl;

			return EXIT_FAILURE;
		}

		status = clGetContextInfo(context, CL_CONTEXT_DEVICES, 0, NULL, &deviceListSize);

		if (status != CL_SUCCESS)
		{
			cout << "Error: Getting Context Info." << endl;

			return EXIT_FAILURE;
		}

		cl_device_id* devices = (cl_device_id*) new uint8[deviceListSize];
	
		if (devices == NULL)
		{
			cout << "Error: No devices found." << endl;

			return EXIT_FAILURE;
		}

		status = clGetContextInfo(context, CL_CONTEXT_DEVICES, deviceListSize, devices, NULL);

		if (status != CL_SUCCESS)
		{
			cout << "Error: Getting Devices info." << endl;

			return EXIT_FAILURE;
		}

		if (clKernelFileHandler.open((uint8*)"clKernel.cl"))
		{
			const char* sourceProgram = clKernelFileHandler.source().c_str(); //To c-string

			size_t      sourceSize = strlen(sourceProgram);

			cout << "Create CL Program." << endl;

			cl_program program = clCreateProgramWithSource(context, 1, &sourceProgram, &sourceSize, &status);

			if (status != CL_SUCCESS)
			{
				cout << "Error: Creating Program with Source." << endl;

				return EXIT_FAILURE;
			}
			else
			{
				cout << "Creating Program with Source Successfully." << endl;
			}

			status = clBuildProgram(program, 1, devices, NULL, NULL, NULL);

			if (status != CL_SUCCESS)
			{
				cout << "Error: Build Program." << endl;

				return EXIT_FAILURE;
			}

			cl_kernel kernel = clCreateKernel(program, "hellocl", &status);

			if (status != CL_SUCCESS)
			{
				cout << "Error: Creating Kenerl Object." << endl;

				return EXIT_FAILURE;
			}

			//Create Memory for kernel programming running
			cl_mem outputBuffer = clCreateBuffer(context, CL_MEM_ALLOC_HOST_PTR, 4*4*4, NULL, &status);

			if (status != CL_SUCCESS)
			{
				cout << "Error: Creating Memory for Kernel Object." << endl;

				return EXIT_FAILURE;
			}

			//Notify Kernel Object Memory instance it can use
			status = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void*)&outputBuffer);

			if (status != CL_SUCCESS)
			{
				cout << "Error: Set Memory for Kernel Object." << endl;

				return EXIT_FAILURE;
			}

			//#define CL_USE_DEPRECATED_OPENCL_1_2_APIS
			cl_command_queue commandQueue = clCreateCommandQueueWithProperties(context, devices[0], 0, &status);

			if (status != CL_SUCCESS)
			{
				cout << "Error: Creating Command Queue." << endl;

				return EXIT_FAILURE;
			}

			//put kernel in command queue to submit
			size_t globalThreads[] = { 4, 4 };
			size_t localThreads[] = { 2, 2 };

			status = clEnqueueNDRangeKernel(commandQueue, kernel, 2, NULL, globalThreads, localThreads, 0, NULL, NULL);

			if (status != CL_SUCCESS)
			{
				cout << "Error: Enqueuing Command Queue." << endl;

				return EXIT_FAILURE;
			}

			status = clFinish(commandQueue);

			if (status != CL_SUCCESS) 
			{
				cout << "Error: Finish command queue." << endl;
				
				return EXIT_FAILURE;
			}

			uint32* resultBuffer = new uint32[4 * 4];
			memset(resultBuffer, 0, 4 * 4 * 4);

			status = clEnqueueReadBuffer(commandQueue, outputBuffer, CL_TRUE, 0, 4* 4 * 4, resultBuffer, 0, NULL, NULL);

			if (status != CL_SUCCESS)
			{
				cout << "Error: Reading Result to Buffer." << endl;

				return EXIT_FAILURE;
			}

			//read result buffer
			for (uint8 i = 0; i < 16; i++)
			{
				cout << hex << resultBuffer[i] << " ";
			}
		}
		else
		{
			cout << "Error: Open CL Program failed." << endl;

			return EXIT_FAILURE;
		}
	}

	return 0;
}

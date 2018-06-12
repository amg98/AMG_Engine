/*
 * Debug.cpp
 *
 *  Created on: 1 jun. 2018
 *      Author: Andrés
 */

#include "Debug.h"
#include "Renderer.h"

#include <stdio.h>
#include <stdlib.h>

namespace AMG {

void Debug::showError(int code, void *param){
	switch(code){
		case 0:
			fprintf(stderr, "No error found\n");
			break;
		case 1:
			fprintf(stderr, "Failed to initialize GLFW\n");
			break;
		case 2:
			fprintf( stderr, "Failed to open GLFW window\n");
			break;
		case 3:
			fprintf(stderr, "Failed to initialize GLEW\n");
			break;
		case 4:
			fprintf(stderr, "Failed to create shader objects\n");
			break;
		case 5:
			fprintf(stderr, "File not found: %s\n", (char*)param);
			break;
		case 6:
			fprintf(stderr, "Error compiling shader: %s\n", (char*)param);
			break;
		case 7:
			fprintf(stderr, "Failed to create program object\n");
			break;
		case 8:
			fprintf(stderr, "Error linking shader: %s\n", (char*)param);
			break;
		case 9:
			fprintf(stderr, "Uniform not found: %s\n", (char*)param);
			break;
		case 10:
			fprintf(stderr, "Wrong signature: %s\n", (char*)param);
			break;
		case 11:
			fprintf(stderr, "Unsupported format: %s\n", (char*)param);
			break;
		case 12:
			fprintf(stderr, "Not a power-of-two texture: %s\n", (char*)param);
			break;
		default:
			fprintf(stderr, "Unknown error\n");
			break;
	}

	fflush(stderr);
	Renderer::exitProcess();
	exit(EXIT_FAILURE);
}

}

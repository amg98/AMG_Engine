/**
 * @file MeshData.h
 * @brief File which contains definitions of mesh data
 */

#ifndef MeshData_H_
#define MeshData_H_

// Includes C/C++
#include <vector>

// Includes OpenGL
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Own includes
#include "Entity.h"

namespace AMG {

/**
 * @struct buffer_info
 * @brief Holds information about a buffer
 */
typedef struct{
	GLuint id;			/**< Internal OpenGL ID of the buffer */
	int size;			/**< Buffer size (OpenGL macro-defined) */
	GLuint type;		/**< Type of buffer */
}buffer_info;

/**
 * @class MeshData
 * @brief Holds data for a Mesh
 */
class MeshData : public Entity {
protected:
	GLuint id;							/**< ID of the OpenGL VAO */
	GLuint indexid;						/**< ID of the indices buffer */
	std::vector<buffer_info> info;		/**< Vector holding information of all the defined buffers */
	int count;							/**< Number of indices / vertices in the mesh */
public:
	MeshData();
	void addBuffer(void *data, int size, int comps, GLuint type, bool drawRaw=false);
	void setIndexBuffer(void *data, int size);
	void draw();
	void drawRaw();
	void enableBuffers();
	void disableBuffers();
	virtual ~MeshData();
};

}

#endif

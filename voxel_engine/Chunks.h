#pragma once
class Chunk;
class voxel;

#include <stdlib.h>
#include <glm/glm.hpp>

using namespace glm;

class Chunks
{
public:
	Chunk** chunks;
	size_t volume;
	unsigned int w, h, d;

	Chunks(int w, int h, int d);
	~Chunks();

	Chunk* getChunk(int x, int y, int z);
	voxel* get(int x, int y, int z);
	void set(int x, int y, int z, int id);
	voxel* rayCast(vec3 start, vec3 dir, float maxLength, vec3& end, vec3& norm, vec3& iend);
};


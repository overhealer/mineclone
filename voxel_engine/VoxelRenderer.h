#pragma once
class Mesh;
class Chunk;

class VoxelRenderer
{
	float* buffer;
	size_t capacity;
public:
	VoxelRenderer(size_t capacity);
	~VoxelRenderer();

	Mesh* render(Chunk* chunk, const Chunk** chunks);
};


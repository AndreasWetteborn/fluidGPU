#pragma once
#include "Texture.h"


// contains logic for having one texture to read from and one to write to.
class DualTexture
{
public:
	DualTexture(int size, float *data);
	DualTexture(int sizeX, int sizeY,  float *data);
	~DualTexture(void);

	GLuint GetReadTexture(){return textures[readID]->GetID(); }
	GLuint GetWriteTexture(){return textures[writeID]->GetID();}
	int GetReadID(){return readID;}
	int GetWriteID(){return writeID;}
	void Swap();

	void ReadBackTexture(float *data);

private:
	Texture *textures[2];
	int readID, writeID;
};

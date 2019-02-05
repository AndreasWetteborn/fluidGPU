#include "DualTexture.h"

DualTexture::DualTexture(int textureSize, float *data): writeID(0), readID(1)
{
	textures[readID] = new Texture(textureSize, textureSize, data);
	textures[writeID] = new Texture(textureSize, textureSize, data);
}

DualTexture::DualTexture(int textureSizeX,int textureSizeY , float *data): writeID(0), readID(1)
{
	textures[readID] = new Texture(textureSizeX, textureSizeY, data);
	textures[writeID] = new Texture(textureSizeX, textureSizeY, data);
}

DualTexture::~DualTexture(void)
{
	//delete [] textures;
}

// Swap the old reading texture to the new writing texture and old writing to the new reading texture.
void DualTexture::Swap()
{
	if (writeID == 0) {
		writeID = 1;
		readID = 0;
	} else {
		writeID = 0;
		readID = 1;
	}
}

void DualTexture::ReadBackTexture(float *data)
{
	textures[readID]->Read(data);
}
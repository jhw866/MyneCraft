#pragma once
#include <random>
#include <math.h>
#include <Ogre.h>

static Ogre::Entity* grassCube = nullptr;
static Ogre::Entity* snowCube = nullptr;
static Ogre::Entity* sandCube = nullptr;

class Biome {
public:
	enum BiomeType{ GRASS, SNOW, SAND, NUM_BIOMES };

	Biome(Ogre::SceneManager*, BiomeType, int, int, int);
	~Biome(void);

	bool inBiome(int, int);
	Ogre::Entity* getCubeEntity(int, int);
	Ogre::Entity* getEntity(BiomeType);
	BiomeType getType() { return biomeType; }

protected:
	Ogre::SceneManager* mSceneManager;
	BiomeType biomeType;
	int centerX;
	int centerY;
	int radius;
};
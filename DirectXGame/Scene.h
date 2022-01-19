#pragma once
#include "Prerequisistes.h"
#include <vector>


class Scene
{
public:
	Scene();
	void BeginPlay();
	void Update(float deltaTime);
	void Render(void (&renderMethod)(const MeshRenderer& meshRenderer));
};


#pragma once
#include "GameplayComponent.h"
#include "Transform.h"
#include <memory>

class SceneComponent : public GameplayComponent
{
public:
	SceneComponent(Transform transform) : m_transform(std::make_unique<Transform>(transform))
	{
	}

	Transform* GetTransform();
protected:
	std::unique_ptr<Transform> m_transform;
};

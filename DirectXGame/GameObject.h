#pragma once
#include <vector>
#include <memory>
#include "Transform.h"
#include "BaseComponent.h"

class GameObject
{
public:
	GameObject();
	~GameObject();

	virtual void BeginPlay();
	virtual void Update(float deltaTime);
	virtual void OnDestroy();

	std::shared_ptr<Transform> GetTransform() const { return m_transform; }

	template<typename T>
	void AddComponent();
private:
	std::vector<std::shared_ptr<BaseComponent>> m_components;
	std::shared_ptr<Transform> m_transform;

	void ClearEmptyComponents();
};

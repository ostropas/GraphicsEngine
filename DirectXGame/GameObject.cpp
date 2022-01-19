#include "GameObject.h"
#include "BaseComponent.h"

GameObject::GameObject()
{
	m_transform = std::make_shared<Transform>();
}

GameObject::~GameObject()
{
}

void GameObject::BeginPlay()
{
	for (const auto& component : m_components)
	{
		component->BeginPlay();
	}
}

void GameObject::Update(float deltaTime)
{
	ClearEmptyComponents();
	for (const auto& component : m_components)
	{
		component->Update(deltaTime);
	}
}

void GameObject::OnDestroy()
{
	for (const auto& component : m_components)
	{
		component->OnDestroy();
	}
}

void GameObject::ClearEmptyComponents()
{
	std::vector<std::shared_ptr<BaseComponent>>::iterator i = m_components.begin();
	while (i != m_components.end())
	{
		if (i->get() == nullptr)
		{
			i = m_components.erase(i);
		}
		else
		{
			++i;
		}
	}
}

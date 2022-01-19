#include "SceneComponent.h"

Transform* SceneComponent::GetTransform()
{
	return m_transform.get();
}

#include "Transform.h"

Transform::Transform()
{
	m_localScale = Vector3D(1, 1, 1);
	m_localPos = Vector3D();
	m_localRot = Vector3D();
	CalcMembers();
}


void Transform::SetPosition(const Vector3D& pos)
{
	m_pos = pos;
	Vector3D globalOffset;
	std::shared_ptr<Transform> parentTransform = GetParent();
	while (parentTransform)
	{
		globalOffset += parentTransform->GetLocalPosition();
		parentTransform = GetParent();
	}
	m_localPos = m_pos - globalOffset;
}

void Transform::SetLocalPosition(const Vector3D& pos)
{
	m_localPos = pos;
	CalcMembers();
}

void Transform::SetRotation(const Vector3D& rot)
{
	m_rot = rot;
	Vector3D globalOffset;
	std::shared_ptr<Transform> parentTransform = GetParent();
	while (parentTransform)
	{
		globalOffset += parentTransform->GetLocalRotation();
		parentTransform = GetParent();
	}
	m_localRot = m_rot - globalOffset;
}

void Transform::SetLocalRotation(const Vector3D& rot)
{
	m_localRot = rot;
	CalcMembers();
}

void Transform::SetLocalScale(const Vector3D& scale)
{
	m_localScale = scale;
}

void Transform::SetScale(const Vector3D& scale)
{
	m_scale = scale;
	Vector3D globalScale;
	std::shared_ptr<Transform> parentTransform = GetParent();
	while (parentTransform)
	{
		Vector3D parentScale = parentTransform->GetLocalScale();
		globalScale = Vector3D(
			globalScale.m_x * parentScale.m_x,
			globalScale.m_y * parentScale.m_y,
			globalScale.m_z * parentScale.m_z
		);
		parentTransform = GetParent();
	}
	m_localScale = m_scale - globalScale;
}

void Transform::SetParent(const std::shared_ptr<Transform>& transform)
{
	m_parent = transform;
	CalcMembers();
}

void Transform::SetChild(const std::shared_ptr<Transform>& transform)
{
	m_childs.push_back(transform);
}

Vector3D Transform::TransformPoint(const Vector3D& position)
{
	return position + GetPosition();
}

void Transform::Update()
{
	CalcMembers();
}

void Transform::CalcMembers()
{
	Vector3D globalPos = m_localPos;
	Vector3D globalRot = m_localRot;
	Vector3D globalScale = m_localScale;
	std::shared_ptr<Transform> parentTransform = GetParent();
	while (parentTransform)
	{
		globalPos += parentTransform->GetLocalPosition();
		globalRot += parentTransform->GetLocalRotation();

		Vector3D parentScale = parentTransform->GetLocalScale();
		globalScale = Vector3D(
			globalScale.m_x * parentScale.m_x,
			globalScale.m_y * parentScale.m_y,
			globalScale.m_z * parentScale.m_z
		);

		parentTransform = GetParent();
	}
	m_pos = globalPos;
	m_rot = globalRot;
	m_scale = globalScale;
}


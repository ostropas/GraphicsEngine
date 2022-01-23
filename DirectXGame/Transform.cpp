#include "Transform.h"

Transform::Transform()
{
	m_localMatrix.setIdentity();
	m_worldMatrix.setIdentity();
}


void Transform::SetPosition(const Vector3D& pos)
{
	Matrix4x4 tmp;
	tmp.setIdentity();
	tmp *= m_worldMatrix.extractScale();
	tmp *= m_worldMatrix.extractRotation();
	tmp.setTranslation(pos);
	m_worldMatrix.setMatrix(tmp);
}

void Transform::SetLocalPosition(const Vector3D& pos)
{
	Matrix4x4 tmp;
	tmp.setIdentity();
	tmp *= m_localMatrix.extractScale();
	tmp *= m_localMatrix.extractRotation();
	tmp.setTranslation(pos);
	m_localMatrix.setMatrix(tmp);
}

void Transform::SetRotation(const Vector3D& rot)
{
	Matrix4x4 tmp;
	tmp.setIdentity();
	tmp.setRotation(rot * Deg2Rad);
	tmp*= m_worldMatrix.extractScale();
	tmp.setTranslation(m_worldMatrix.getTranslation());
	m_worldMatrix.setMatrix(tmp);
}

void Transform::SetLocalRotation(const Vector3D& rot)
{
	Matrix4x4 tmp;
	tmp.setIdentity();
	tmp.setRotation(rot * Deg2Rad);
	tmp*= m_localMatrix.extractScale();
	tmp.setTranslation(m_localMatrix.getTranslation());
	m_localMatrix.setMatrix(tmp);
}

void Transform::SetLocalScale(const Vector3D& scale)
{
	Matrix4x4 tmp;
	tmp.setIdentity();
	tmp.setScale(scale);
	tmp*= m_localMatrix.extractRotation();
	tmp.setTranslation(m_localMatrix.getTranslation());
	m_localMatrix.setMatrix(tmp);
}

void Transform::SetScale(const Vector3D& scale)
{
	Matrix4x4 tmp;
	tmp.setIdentity();
	tmp.setScale(scale);
	tmp*= m_worldMatrix.extractRotation();
	tmp.setTranslation(m_worldMatrix.getTranslation());
	m_worldMatrix.setMatrix(tmp);
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
	
}


#pragma once
#include "Vector3D.h";
#include <memory>
#include <vector>
#include "Matrix4x4.h"
#include "MathUtils.h"

class Transform
{
public:
	Transform();

	Vector3D GetPosition() const {
		return m_worldMatrix.getTranslation();
	}
	void SetPosition(const Vector3D& pos);
	Vector3D GetLocalPosition() const {
		return m_localMatrix.getTranslation();
	}
	void SetLocalPosition(const Vector3D& pos);

	Vector3D GetRotation() const {
		return m_worldMatrix.extractRotation().getEulerAngles() * Rad2Deg;
	}
	void SetRotation(const Vector3D& rot);
	Vector3D GetLocalRotation() const {
		return m_localMatrix.extractRotation().getEulerAngles() * Rad2Deg;
	}
	void SetLocalRotation(const Vector3D& rot);

	Vector3D GetLocalScale() const {
		return m_localMatrix.extractScale().getScale();
	}
	void SetLocalScale(const Vector3D& scale);

	Vector3D GetScale() const {
		return m_worldMatrix.extractScale().getScale();
	}
	void SetScale(const Vector3D& scale);

	void SetParent(const std::shared_ptr<Transform>& transform);	

	std::shared_ptr<Transform> GetParent() const {
		return m_parent;
	}

	Vector3D TransformPoint(const Vector3D& position);
	void Update();
private:
	void SetChild(const std::shared_ptr<Transform>& transform);
	std::shared_ptr<Transform> m_parent;
	std::vector<std::shared_ptr<Transform>> m_childs;
	Matrix4x4 m_worldMatrix;
	Matrix4x4 m_localMatrix;

	void CalcMembers();
};


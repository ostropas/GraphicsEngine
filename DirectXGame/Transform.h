#pragma once
#include "Vector3D.h";
#include <memory>
#include <vector>

class Transform
{
public:
	Transform();

	Vector3D GetPosition() const {
		return m_pos;
	}
	void SetPosition(const Vector3D& pos);
	Vector3D GetLocalPosition() const {
		return m_localPos;
	}
	void SetLocalPosition(const Vector3D& pos);

	Vector3D GetRotation() const {
		return m_rot;
	}
	void SetRotation(const Vector3D& rot);
	Vector3D GetLocalRotation() const {
		return m_localRot;
	}
	void SetLocalRotation(const Vector3D& rot);

	Vector3D GetLocalScale() const {
		return m_localScale;
	}
	void SetLocalScale(const Vector3D& scale);

	Vector3D GetScale() const {
		return m_scale;
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
	Vector3D m_localPos;
	Vector3D m_localScale;
	Vector3D m_localRot;

	Vector3D m_pos;
	Vector3D m_rot;
	Vector3D m_scale;

	void CalcMembers();
};


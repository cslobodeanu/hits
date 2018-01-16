#include "transform.h"
#include "gameobject.h"

using namespace MorganaEngine::Framework::GameObjects;
using namespace MorganaEngine::Framework::GameObjects;

__implement_component(Transform, Component);

Transform::Transform()
{
	localMatrix = Matrix::Identity;
	localMatrixMustBeUpdated = false;

	worldMatrix = Matrix::Identity;

	position = Vector3::Zero;
	rotation = Quaternion::Identity;
	scale = Vector3::One;

	localPosition = Vector3::Zero;
	localRotation = Quaternion::Identity;
	localScale = Vector3::One;

	localIsIdentity = true;

	positionIsZero = true;
	rotationIsZero = true;
	scaleIsOne = true;

	worldMatrixMustBeUpdated = true;
	invertedWorldMatrixMustBeUpdated = true;

	localInverted.SetIdentity();
	localInvertedMustBeUpdated = false;

	DoAddProperties();
}

Transform::~Transform()
{
}

void Transform::DoAddProperties()
{
	super::DoAddProperties();

	__add_property1(localPosition);
	__add_property1(localOffset);
	__add_property1(localRotation);
	__add_property1(localScale);

	__add_property1(position);
	__add_property1(rotation);
	__add_property1(scale);
}

const Matrix& Transform::GetLocalMatrix()
{
	if (localMatrixMustBeUpdated)
	{
		UpdateLocalMatrix();
	}
	return localMatrix;
}

void Transform::MultiplyWorldWithLocal(const Transform& world, const Transform& local, Transform& worldResult)
{
	const Vector3 sc = Vector3::Multiply(world.scale, local.localScale);
	const Quaternion rot = Quaternion::Concatenate(world.rotation, local.localRotation);

	const Vector3 p1 = local.localPosition + local.localOffset;
	const Vector3 p = Vector3::Multiply(p1, world.scale);
	const Vector3 pos = world.rotation * p + world.position;

	assign(worldResult.scale, sc);
	assign(worldResult.position, pos);
	assign(worldResult.rotation, rot);
}

void Transform::MultiplyWorldWithLocal(const Transform& world, const Transform1& local, Transform1& worldResult)
{
	worldResult.scale = Vector3::Multiply(world.scale, local.scale);
	worldResult.rotation = Quaternion::Concatenate(world.rotation, local.rotation);

	Vector3 p1 = local.position;
	Vector3 p = Vector3::Multiply(p1, world.scale);
	worldResult.position = world.rotation * p + world.position;
}

void Transform::MultiplyWorldWithLocal(const Transform1& world, const Transform1& local, Transform1& worldResult)
{
	worldResult.scale = Vector3::Multiply(world.scale, local.scale);
	worldResult.rotation = Quaternion::Concatenate(world.rotation, local.rotation);

	Vector3 p1 = local.position;
	Vector3 p = Vector3::Multiply(p1, world.scale);
	worldResult.position = world.rotation * p + world.position;
}

void Transform::OnPropertySet(_propertyDef* p)
{
	super::OnPropertySet(p);

	if (p == &localPosition || p == &localRotation || p == &localScale || p == &localOffset)
	{
		UpdateIdentityFlag();

		UpdateWorldFromLocal();

		localMatrixMustBeUpdated = true;

		worldMatrixMustBeUpdated = true;
		invertedWorldMatrixMustBeUpdated = true;

		localInvertedMustBeUpdated = true;
	}

	if (p == &position || p == &rotation || p == &scale)
	{
		UpdateLocalFromWorld();

		worldMatrixMustBeUpdated = true;
		invertedWorldMatrixMustBeUpdated = true;

		positionIsZero = p != &position;
		rotationIsZero = p != &rotation;
		scaleIsOne = p != &scale;

		UpdateChildrenWorld();
	}
}

void Transform::UpdateIdentityFlag()
{
	localIsIdentity = localPosition == Vector3::Zero && localRotation == Quaternion::Identity && localScale == Vector3::One;
}

void Transform::UpdateLocalMatrix()
{
	if (localIsIdentity)
	{
		localMatrix = Matrix::Identity;
	}
	else
	{
		localMatrix = Matrix::CreateFromQuaternion(localRotation);
		localMatrix.Scale(localScale);
		localMatrix.Translation() = localPosition + localOffset;
	}
	localMatrixMustBeUpdated = false;
}

void Transform::UpdateWorldMatrix()
{
	worldMatrix = Matrix::CreateFromQuaternion(rotation);
	worldMatrix.Scale(scale);
	worldMatrix.Translation() = position;
	worldMatrixMustBeUpdated = false;
}

const Matrix& Transform::GetWorldMatrix()
{
	if (worldMatrixMustBeUpdated)
	{
		UpdateWorldMatrix();
	}

	return worldMatrix;
}

const Matrix* Transform::GetWorldMatrixPtr()
{
	if (worldMatrixMustBeUpdated)
	{
		UpdateWorldMatrix();
	}

	return &worldMatrix;
}

const Matrix* Transform::GetInvertedWorldMatrixPtr()
{
	if (invertedWorldMatrixMustBeUpdated)
	{
		GetWorldMatrixPtr();
		Matrix::Invert(&worldMatrix, &invertedWorldMatrix);
		invertedWorldMatrixMustBeUpdated = false;
	}

	return &invertedWorldMatrix;
}

void Transform::UpdateWorldFromLocal()
{
	UpdateWorld();

	UpdateChildrenWorld();

	worldMatrixMustBeUpdated = true;
	invertedWorldMatrixMustBeUpdated = true;
}

void Transform::UpdateLocalFromWorld()
{
	if (GetParent() == NULL)
	{
		assign(localPosition, position);
		assign(localRotation, rotation);
		assign(localScale, scale);
	}
	else
	{
		Transform1 inv;
		InvertWorld(*(GetParent()), inv);
		Transform1 tmp1, tmp2;

		tmp1.position = position;
		tmp1.scale = scale;
		tmp1.rotation = rotation;

		MultiplyWorldWithLocal(inv, tmp1, tmp2);
		assign(localPosition, tmp2.position - localOffset);
		assign(localRotation, tmp2.rotation);
		assign(localScale, tmp2.scale);
		//localOffset = Vector3::Zero;
	}

	localIsIdentity = false;

	localInvertedMustBeUpdated = true;
	localMatrixMustBeUpdated = true;
}

void Transform::UpdateWorld()
{
	if (GetParent() == NULL)
	{
		assign(localPosition, position);
		assign(localRotation, rotation);
		assign(localScale, scale);
	}
	else
	{
		MultiplyWorldWithLocal(*GetParent(), *this, *this);
		positionIsZero = false;
		rotationIsZero = false;
		scaleIsOne = false;
	}
	worldMatrixMustBeUpdated = true;
	invertedWorldMatrixMustBeUpdated = true;
}

void Transform::UpdateChildrenWorld()
{
	if (gameObject != NULL)
	{
		for (int i = 0; i < gameObject->GetChildrenCount(); i++)
		{
			Transform* ct = gameObject->GetChild(i)->transform;
			ct->UpdateWorld();
			ct->UpdateChildrenWorld();
		}
	}
}

Transform* Transform::GetParent()
{
	if (gameObject != NULL && gameObject->GetParent() != NULL)
		return gameObject->GetParent()->transform;
	return NULL;
}

void Transform::InvertLocal(const Transform& t, Transform1& res)
{
	res.position = -t.localPosition;
	res.rotation = Quaternion::Conjugate(t.localRotation);
	res.scale = Vector3(1.0f / t.localScale->x, 1.0f / t.localScale->y, 1.0f / t.localScale->z);
}

void Transform::InvertWorld(const Transform& t, Transform1& res)
{
	res.position = -t.position;
	res.rotation = Quaternion::Conjugate(t.rotation);
	res.scale = Vector3(1.0f / t.scale->x, 1.0f / t.scale->y, 1.0f / t.scale->z);
}

const Transform1& Transform::Inverse()
{
	if (localInvertedMustBeUpdated)
	{
		InvertLocal(*this, localInverted);
		localInvertedMustBeUpdated = false;
	}

	return localInverted;
}

Vector3 Transform::TransformPoint(Vector3 point) const
{
	Vector3 v = rotation * point;
	v = position + Vector3::Multiply(v, scale);
	return v;
}

Vector3 Transform::InverseTransformPoint(Vector3 point)
{
	Inverse();
		
	Vector3 v = localInverted.rotation * point;
	v = localInverted.position + Vector3::Multiply(&v, &localInverted.scale);
	return v;
}


Vector3 Transform::TransformNormal(Vector3 nrm) const
{
	return rotation * nrm;
}

const bool Transform::LocalIsIdentity() const
{
	return localIsIdentity;
}

void Transform::OnParentAttached()
{
	UpdateWorld();
}

void Transform::assign(_property<Vector3>& dst, const Vector3& src)
{
	memcpy((void*)(&dst->x), &src.x, 3 * sizeof(float));
}

void Transform::assign(_property<Quaternion>& dst, const Quaternion& src)
{
	memcpy((void*)(&dst->x), &src.x, 4 * sizeof(float));
}

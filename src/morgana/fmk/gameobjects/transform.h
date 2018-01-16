#ifndef __MORGANA_FMK_COMPONENTS_BASE_TRANSFORM_H__
#define __MORGANA_FMK_COMPONENTS_BASE_TRANSFORM_H__

#include "metypes.h"
#include "reflection.h"
#include "component.h"

namespace MorganaEngine
{
	namespace Framework
	{
		namespace GameObjects
		{
			class Transform1
			{
			public:
				Vector3				position;
				Quaternion			rotation;
				Vector3				scale;

				void SetIdentity()
				{
					position = Vector3::Zero;
					rotation = Quaternion::Identity;
					scale = Vector3::One;
				}
			};

			class Transform : public Component
			{
				__declare_component(Transform, Component);
			public:

				Transform();
				Transform(const Transform& other)
				{
					localPosition = other.localPosition;
					localRotation = other.localRotation;
					localScale = other.localScale;
					localIsIdentity = other.localIsIdentity;

					position = other.position;
					rotation = other.rotation;
					scale = other.scale;

					positionIsZero = other.positionIsZero;
					rotationIsZero = other.rotationIsZero;
					scaleIsOne = other.scaleIsOne;
				}

				virtual ~Transform();

				_property<Vector3>		localPosition;
				_property<Quaternion>	localRotation;
				_property<Vector3>		localScale;

				_property<Vector3>		position;
				_property<Quaternion>	rotation;
				_property<Vector3>		scale;

				bool					positionIsZero;
				bool					rotationIsZero;
				bool					scaleIsOne;

				const Matrix&		GetLocalMatrix();
				const Matrix&		GetWorldMatrix();
				const Matrix*		GetWorldMatrixPtr();
				const Matrix*		GetInvertedWorldMatrixPtr();

// 				Transform			MultiplyLocalWithLocal(const Transform& other) const;
// 				static Transform	MultiplyLocalWithLocal(const Transform& t1, const Transform& t2);

				Vector3				TransformPoint(Vector3 point) const;
				Vector3				InverseTransformPoint(Vector3 point);
				Vector3				TransformNormal(Vector3 nrm) const;

				const Transform1&	Inverse();

				const bool			LocalIsIdentity() const;
					
			protected:

				_property<Vector3>	localOffset;

				friend class		GameObject;
				void				DoAddProperties();
				virtual void		OnPropertySet(_propertyDef* p);

				Matrix				localMatrix;
				bool				localMatrixMustBeUpdated;
				bool				localIsIdentity;
				void				UpdateIdentityFlag();
				virtual void		UpdateLocalMatrix();

				Matrix				worldMatrix;
				bool				worldMatrixMustBeUpdated;
				Matrix				invertedWorldMatrix;
				bool				invertedWorldMatrixMustBeUpdated;
				virtual void		UpdateWorldMatrix();

				virtual void		UpdateWorldFromLocal();
				virtual void		UpdateLocalFromWorld();

				static void			MultiplyWorldWithLocal(const Transform& world, const Transform& local, Transform& worldResult);
				static void			MultiplyWorldWithLocal(const Transform& world, const Transform1& local, Transform1& worldResult);
				static void			MultiplyWorldWithLocal(const Transform1& world, const Transform1& local, Transform1& worldResult);

				void				UpdateWorld();

				void				UpdateChildrenWorld();

				virtual void		OnParentAttached();

				Transform*			GetParent();

				static void			InvertLocal(const Transform& t, Transform1& res);
				static void			InvertWorld(const Transform& t, Transform1& res);

				Transform1			localInverted;
				bool				localInvertedMustBeUpdated;

				static void			assign(_property<Vector3>& dst, const Vector3& src);
				static void			assign(_property<Quaternion>& dst, const Quaternion& src);
			};
		}
	}
}
#endif
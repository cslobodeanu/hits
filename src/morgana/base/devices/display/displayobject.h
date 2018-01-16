#ifndef __MORGANA_BASE_DEVICES_DISPLAY_DISPLAY_OBJECT_H__
#define __MORGANA_BASE_DEVICES_DISPLAY_DISPLAY_OBJECT_H__

#include "reflection.h"
#include "rendepipelinedefs.h"

namespace MorganaEngine
{
	namespace Base
	{
		using namespace Reflection;
		namespace Devices
		{
			namespace Display
			{
				class DisplayDevice;
				class DisplayObject : public MEObject, public IsNamed, public IsVersioned
				{
					__declare_class(DisplayObject, MEObject);
				protected:
					DisplayObject();
					void		GenerateHandleEventually();
				public:

					DisplayObject(const int type, const int bindingPoint);
					virtual ~DisplayObject();

					virtual const unsigned long GetHandle() const;	

					const bool	IsValid() const;
					const int	GetDisplayObjectType() const;
					const int	GetBindingPoint() const;

					const void* Lock();
					void		Unlock();

					bool		valid;

					const void* GetCreateParams() { return createParams; }


				friend class DisplayObjectFactory;
				friend class DisplayDevice;

				protected:

					virtual void	OnObjectBound(int bindingPoint, int stage);

					void			SetHandle(const unsigned long h);
					void			SetUserData(void* ptr);
					const void*		GetUserData() const;

					void*			createParams;
					void			SetCreateParams(void* params, const int size);

					DisplayDevice*	display;

					void			SetLockedPtr(const void* ptr);
					const bool		IsLocked();
					const void*		GetLockedPtr();

				private:

					void			DeleteCreateParams();

					unsigned long	handle, bindingPoint;
					char			type;

					void			*userData;

					const void*		lockedPtr;
				};
			}
		}
	}
}

#define __CreateParams				((CreateParams_t*)createParams)


#endif
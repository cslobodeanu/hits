#ifndef __MORGANA_FMK_CANVAS_INPUT_EVENTS_MANAGER_H__
#define __MORGANA_FMK_CANVAS_INPUT_EVENTS_MANAGER_H__

#include "gameobjects.h"
#include "recttransform.h"

namespace MorganaEngine
{
	namespace Framework
	{
		namespace Canvas
		{
			class InputEventsReceiver;
			class InputEventsManager : public Component
			{
				__declare_component(InputEventsManager, Component);
			public:

				InputEventsManager();
				virtual ~InputEventsManager();

				void		AddReceiver(InputEventsReceiver* ier);

			protected:

				Array<InputEventsReceiver*> receivers;

				virtual void Start();
				virtual void Update();

				int							touchState;
				RectTransform*				capture;

				void UpdateEvents();
				void OnObjectDestroyed(MEObject* obj);

				virtual void				OnDestroy();
			};
		}
	}
}

#endif
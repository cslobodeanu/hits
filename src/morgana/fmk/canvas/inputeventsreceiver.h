#ifndef __MORGANA_FMK_CANVAS_INPUT_EVENTS_RECEIVER_H__
#define __MORGANA_FMK_CANVAS_INPUT_EVENTS_RECEIVER_H__

#include "gameobjects.h"
#include "inputeventsmanager.h"

namespace MorganaEngine
{
	namespace Framework
	{
		namespace Canvas
		{
			class InputEventsReceiver : public Component
			{
				__declare_component(InputEventsReceiver, Component);

			public:

				InputEventsReceiver();
				virtual ~InputEventsReceiver();

				Action<Component*, Vector2, bool&>	OnTouchDown; // sender, pos, handled
				Action<Component*, Vector2, bool&>	OnTouchUp;
				Action<Component*, Vector2, bool&>	OnTouchMove;

			protected:

				InputEventsManager*		inputManager;

				virtual void Start();
				virtual void Update();
			};
		}
	}
}

#endif
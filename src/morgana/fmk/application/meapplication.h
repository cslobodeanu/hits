#ifndef __MORGANA_FMK_APPLICATION_MEAPPLICATION_H__
#define __MORGANA_FMK_APPLICATION_MEAPPLICATION_H__

#include "../../base/base.h"
#include "../../base/views/meview.h"
#include "meclock.h"
#include "../gameobjects/gameobject.h"

namespace MorganaEngine
{
	namespace Framework
	{
		namespace Application
		{
			class MorganaEngine::Base::Views::MEView;
			using namespace GameObjects;
			class MEApplication : public MEObject
			{
				__declare_class(MEApplication, MEObject);
			protected:
				bool							shouldClose;
				bool							isInitialized;
				bool							isShutdownComplete;
			friend class GameObject;

			void								CallCreateGameObject(GameObject* obj);
			void								CallDeleteGameObject(GameObject* obj);


			void								LoadConfiguration();

			MEApplication();

			public:
				virtual ~MEApplication();

				virtual void	Initialize();
				void			Run();
				void			NotifyShutdown();
				const bool		IsShuttingdown() const;
				const bool		IsShutdownComplete() const;
				void			Shutdown();

				void			CallApplicationWait();

				static MEApplication* main;

			public:
				
				Action<>				OnInitialize;
				Action<>				OnShutdown;
				Action<>				OnRunFrame;
				Action<>				OnBeginFrame;
				Action<>				OnEndFrame;
				Action<GameObject*>		OnCreateGameObject;
				Action<GameObject*>		OnDeleteGameObject;
				Action<>				OnApplicationWait;

				String					GetDescription();

				MorganaEngine::Base::Views::MEView*	mainView;

			protected:
				virtual void			RunFrame();
			};
		}
	}
}

#endif
#ifndef __MORGANA_FMK_RESOURCES_RESOURCE_CONTEXT_H__
#define __MORGANA_FMK_RESOURCES_RESOURCE_CONTEXT_H__

#include "reflection.h"
#include "meclock.h"
#include "callbacks.h"

namespace MorganaEngine
{
	namespace Framework
	{
		namespace Resources
		{
			class ResourceContext : public MEObject
			{
				__declare_class(ResourceContext, MEObject);
			public:
				ResourceContext();
				virtual ~ResourceContext();

				void				Free();

				template<class translatedClass>
				translatedClass*	Load(const char* paths, const char* args = NULL);

				void				Append(MEObject* obj, const char* name);

				String				GetPathsForObject(const MEObject* base);

				void				RemoveObjectReferences(const void* obj);

				static ResourceContext*	global;
				static ResourceContext*	current;

				String				GetPreferredPath(MEObject* rf);
				String				GetPreferredExtension(MEObject* rf);

				static Action<MEObject*>	OnResourceLoaded;
				static Action<MEObject*>	OnResourceChanged;
				static Action<MEObject*>	OnResourceRemoved;

				template<class T>
				Array<T*>					GetLoadedResources();

			private:

				MEObject*			_Load(const char* paths, CLASSDESC type, const char* args = NULL);

				MEObject*			LoadImage(const char* paths);
				MEObject*			LoadTexture(const char* paths);
				MEObject*			LoadMaterial(const char* paths);
				MEObject*			LoadShaderFile(const char* paths, const char* args = NULL, bool noAddRef = false);
				MEObject*			LoadShader(const char* paths, const char* args = NULL);
				MEObject*			LoadFont(const char* paths);
				MEObject*			LoadTrueTypeFont(const char* paths);
				MEObject*			LoadRawData(const char* paths);
				MEObject*			LoadTextData(const char* paths);

				template<class T>
				T*					TranslateFile(const char* paths, const char* args);

				struct ResPair_t;
				void				Reload(ResPair_t& rp);
				void				ReloadImage(ResPair_t& rp);
				void				ReloadTexture(ResPair_t& rp);
				void				ReloadMaterial(ResPair_t& rp);
				void				ReloadShaderFile(ResPair_t& rp);
				void				ReloadShader(ResPair_t& rp);
				void				ReloadFont(ResPair_t& rp);

				MEObject*			GetExisting(const char* paths, CLASSDESC type, const char* args);
				ResPair_t*			GetExisting2(const char* paths, CLASSDESC type, const char* args);
				
				void				OnObjectDestroyed(MEObject* obj);
				bool				defaultResourcesLoaded;
				void				AppendDefaultResources();


				struct ResPair_t
				{
					MEObject*			object;
					StringHash			paths;
					StringHash			fullPaths;
					StringHash			args;
#ifdef OS_WINDOWS
					Array<DateTime_t>	fileTimeStamps;
#endif

					ResPair_t()
					{
						object = NULL;
					}

					ResPair_t(MEObject* o, const char* paths, const char* args)
					{
						object = o;
						this->paths = paths;
						this->args = args;
					}

					ResPair_t(const ResPair_t& other)
					{
						object = other.object;
						paths = other.paths;
						args = other.args;

					}
				};

				UsageArray<ResPair_t>	loadedResources;

				void				AddReference(MEObject* obj, const char* paths, const char* args);

#ifdef OS_WINDOWS
				int				updateIndex;
				bool			registeredForUpdate;
				void			RegisterForUpdate();
				void			OnAppRunFrame();
#endif
			};

			template<class T>
			Array<T*> ResourceContext::GetLoadedResources()
			{
				Array<T*> list;
				for (UsageArrayItem<ResPair_t>* rp = loadedResources.ptr(); rp < loadedResources.end(); rp++)
				{
					if (rp->ref.object->IsKindOf(T::AsClassDesc) == false) continue;
					list.Add(__dynamic_cast<T>(rp->ref.object));
				}

				return list;
			}

			template<class translatedClass>
			translatedClass* ResourceContext::Load(const char* paths, const char* args /*= NULL*/)
			{
				AppendDefaultResources();
				MEObject* res = _Load(paths, translatedClass::AsClassDesc, args);
				OnResourceLoaded(res);
				return __dynamic_cast<translatedClass>(res);
			}

			template<class T>
			T* ResourceContext::TranslateFile(const char* paths, const char* args)
			{
				MEObject* res = GetExisting(paths, T::AsClassDesc, args);
				if (res)
					return __dynamic_cast<T>(res);

				T* obj = ResourceFile::Translate<T>(paths, args);
				AddReference(obj, paths, args);
				return obj;
			}
		}
	}
}

#endif
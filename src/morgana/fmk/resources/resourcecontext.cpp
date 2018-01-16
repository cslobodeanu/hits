#include "resourcecontext.h"
#include "resourcefile.h"
#include "images/imagefile.h"
#include "materials/material.h"
#include "shaders/shaderfile.h"
#include "fonts/fonts.h"
#include "rawdata.h"
#include "textdata.h"
#include "devices.h"

#ifdef OS_WINDOWS
#include "../application/meapplication.h"
using namespace MorganaEngine::Framework::Application;
#endif

using namespace MorganaEngine::Framework::Resources::Images;
using namespace MorganaEngine::Framework::Resources::Materials;
using namespace MorganaEngine::Framework::Resources::Shaders;
using namespace MorganaEngine::Framework::Resources;

__implement_class(ResourceContext, MEObject);

ResourceContext*	ResourceContext::global = new ResourceContext();
ResourceContext*	ResourceContext::current = NULL;

Action<MEObject*>	ResourceContext::OnResourceLoaded;
Action<MEObject*>	ResourceContext::OnResourceChanged;
Action<MEObject*>	ResourceContext::OnResourceRemoved;

ResourceContext::ResourceContext()
{
	registeredForUpdate = false;
	updateIndex = 0;

	defaultResourcesLoaded = false;

	MEObject::OnObjectDestroy += Callback(OnObjectDestroyed, MEObject*);
}

ResourceContext::~ResourceContext()
{
	Free();

	MEObject::OnObjectDestroy -= Callback(OnObjectDestroyed, MEObject*);
}

void ResourceContext::RemoveObjectReferences(const void* obj)
{
	for (int i = 0; i < loadedResources.Length(); i++)
	{
		if (loadedResources[i].ref.object == obj)
		{
			loadedResources.RemoveNoFast(i);
			i--;
		}
	}
}

MEObject* ResourceContext::_Load(const char* paths, CLASSDESC type, const char* args /*= NULL*/)
{
	MEObject* res = GetExisting(paths, type, args);
	if (res != NULL)
		return res;

	if (type == ImageFile::AsClassDesc)
		return LoadImage(paths);

	if (type == Texture::AsClassDesc)
		return LoadTexture(paths);

	if (type == Material::AsClassDesc)
		return LoadMaterial(paths);

	if (type == ShaderFile::AsClassDesc)
		return LoadShaderFile(paths, args);

	if (type == ShaderProgram::AsClassDesc)
		return LoadShader(paths, args);

	if (type == Font::AsClassDesc)
		return LoadFont(paths);

	if (type == FontTrueType::AsClassDesc)
		return LoadTrueTypeFont(paths);

	if (type == RawData::AsClassDesc)
		return LoadRawData(paths);

	if (type == TextData::AsClassDesc)
		return LoadTextData(paths);

	return NULL;
}

MEObject* ResourceContext::LoadImage(const char* paths)
{
	MEObject* res = GetExisting(paths, ImageFile::AsClassDesc, NULL);
	if (res)
		return res;
	return TranslateFile<ImageFile>(paths, NULL);
}

MEObject* ResourceContext::LoadTexture(const char* paths)
{
	MEObject* res = GetExisting(paths, Texture::AsClassDesc, NULL);
	if (res != null)
		return res;

	bool newImg = GetExisting(paths, ImageFile::AsClassDesc, NULL) == NULL;

	ImageFile* img = __dynamic_cast<ImageFile>(LoadImage(paths));
	if (img == NULL || img->ConversionWasOk() == false)
		return NULL;

	Texture* tex = new Texture();
	implementation<DisplayObjectFactory>()->Create(tex, &Texture::CreateParams_t(img->width, img->height));
	Rect r(0, 0, img->width, img->height);
	implementation<DisplayObjectFactory>()->UpdateSubresource(tex, r, 0, img->GetPixels());

	AddReference(tex, paths, NULL);

	if (newImg)
	{
		SAFE_DEL(img);
	}

	return tex;
}

MEObject* ResourceContext::LoadMaterial(const char* paths)
{
	MEObject* res = GetExisting(paths, Material::AsClassDesc, NULL);
	if (res)
		return res;
	return TranslateFile<Material>(paths, NULL);
}

MEObject* ResourceContext::LoadShaderFile(const char* paths, const char* args /*= NULL*/, bool noAddRef /*= false*/)
{
	MEObject* res = GetExisting(paths, ShaderFile::AsClassDesc, args);
	if (res)
		return res;

	String p = paths;
	if (p.Find(';') < 0)
	{
		// duplicate
		p += ".vsh;";
		p += paths;
		p += ".fsh;";
	}

	ShaderFile* sf = TranslateFile<ShaderFile>(p, args);
	if (sf == NULL || sf->ConversionWasOk() == false)
	{
		return NULL;
	}

	sf->SetName(p);

	if (noAddRef == false)
		AddReference(sf, paths, args);

	return sf;
}

MEObject* ResourceContext::LoadShader(const char* paths, const char* args /*= NULL*/)
{
	bool newShader = GetExisting(paths, ShaderFile::AsClassDesc, args) == NULL;
	ShaderFile* sf = __dynamic_cast<ShaderFile>(LoadShaderFile(paths, args));

	DisplayDevice::main->BindObjectNull(DISPLAY_OBJECT_SHADER_PROGRAM);

	ShaderProgram* prg = new ShaderProgram();
	implementation<DisplayObjectFactory>()->GenerateHandle(prg);
	implementation<DisplayObjectFactory>()->Create(prg, &DisplayObjects::ShaderProgram::CreateParams_t(sf->m_sSource));
	prg->SetName(sf->GetName());

	if (sf->m_sSource.Contains("#tags:"))
	{
		const int i = sf->m_sSource.Find("#tags:");
		String ss = sf->m_sSource.Substring(i);
		ss[ss.Find('\n')] = 0;
		prg->tags = ss;
	}

	AddReference(prg, sf->GetFilePaths(), args);

	if (newShader)
		SAFE_DEL(sf);

	return prg;
}

MEObject* ResourceContext::LoadFont(const char* paths)
{
	MEObject* res = GetExisting(paths, Font::AsClassDesc, NULL);
	if (res)
		return res;
	return TranslateFile<Font>(paths, NULL);
}

MEObject* ResourceContext::LoadTrueTypeFont(const char* paths)
{
	MEObject* res = GetExisting(paths, FontTrueType::AsClassDesc, NULL);
	if (res)
		return res;
	return TranslateFile<FontTrueType>(paths, NULL);
}

MEObject* ResourceContext::LoadRawData(const char* paths)
{
	MEObject* res = GetExisting(paths, RawData::AsClassDesc, NULL);
	if (res)
		return res;
	return TranslateFile<RawData>(paths, NULL);
}

MEObject* ResourceContext::LoadTextData(const char* paths)
{
	MEObject* res = GetExisting(paths, TextData::AsClassDesc, NULL);
	if (res)
		return res;
	return TranslateFile<TextData>(paths, NULL);
}

void ResourceContext::Reload(ResourceContext::ResPair_t& rp)
{
	if (rp.object->IsKindOf(ImageFile::AsClassDesc))
		ReloadImage(rp);
	else
	if (rp.object->IsKindOf(Texture::AsClassDesc))
		ReloadTexture(rp);
	else
	if (rp.object->IsKindOf(Material::AsClassDesc))
		ReloadMaterial(rp);
	else
	if (rp.object->IsKindOf(ShaderFile::AsClassDesc))
		ReloadShaderFile(rp);
	else
	if (rp.object->IsKindOf(ShaderProgram::AsClassDesc))
		ReloadShader(rp);
	else
	if (rp.object->IsKindOf(Font::AsClassDesc))
		ReloadFont(rp);

	OnResourceChanged(rp.object);
}

void ResourceContext::ReloadImage(ResourceContext::ResPair_t& rp)
{
	ImageFile* newOne = ResourceFile::Translate<ImageFile>(rp.paths);
	ImageFile* img = __dynamic_cast<ImageFile>(rp.object);
	if (img->width != newOne->width || img->height != newOne->height || img->bits != newOne->bits)
		img->Alloc(newOne->width, newOne->height, newOne->bits);
	memcpy(img->GetPixels(), newOne->GetPixels(), newOne->GetRowSize() * newOne->height);
	SAFE_DEL(newOne);
}

void ResourceContext::ReloadTexture(ResourceContext::ResPair_t& rp)
{
	bool newImg = GetExisting(rp.paths, ImageFile::AsClassDesc, rp.args) == NULL;

	ImageFile* img = __dynamic_cast<ImageFile>(LoadImage(rp.paths));
	if (img == NULL) return;
	Texture* tex = __dynamic_cast<Texture>(rp.object);

	if (img->width != tex->GetBounds().width || img->height != tex->GetBounds().height)
		implementation<DisplayObjectFactory>()->Create(tex, &Texture::CreateParams_t(img->width, img->height));

	Rect r(0, 0, img->width, img->height);
	implementation<DisplayObjectFactory>()->UpdateSubresource(tex, r, 0, img->GetPixels());

	if (newImg)
	{
		SAFE_DEL(img);
	}
}

void ResourceContext::ReloadMaterial(ResourceContext::ResPair_t& rp)
{
	DEBUG_OUT("Reload material [%s]...", rp.paths.c_str());

	Material* newOne = ResourceFile::Translate<Material>(rp.paths);
	if (newOne->ConversionWasOk())
	{
		Material* mat = __dynamic_cast<Material>(rp.object);
		mat->Copy(*newOne);
		mat->IncrementVersion();
		DEBUG_OUT("New version is %d", mat->GetVersion());
	}
	else
	{
		DEBUG_OUT("Failed to import material [%s].", rp.paths.c_str());
	}
	SAFE_DEL(newOne);
}

void ResourceContext::ReloadShaderFile(ResourceContext::ResPair_t& rp)
{
	ShaderFile* newOne = ResourceFile::Translate<ShaderFile>(rp.paths);
	ShaderFile* sf = __dynamic_cast<ShaderFile>(rp.object);
	sf->m_sSource = newOne->m_sSource;
	SAFE_DEL(newOne);
}

void ResourceContext::ReloadShader(ResourceContext::ResPair_t& rp)
{
	bool newOne = GetExisting(rp.paths, ShaderFile::AsClassDesc, rp.args) == NULL;

	ShaderFile* sf = __dynamic_cast<ShaderFile>(LoadShaderFile(rp.paths, NULL, true));
	if (sf == NULL || sf->ConversionWasOk() == false)
	{
		DEBUG_OUT("Failed to reload shader file [%s].", rp.paths.c_str());
		return;
	}

	DisplayDevice::main->BindObjectNull(DISPLAY_OBJECT_SHADER_PROGRAM);

	ShaderProgram* sp = __dynamic_cast<ShaderProgram>(rp.object);
	implementation<DisplayObjectFactory>()->Create(sp, &DisplayObjects::ShaderProgram::CreateParams_t(sf->m_sSource));

	if (newOne)
	{
		SAFE_DEL(sf);
	}
}

void ResourceContext::ReloadFont(ResourceContext::ResPair_t& rp)
{
	Font* newOne = ResourceFile::Translate<Font>(rp.paths);
	Font* fnt = __dynamic_cast<Font>(rp.object);
	fnt->Copy(*newOne);
	SAFE_DEL(newOne);
}

MEObject* ResourceContext::GetExisting(const char* paths, CLASSDESC type, const char* args)
{
	ResPair_t* rp = GetExisting2(paths, type, args);
	return rp ? rp->object : NULL;
}

ResourceContext::ResPair_t* ResourceContext::GetExisting2(const char* paths, CLASSDESC type, const char* args)
{
	const int pathsHash = StringHash::ComputeHashCode(paths);
	const int argsHash = StringHash::ComputeHashCode(args);
	for (UsageArrayItem<ResPair_t>* rp = loadedResources.ptr(); rp < loadedResources.end(); rp++)
	{
		if (rp->ref.object->IsKindOf(type) == false) continue;
		if (rp->ref.paths.Equals(paths, pathsHash) == false) continue;
		if (rp->ref.args.Equals(args, argsHash) == false) continue;

		UsageArrayItem<ResPair_t>::IncrementUsage(rp, loadedResources.ptr());
		return &rp->ref;
	}
	return NULL;
}

String ResourceContext::GetPathsForObject(const MEObject* base)
{
	for (UsageArrayItem<ResPair_t>* rp = loadedResources.ptr(); rp < loadedResources.end(); rp++)
	{
		if (rp->ref.object != base) continue;
		return rp->ref.paths;
	}
	return "";
}

void ResourceContext::AddReference(MEObject* obj, const char* paths, const char* args)
{
	if (obj == NULL)
		return;
	ResPair_t rp(obj, paths, args);

#ifdef OS_WINDOWS
	RegisterForUpdate();

	String paths2 = paths;
	ResourceFile* tf = __dynamic_cast<ResourceFile>(obj);
	if (tf != NULL)
	{
		paths2 = tf->GetFilePaths();
	}

	rp.fullPaths = paths2;

	Array<String> _paths = Paths::Split(paths2);
	for (int i = 0; i < _paths.Length(); i++)
	{
		String fullPath = Paths::GetPathInDocuments(_paths[i]);
		DateTime_t dt = singleton<MFiles>()->GetFileTimeStamp(fullPath);
		rp.fileTimeStamps.Add(dt);
	}
#endif

	loadedResources.Push(rp);
}

void ResourceContext::OnObjectDestroyed(MEObject* obj)
{
	RemoveObjectReferences(obj);
}

#ifdef OS_WINDOWS
void ResourceContext::RegisterForUpdate()
{
	if (registeredForUpdate) return;

	MEApplication::main->OnRunFrame += Callback(OnAppRunFrame);
	updateIndex = 0;

	registeredForUpdate = true;
}

void ResourceContext::OnAppRunFrame()
{
	for (int i = 0; i < 16; i++)
	{
		if (updateIndex >= loadedResources.Length())
		{
			updateIndex = 0;
			break;
		}

		ResPair_t& rp = loadedResources[updateIndex].ref;
		String paths = rp.paths;
		ResourceFile* tf = __dynamic_cast<ResourceFile>(rp.object);
		if (tf != NULL)
		{
			paths = tf->GetFilePaths();
		}
		Array<String> _paths = Paths::Split(paths);
		for (int k = 0; k < rp.fileTimeStamps.Length(); k++)
		{
			String fullPath = Paths::GetPathInDocuments(_paths[k]);
			DateTime_t dt = singleton<MFiles>()->GetFileTimeStamp(fullPath);
			if (memcmp(&dt, &rp.fileTimeStamps[k], sizeof(DateTime_t)) != 0)
			{
				DEBUG_OUT("Reloading [%s]...", fullPath.c_str());
				Reload(rp);
				memcpy(&rp.fileTimeStamps[k], &dt, sizeof(DateTime_t));
			}
		}

		updateIndex++;
	}
}

void ResourceContext::Append(MEObject* obj, const char* name)
{
	ResPair_t* rp = GetExisting2(name, obj->GetClassDesc(), NULL);
	if (rp == NULL)
	{
		AddReference(obj, name, NULL);
	}
	else
	{
		rp->object = obj;
	}
}

void ResourceContext::AppendDefaultResources()
{
	if (defaultResourcesLoaded) return;

	Append(Texture::DefaultWhite, "white");
	Append(Texture::DefaultGray, "gray");
	Append(Texture::DefaultBlack, "black");
	Append(Texture::DefaultRed, "red");
	Append(Texture::DefaultGreen, "green");
	Append(Texture::DefaultBlue, "blue");
	Append(Texture::DefaultUp, "up");
	Append(Texture::DefaultGradient, "gradient");
	Append(Texture::DefaultBlob, "blob");
	Append(Texture::DefaultSmallDisc, "smalldisc");
	Append(Texture::DefaultBigDisc, "bigdisc");
	Append(Texture::DefaultSmallCircle, "smallcircle");
	Append(Texture::DefaultCheckered, "checkered");
	Append(Texture::DefaultCheckered16, "checkered16");

	defaultResourcesLoaded = true;
}

String ResourceContext::GetPreferredPath(MEObject* rf)
{
	if (rf->IsKindOf(Font::AsClassDesc))
		return "fonts";

	if (rf->IsKindOf(Material::AsClassDesc))
		return "materials";

	if (rf->IsKindOf(ShaderFile::AsClassDesc))
		return "shaders";

	return "";
}

String ResourceContext::GetPreferredExtension(MEObject* rf)
{
	if (rf->IsKindOf(Font::AsClassDesc))
		return "mfnt";

	if (rf->IsKindOf(Material::AsClassDesc))
		return "mat";

	return "";
}

void ResourceContext::Free()
{
	while(loadedResources.Length() > 0)
	{
		UsageArrayItem<ResPair_t>& last = loadedResources.Last();
		SAFE_DEL(last.ref.object);
	}
}

#endif
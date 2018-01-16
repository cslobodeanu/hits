#include "material.h"
#include "pipelinestates.h"
#include "displaydevice.h"
#include "base.h"
#include "meresources.h"

using namespace MorganaEngine::Framework::Resources::Materials;
using namespace MorganaEngine::Framework::Resources;

__implement_class(Material, ResourceFile);

#define RENDER_QUEUE_EXTRA 2000
#define QUEUE_TO_INDEX(q)		((int)q * RENDER_QUEUE_EXTRA + RENDER_QUEUE_EXTRA / 2)

const Material* Material::DefaultCanvas = NULL;
const Material*	Material::DefaultMaskedCanvas = NULL;
const Material*	Material::DefaultFontCanvas = NULL;

Material::Material()
{
	shader = null;
	depthTest = DepthState::LessEqual;
	depthWrite = true;
	blend = BlendState::Opaque;
	cull = RasterizerState::CullNone;
	stencilTest = StencilState::Disabled;

	textures.SetElemsBatchSize(1);
	textures.SetNullOnAlloc();
	textures.SetLength(1);
	textures[0] = ResourceContext::global->Load<Texture>("checkered16");

	shininess = 20.0f;
	specular = Color::white;
	emissive = Color::black;

	renderQueueIndex = 0;

	hash = 0;

	SetVersion(0);
}

Material::~Material()
{
}

void Material::ImportFromStreams(const StreamList&  streams, const char* args /*= NULL*/)
{
	Stream* s = streams.First();

	DEBUG_OUT("Loading material [%s] ...", s->location.c_str());

	conversionWasOk = false;

	const int fsz = s->Size();
	if (fsz <= 0)
	{
		DEBUG_OUT("[1]Cannot open [%s].", s->location.c_str());
		return;
	}

	if (s->Open(Stream::READ) == false)
	{
		DEBUG_OUT("[2]Cannot open [%s].", s->location.c_str());
		return;
	}

	char* buf = new char[fsz + 1];
	s->ReadBuffer(buf, fsz);
	buf[fsz] = 0;

	tinyxml2::XMLDocument doc;
	tinyxml2::XMLError err = doc.Parse(buf);
	delete[] buf;

	if (err != tinyxml2::XML_SUCCESS)
	{
		DEBUG_OUT("XML parse error (%d) for %s", s->location.c_str());
		return;
	}

	tinyxml2::XMLElement* titleElement = doc.FirstChildElement("material");
	if (titleElement == NULL)
	{
		ASSERT(false, "Not a material");
	}

	const char* atrstr = titleElement->Attribute("name");
	if (atrstr != null)
		name = atrstr;
	else
		name = "Unnamed material";

	tinyxml2::XMLNode* dataNode = titleElement->FirstChildElement("materialdata");

	if (dataNode != null)
	{
		tinyxml2::XMLElement* child = dataNode->FirstChildElement();
		while (child != null)
		{
			FillValuesFromParsedBlock(child);
			child = child->NextSiblingElement();
		}
	}
	else
	{
		DEBUG_OUT("No [materialdata] in material file.");
		return;
	}

	conversionWasOk = true;
}

void Material::FillValuesFromParsedBlock(tinyxml2::XMLElement* elem)
{
	String s = elem->Value();
	//if (s.Equals("parameter") == false) return;

	String param = s;// elem->Attribute("name");
	String val = elem->Attribute("value");

	if (param.Equals("cull"))
	{
		if (val.EqualsNC("none"))
			cull = RasterizerState::CullNone;
		else
			if (val.EqualsNC("front"))
				cull = RasterizerState::CullFront;
			else
				if (val.EqualsNC("back"))
					cull = RasterizerState::CullBack;
	}
	else
	if (param.Equals("depthwrite"))
	{
		if (val.ToBool() == false)
			depthWrite = false;
	}
	else
	if (param.Equals("depthtest"))
	{
		if (val.EqualsNC("disabled") || val.EqualsNC("none"))
			depthTest = DepthState::Disabled;

		if (val.EqualsNC("lessequal") || val.EqualsNC("lequal"))
			depthTest = depthWrite ? DepthState::LessEqual : DepthState::LessEqualNoWrite;

		if (val.EqualsNC("equal"))
			depthTest = DepthState::Equal;
	}
	else
	if (param.Equals("blend"))
	{
		if (val.EqualsNC("opaque") || val.EqualsNC("none"))
			blend = BlendState::Opaque;
		else
			if (val.EqualsNC("add") || val.EqualsNC("additive"))
				blend = BlendState::Additive;
			else
				if (val.EqualsNC("multiply"))
					blend = BlendState::Multiply;
				else
					if (val.EqualsNC("alpha"))
						blend = BlendState::AlphaBlend;
	}
	else
	if (param.Equals("shininess") || param.Equals("specularpower"))
	{
		shininess = val.ToFloat();
	}
	else
	if (param.Equals("specularcolor") || param.Equals("specular"))
	{
		val.Trim(" ()");
		val.ToFloats(specular.r, specular.g, specular.b);
	}
	else
	if (param.Equals("emissivecolor") || param.Equals("emissive"))
	{
		val.Trim(" ()");
		val.ToFloats(emissive.r, emissive.g, emissive.b);
	}

	bool isTextureParam = false;

	for (int i = 0; i < 8; i++)
	{
		String str;
		str.Printf("texture%d", i);

		if (i == 0)
		{
			if (param.Equals(str) == false && param.Equals("maintex") == false) continue;
		}
		else
		{
			if (param.Equals(str) == false) continue;
		}

		val = elem->Attribute("path");
		
		Texture* tex = NULL;
		if (val.StartsWith("default:"))
		{
			str = val;
			str.Delete(0, 8);
			if (str.Equals("checkered"))
				tex = Texture::DefaultCheckered16;
		}
		else
		{
			tex = ResourceContext::global->Load<Texture>(val);
		}
		textures.SetNo(i, tex);

		isTextureParam = true;
		break;
	}

	if (isTextureParam)
	{

	}
	else
	if (param.Equals("shaders"))
	{
		String vsh, fsh;

		const char* atr = elem->Attribute("file");
		if (atr != null)
		{
			vsh.Printf("%s.vsh", Paths::RemoveExtension(atr).c_str());
			fsh.Printf("%s.fsh", Paths::RemoveExtension(atr).c_str());
		}
		else
		{
			atr = elem->Attribute("vertex");
			if (atr == null)
				DEBUG_OUT("Missing vertex shader !");
			vsh.Printf("%s.vsh", atr);

			atr = elem->Attribute("fragment");
			if (atr == null)
			{
				atr = elem->Attribute("pixel");
				if (atr == null)
					DEBUG_OUT("Missing pixel shader !");
			}
			fsh.Printf("%s.fsh", atr);
		}

		if (vsh.IsEmpty() == false && fsh.IsEmpty() == false)
		{
			String paths = vsh + ";" + fsh;
			shader = ResourceContext::global->Load<ShaderProgram>(paths);
			BuildParameterInfo();
		}

	}
	else
	if (name.Equals("queue"))
	{
		int plus = val.Find('+');
		int minus = val.Find('-');

		renderQueueIndex = QUEUE_TO_INDEX(RenderQueue::Opaque);

		val.ToLower();

		if (val.StartsWith("transparent")) renderQueueIndex = QUEUE_TO_INDEX(RenderQueue::Transparent);
		if (val.StartsWith("background")) renderQueueIndex = QUEUE_TO_INDEX(RenderQueue::Background);
		if (val.StartsWith("foreground")) renderQueueIndex = QUEUE_TO_INDEX(RenderQueue::Foreground);
		if (val.StartsWith("alphatest")) renderQueueIndex = QUEUE_TO_INDEX(RenderQueue::AlphaTested);

		if (plus > 0)
		{
			String s = val.Substring(plus + 1);
			renderQueueIndex += s.ToInt();
		}

		if (minus > 0)
		{
			String s = val.Substring(minus + 1);
			renderQueueIndex -= s.ToInt();
		}
	}


	// setup default params
	currentParameterSet = ParameterSet::Default;
	currentParameterSet = ParameterSet::User;


	/*

	

	*/

	ComputeHash();
}

const bool Material::SaveToStream(Stream* s)
{
	return false;
}

const Material::RenderQueue Material::GetBaseRenderQueue() const
{
	int rq = renderQueueIndex / RENDER_QUEUE_EXTRA;
	return (RenderQueue)rq;
}

Texture* Material::SetTexture(Texture* t, const int stage /*= 0*/)
{
	if (textures.Length() <= stage)
		textures.SetLength(stage + 1);
	textures[stage] = t != NULL ? t : Texture::DefaultWhite;

	ComputeHash();

	return textures[stage];
}

const bool Material::HasParameter(const char* name)
{
	return shader ? shader->HasParameter(name) : false;
}

void Material::BuildParameterInfo()
{
	int count = shader->uniformEntries.Length();
	defaultParameters.SetLength(count);
	for (int i = 0; i < count; i++)
	{
		ParameterData_t& pd = defaultParameters[i];
		pd.uniform = &shader->uniformEntries[i];
		pd.UploadData<char>(null, shader->uniformEntries[i].dataSize);
	}

	userParameters.Copy(defaultParameters);
}
 void Material::UploadParameters(ParameterSet which)
{
	Array<ParameterData_t>& params = GetParameterSet(which);

	bool skippedParams = false;
	for (int i = 0; i < params.Length(); i++)
	{
		if (params[i].data == null) continue;

		if (params[i].user == (which == ParameterSet::User))
			shader->UploadUniformData(params[i].uniform, params[i].data);
		else
		{
			skippedParams = true;
		}
	}


	if (skippedParams)
	{
		for (int i = 0; i < params.Length(); i++)
		{
			if (params[i].user != (which == ParameterSet::User))
			{
				params.RemoveNo(i);
				i--;
			}
		}
	}
}

 const void Material::ChangeShader(ShaderProgram* s)
 {
	 if (shader != s)
	 {
		 shader = s;

		 ComputeHash();

		 BuildParameterInfo();
	 }
 }

 void Material::LoadDefaults()
 {
	 DefaultCanvas = ResourceContext::global->Load<Material>("defaultcanvas");
	 DefaultMaskedCanvas = ResourceContext::global->Load<Material>("defaultmaskcanvas");
	 DefaultFontCanvas = ResourceContext::global->Load<Material>("defaultfontcanvas");
 }

 void Material::ComputeHash()
 {
	 hash = 0;

	 hash += depthWrite ? 1 : 0;
	 hash += (unsigned long)depthTest;
	 hash += (unsigned long)blend;
	 hash += (unsigned long)cull;

	 hash += (unsigned long)shader;
// 	 float						shininess;
// 	 Color						specular;
// 	 Color						emissive;

	 for (int i = 0; i < textures.Length(); i++)
		 hash += (unsigned long)textures[i];
 }

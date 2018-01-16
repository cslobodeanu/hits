#include "shaderproxy.h"
#include "meresources.h"

using namespace MorganaEngine::Framework::Resources::Shaders;

ShaderProxy::ShaderProxy()
{
	shader = NULL;
}

ShaderProxy::~ShaderProxy()
{

}

const int ShaderProxy::UseDefinition(const char* defName)
{
	int h = GetVariant(defName);
	if (h >= 0)
	{
		UseDefinition(h);
		return h;
	}

	ProxyStruct_t ps;
	ps.define = defName;
	String paths = ResourceContext::global->GetPathsForObject(shader);

	ps.shader = ResourceContext::global->Load<ShaderProgram>(paths, CreateArgs(defName));
	variants.Add(ps);

	return variants.Length() - 1;
}

const void ShaderProxy::UseDefinition(const int handle)
{
	ProxyStruct_t& ps = variants[handle];
	shader = ps.shader;
}

const int ShaderProxy::GetVariant(const char* defName)
{
	const int h = String::ComputeHashCode(defName);
	int index = 0;
	for (ProxyStruct_t* ps = variants.ptr(); ps < variants.end(); ps++, index++)
	{
		if (ps->define.Equals(defName, h))
			return index;
	}

	return -1;
}

String ShaderProxy::CreateArgs(const char* defName)
{
	String _d = defName;
	Array<String> items;
	_d.Split(";", items);
	
	String ret = "";
	for (int i = 0; i < items.Length(); i++)
	{
		ret += "#define ";
		ret += items[i];
		ret += "\n";
	}

	return ret;
}
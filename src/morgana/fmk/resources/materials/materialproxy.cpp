#include "materialproxy.h"

using namespace Morgana::Framework::Resources::Materials;

__implement_class(MaterialProxy, Material);

MaterialProxy::MaterialProxy()
{
	shaderVersion = -1;
	parametersChecksum = 0;
}

MaterialProxy::~MaterialProxy()
{

}

MaterialProxy::MaterialProxy(const Material& other)
{
	shaderVersion = -1;

	Copy(other);
	
	source = &other;
	SetVersion(other.GetVersion());

	SyncParameters();

	parametersChecksum = 0;
}

void MaterialProxy::SyncParameters()
{
	const int uc = shader->shader->uniforms.Length();
	if (shaderVersion == shader->shader->GetVersion()) return;

	parameters.SetLength(uc);
	for (int i = 0; i < uc; i++)
		parameters[i].uniform = &shader->shader->uniforms[i];

	shaderVersion = shader->shader->GetVersion();
}

const void MaterialProxy::UploadShaderParameters() const
{
	DisplayDevice::main->BindObject(shader->shader);
	if (version == source->GetVersion()) return;
	for (ParamEntry_t* p = parameters.ptr(); p < parameters.end(); p++)
	{
		if (p->size > 0 && p->version > 0)
			shader->shader->SetData(p->uniform, p->data);
	}
}

const bool MaterialProxy::HasParameter(const char* name)
{
	SyncParameters();
	return shader->shader->HasParameter(name);
}

MaterialProxy::ParamEntry_t* MaterialProxy::GetParameter(const char* name)
{
	SyncParameters();

	const int hash = StringHash::ComputeHashCode(name);
	for (ParamEntry_t* p = parameters.ptr(); p < parameters.end(); p++)
	{
		if (p->uniform->locationStr.Equals(name, hash))
		{
			return p;
		}
	}
	return NULL;
}

MaterialProxy::ParamEntry_t* MaterialProxy::GetParameter(const int& index)
{
	if (index < 0 || index >= parameters.Length()) return NULL;
	return &parameters[index];
}

const bool MaterialProxy::Equals(const MaterialProxy* other) const
{
	if (Material::Equals(other) == false) return false;

	if (parameters.Length() != other->parameters.Length()) return false;
	if (parametersChecksum != other->parametersChecksum) return false;
	if (parametersChecksum == 0) return true;

	for (ParamEntry_t* p1 = parameters.ptr(), *p2 = other->parameters.ptr(); p1 < parameters.end(); p1++, p2++)
	{
		if (p1->version <= 0 && p2->version <= 0) continue;
		if (p1->size != p2->size) return false;
		if (memcmp(p1->data, p2->data, p1->size) != 0) return false;
	}

	return true;
}

void MaterialProxy::CheckVersion()
{
	if (source != NULL && source->GetVersion() > GetVersion())
	{
		Copy(*source);
	}
}

void MaterialProxy::AddToChecksum(const char* ptr, const int size)
{
	int i = 0;
	while (i < size)
	{
		parametersChecksum += ptr[i];
		i++;
	}
}

void MaterialProxy::RemoveFromChecksum(const char* ptr, const int size)
{
	int i = 0;
	while (i < size)
	{
		parametersChecksum -= ptr[i];
		i++;
	}
}

const int MaterialProxy::GetParameterIndex(const char* name)
{
	SyncParameters();

	const int hash = StringHash::ComputeHashCode(name);
	int index = 0;
	for (ParamEntry_t* p = parameters.ptr(); p < parameters.end(); p++)
	{
		if (p->uniform->locationStr.Equals(name, hash))
		{
			return index;
		}
		index++;
	}
	return -1;
}

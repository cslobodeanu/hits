#include "shaderprogram.h"
#include "../displaydevice.h"

using namespace MorganaEngine::Base::Devices::Display;
using namespace MorganaEngine::Base::Devices::Display::DisplayObjects;


__implement_class(ShaderProgram, DisplayObject);

ShaderProgram::ShaderProgram() : DisplayObject(DISPLAY_OBJECT_SHADER_PROGRAM, 0)
{
}

ShaderProgram::~ShaderProgram()
{
}

ShaderProgram::UniformEntry_t	ShaderProgram::UniformEntry_t::Invalid(-1000);

ShaderProgram::UniformEntry_t& ShaderProgram::GetUniformByName(const char* name)
{
	int hash = StringHash::ComputeHashCode(name);
	for (UniformEntry_t* ue = uniformEntries.ptr(); ue < uniformEntries.end(); ue++)
	{
		if (ue->locationStr.Equals(name, hash))
		{
			return *ue;
		}
	}

	return UniformEntry_t::Invalid;
}


int ShaderProgram::GetParameterIndex(const char* name)
{
	int hash = StringHash::ComputeHashCode(name);
	int index = 0;
	for (UniformEntry_t* ue = uniformEntries.ptr(); ue < uniformEntries.end(); ue++)
	{
		if (ue->locationStr.Equals(name, hash))
		{
			return index;
		}

		index++;
	}

	return -1;
}

const bool ShaderProgram::HasParameter(const char* name) const
{
	int hash = StringHash::ComputeHashCode(name);
	for (UniformEntry_t* ue = uniformEntries.ptr(); ue < uniformEntries.end(); ue++)
	{
		if (ue->locationStr.Equals(name, hash))
		{
			return true;
		}
	}

	return false;
}

void ShaderProgram::AddUniformEntry(const char* name, int type, int numElems)
{
	if(HasParameter(name))
	{
		return;
	}

	UniformEntry_t u;
	u.locationStr = name;
	u.dataType = type;
	u.numElements = numElems;
	u.dataSize = GetDataSize(u);
	uniformEntries.Add(u);
}

#define VALIDATE_UNIFORM(name, type, numElems)  \
	UniformEntry_t& ue = GetUniformByName(name); \
	if (ue == UniformEntry_t::Invalid) return; \
	ASSERT(ue.dataType == UNIFORM_##type); \
	ASSERT(ue.numElements >= numElems);

#define VALIDATE_UNIFORM2(index, type, numElems)  \
	if (index < 0) return; \
	UniformEntry_t& ue = uniformEntries[index]; \
	ASSERT(ue.dataType == UNIFORM_##type); \
	ASSERT(ue.numElements >= numElems);

void ShaderProgram::SetFloat(const char* uniformName, const float& f)
{
	VALIDATE_UNIFORM(uniformName, float, 1);
	display->BindObject(this);
	display->UploadShaderUniformData(&ue, &f, 1);
}

void ShaderProgram::SetVector(const char* uniformName, const Vector2& v)
{
	VALIDATE_UNIFORM(uniformName, Vector2, 1);
	display->BindObject(this);
	display->UploadShaderUniformData(&ue, &v.x, 1);
}

void ShaderProgram::SetVector(const char* uniformName, const Vector3& v)
{
	VALIDATE_UNIFORM(uniformName, Vector3, 1);
	display->BindObject(this);
	display->UploadShaderUniformData(&ue, &v.x, 1);
}

void ShaderProgram::SetVector(const char* uniformName, const Vector4& v)
{
	VALIDATE_UNIFORM(uniformName, Vector4, 1);
	display->BindObject(this);
	display->UploadShaderUniformData(&ue, &v.x, 1);
}

void ShaderProgram::SetMatrix(const char* uniformName, const Matrix& v)
{
	VALIDATE_UNIFORM(uniformName, Matrix, 1);
	display->BindObject(this);
	display->UploadShaderUniformData(&ue, v.Ptr(), 1);
}

void ShaderProgram::SetColor(const char* uniformName, const Color255& c)
{
	UniformEntry_t& ue = GetUniformByName(uniformName);
	if (ue == UniformEntry_t::Invalid) return;
	ASSERT(ue.dataType == UNIFORM_Vector3 || ue.dataType == UNIFORM_Vector4);
	ASSERT(ue.numElements >= 1);

	if(ue.dataType == UNIFORM_Vector3)
	{
		Vector3 v; float a;
		c.ToFloat(v.x, v.y, v.z, a);
		SetVector(uniformName, v);
	}
	if(ue.dataType == UNIFORM_Vector4)
	{
		Vector4 v;
		c.ToFloat(v.x, v.y, v.z, v.w);
		SetVector(uniformName, v);
	}
}

void ShaderProgram::SetColor(const char* uniformName, const Color& c)
{
	UniformEntry_t& ue = GetUniformByName(uniformName);
	if (ue == UniformEntry_t::Invalid) return;
	ASSERT(ue.dataType == UNIFORM_Vector3 || ue.dataType == UNIFORM_Vector4);
	ASSERT(ue.numElements >= 1);

	if (ue.dataType == UNIFORM_Vector3)
	{
		SetVector(uniformName, (Vector3)c);
	}
	if (ue.dataType == UNIFORM_Vector4)
	{
		SetVector(uniformName, (Vector4)c);
	}
}

void ShaderProgram::SetInt(const char* uniformName, const int& i)
{
	VALIDATE_UNIFORM(uniformName, int, 1);
	display->BindObject(this);
	display->UploadShaderUniformData(&ue, &i);
}
	

void ShaderProgram::SetFloats(const char* uniformName, const float* f, const int count = 1)
{
	VALIDATE_UNIFORM(uniformName, float, count);
	display->BindObject(this);
	display->UploadShaderUniformData(&ue, f, count);
}

void ShaderProgram::SetVectors(const char* uniformName, const Vector2* v, const int count = 1)
{
	VALIDATE_UNIFORM(uniformName, Vector2, count);
	display->BindObject(this);
	display->UploadShaderUniformData(&ue, v, count);
}

void ShaderProgram::SetVectors(const char* uniformName, const Vector3* v, const int count = 1)
{
	VALIDATE_UNIFORM(uniformName, Vector3, count);
	display->BindObject(this);
	display->UploadShaderUniformData(&ue, v, count);
}

void ShaderProgram::SetVectors(const char* uniformName, const Vector4* v, const int count = 1)
{
	VALIDATE_UNIFORM(uniformName, Vector4, count);
	display->BindObject(this);
	display->UploadShaderUniformData(&ue, v, count);
}

void ShaderProgram::SetMatrices(const char* uniformName, const Matrix* v, const int count = 1)
{
	VALIDATE_UNIFORM(uniformName, Matrix, count);
	display->BindObject(this);
	display->UploadShaderUniformData(&ue, v, count);
}

void ShaderProgram::SetInts(const char* uniformName, const int* i, const int count = 1)
{
	VALIDATE_UNIFORM(uniformName, int, count);
	display->BindObject(this);
	display->UploadShaderUniformData(&ue, i, count);
}

void ShaderProgram::SetFloat(const int& uniformIndex, const float& f)
{
	VALIDATE_UNIFORM2(uniformIndex, float, 1);
	display->BindObject(this);
	display->UploadShaderUniformData(&ue, &f, 1);
}

void ShaderProgram::SetVector(const int& uniformIndex, const Vector2& v)
{
	VALIDATE_UNIFORM2(uniformIndex, Vector2, 1);
	display->BindObject(this);
	display->UploadShaderUniformData(&ue, &v.x, 1);
}

void ShaderProgram::SetVector(const int& uniformIndex, const Vector3& v)
{
	VALIDATE_UNIFORM2(uniformIndex, Vector3, 1);
	display->BindObject(this);
	display->UploadShaderUniformData(&ue, &v.x, 1);
}

void ShaderProgram::SetVector(const int& uniformIndex, const Vector4& v)
{
	VALIDATE_UNIFORM2(uniformIndex, Vector4, 1);
	display->BindObject(this);
	display->UploadShaderUniformData(&ue, &v.x, 1);
}

void ShaderProgram::SetMatrix(const int& uniformIndex, const Matrix& v)
{
	VALIDATE_UNIFORM2(uniformIndex, Matrix, 1);
	display->BindObject(this);
	display->UploadShaderUniformData(&ue, v.Ptr(), 1);
}

void ShaderProgram::SetColor(const int& uniformIndex, const Color255& c)
{
	if (uniformIndex < 0) return;
	UniformEntry_t& ue = uniformEntries[uniformIndex];

	ASSERT(ue.dataType == UNIFORM_Vector3 || ue.dataType == UNIFORM_Vector4);
	ASSERT(ue.numElements >= 1);

	if (ue.dataType == UNIFORM_Vector3)
	{
		Vector3 v; float a;
		c.ToFloat(v.x, v.y, v.z, a);
		SetVector(uniformIndex, v);
	}
	if (ue.dataType == UNIFORM_Vector4)
	{
		Vector4 v;
		c.ToFloat(v.x, v.y, v.z, v.w);
		SetVector(uniformIndex, v);
	}
}

void ShaderProgram::SetInt(const int& uniformIndex, const int& i)
{
	VALIDATE_UNIFORM2(uniformIndex, int, 1);
	display->BindObject(this);
	display->UploadShaderUniformData(&ue, &i);
}


void ShaderProgram::SetFloats(const int& uniformIndex, const float* f, const int count = 1)
{
	VALIDATE_UNIFORM2(uniformIndex, float, count);
	display->BindObject(this);
	display->UploadShaderUniformData(&ue, f, count);
}

void ShaderProgram::SetVectors(const int& uniformIndex, const Vector2* v, const int count = 1)
{
	VALIDATE_UNIFORM2(uniformIndex, Vector2, count);
	display->BindObject(this);
	display->UploadShaderUniformData(&ue, v, count);
}

void ShaderProgram::SetVectors(const int& uniformIndex, const Vector3* v, const int count = 1)
{
	VALIDATE_UNIFORM2(uniformIndex, Vector3, count);
	display->BindObject(this);
	display->UploadShaderUniformData(&ue, v, count);
}

void ShaderProgram::SetVectors(const int& uniformIndex, const Vector4* v, const int count = 1)
{
	VALIDATE_UNIFORM2(uniformIndex, Vector4, count);
	display->BindObject(this);
	display->UploadShaderUniformData(&ue, v, count);
}

void ShaderProgram::SetMatrices(const int& uniformIndex, const Matrix* v, const int count = 1)
{
	VALIDATE_UNIFORM2(uniformIndex, Matrix, count);
	display->BindObject(this);
	display->UploadShaderUniformData(&ue, v, count);
}

void ShaderProgram::SetInts(const int& uniformIndex, const int* i, const int count = 1)
{
	VALIDATE_UNIFORM2(uniformIndex, int, count);
	display->BindObject(this);
	display->UploadShaderUniformData(&ue, i, count);
}

void ShaderProgram::UploadUniformData(ShaderProgram::UniformEntry_t* ue, const void* data)
{
	display->BindObject(this);
	display->UploadShaderUniformData(ue, data, ue->numElements);
}

void ShaderProgram::ClearUniformEntries()
{
	uniformEntries.SetLength(0);
}

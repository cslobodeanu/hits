#ifndef __MORGANA_BASE_DEVICES_DISPLAY_OBJECTS_SHADER_PROGRAM_H__
#define __MORGANA_BASE_DEVICES_DISPLAY_OBJECTS_SHADER_PROGRAM_H__

#include "../displayobject.h"
#include "../rendepipelinedefs.h"
#include "../math/memath.h"
#include "../types/mestring.h"

using namespace MorganaEngine::Base::Math;
using namespace MorganaEngine::Base::Types;

#define INVALID_UNIFORM_NAME		-1024
#define UNIFORM_NAME_NOT_PARSED		-1

namespace MorganaEngine
{
	namespace Base
	{
		namespace Devices
		{
			namespace Display
			{
				namespace DisplayObjects
				{
					class ShaderProgram : public DisplayObject
					{
						__declare_class(ShaderProgram, DisplayObject);
					public:
						ShaderProgram();
						virtual ~ShaderProgram();

						struct CreateParams_t
						{
							String shaderSource; // vertex, pixel, geometry, collapsed in same buffer

							CreateParams_t(const char* sh) : shaderSource(sh)
							{

							}
						};

						void			AddUniformEntry(const char* name, int type, int numElems);
						const bool		HasParameter(const char* name) const;

						void			SetFloat(const char* uniformName, const float& f);
						void			SetVector(const char* uniformName, const Vector2& v);
						void			SetVector(const char* uniformName, const Vector3& v);
						void			SetVector(const char* uniformName, const Vector4& v);
						void			SetColor(const char* uniformName, const Color255& c);
						void			SetColor(const char* uniformName, const Color& c);
						void			SetMatrix(const char* uniformName, const Matrix& v);
						void			SetInt(const char* uniformName, const int& i);

						void			SetFloats(const char* uniformName, const float* f, const int count);
						void			SetVectors(const char* uniformName, const Vector2* v, const int count);
						void			SetVectors(const char* uniformName, const Vector3* v, const int count);
						void			SetVectors(const char* uniformName, const Vector4* v, const int count);
						void			SetMatrices(const char* uniformName, const Matrix* v, const int count);
						void			SetInts(const char* uniformName, const int* i, const int count);

						void			SetFloat(const int& uniformIndex, const float& f);
						void			SetVector(const int& uniformIndex, const Vector2& v);
						void			SetVector(const int& uniformIndex, const Vector3& v);
						void			SetVector(const int& uniformIndex, const Vector4& v);
						void			SetColor(const int& uniformIndex, const Color255& c);
						void			SetColor(const int& uniformIndex, const Color& c);
						void			SetMatrix(const int& uniformIndex, const Matrix& v);
						void			SetInt(const int& uniformIndex, const int& i);

						void			SetFloats(const int& uniformIndex, const float* f, const int count);
						void			SetVectors(const int& uniformIndex, const Vector2* v, const int count);
						void			SetVectors(const int& uniformIndex, const Vector3* v, const int count);
						void			SetVectors(const int& uniformIndex, const Vector4* v, const int count);
						void			SetMatrices(const int& uniformIndex, const Matrix* v, const int count);
						void			SetInts(const int& uniformIndex, const int* i, const int count);

						int				GetParameterIndex(const char* name);

						struct UniformEntry_t
						{
							int						locationInt;
							StringHash				locationStr;
							int						dataType;
							int						dataSize;
							int						numElements;

							UniformEntry_t()
							{
								locationInt = UNIFORM_NAME_NOT_PARSED;
							}

							UniformEntry_t(const UniformEntry_t& other)
							{
								locationInt = other.locationInt;
								locationStr = other.locationStr;
								dataType = other.dataType;
								dataSize = other.dataSize;
								numElements = other.numElements;
							}

							UniformEntry_t(int location)
							{
								locationInt = location;
							}

							static UniformEntry_t Invalid;

							bool operator ==(UniformEntry_t const& b) { return locationInt == b.locationInt; }
							bool operator !=(UniformEntry_t const& b) { return locationInt != b.locationInt; }
						};

						void					UploadUniformData(UniformEntry_t* ue, const void* data);

						Array<UniformEntry_t>	uniformEntries;
						void					ClearUniformEntries();

						String					tags;

					protected:
						UniformEntry_t&			GetUniformByName(const char* name);
					private:
						const int	GetDataSize(UniformEntry_t& ue) const
						{
							switch (ue.dataType)
							{
							case UNIFORM_float: return ue.numElements * sizeof(float);
							case UNIFORM_Vector2: return ue.numElements * 2 * sizeof(float);
							case UNIFORM_Vector3: return ue.numElements * 3 * sizeof(float);
							case UNIFORM_Vector4: return ue.numElements * 4 * sizeof(float);
							case UNIFORM_Matrix: return ue.numElements * 16 * sizeof(float);
							case UNIFORM_int: return ue.numElements * sizeof(int);
							}
							return 0;
						}
					};
				}
			}
		}
	}
}

#endif
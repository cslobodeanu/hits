#ifndef __MORGANA_FMK_RESOURCES_MATERIALS_MATERIAL_PROXY_H__
#define __MORGANA_FMK_RESOURCES_MATERIALS_MATERIAL_PROXY_H__

#include "material.h"

namespace Morgana
{
	namespace Framework
	{
		namespace Resources
		{
			namespace Materials
			{
				class MaterialProxy : public Material
				{
					__declare_class(MaterialProxy, Material)
					void CheckVersion();
				public:
					MaterialProxy();
					MaterialProxy(const Material& other);
					virtual ~MaterialProxy();

					const Material*				source;

					const bool					HasParameter(const char* name);

					inline void					SetFloat(const char* name, const float& f)		{ SetParameter<float>(name, f); }
					inline void					SetVector(const char* name, const Vector2& v)	{ SetParameter<Vector2>(name, v); }
					inline void					SetVector(const char* name, const Vector3& v)	{ SetParameter<Vector3>(name, v); }
					inline void					SetVector(const char* name, const Vector4& v)	{ SetParameter<Vector4>(name, v); }
					inline void					SetColor(const char* name, const Color& c)		{ SetParameter<Color>(name, c); }
					inline void					SetMatrix(const char* name, const Matrix& v)	{ SetParameter<Matrix>(name, v); }
					inline void					SetInt(const char* name, const int& i)			{ SetParameter<int>(name, i); }

					inline void					SetFloats(const char* name, const float* f, const int count = 1)	{ SetParameters<float>(name, f, count); }
					inline void					SetVectors(const char* name, const Vector2* v, const int count = 1)	{ SetParameters<Vector2>(name, v, count); }
					inline void					SetVectors(const char* name, const Vector3* v, const int count = 1)	{ SetParameters<Vector3>(name, v, count); }
					inline void					SetVectors(const char* name, const Vector4* v, const int count = 1)	{ SetParameters<Vector4>(name, v, count); }
					inline void					SetMatrices(const char* name, const Matrix* v, const int count = 1)	{ SetParameters<Matrix>(name, v, count); }
					inline void					SetInts(const char* name, const int* i, const int count = 1)		{ SetParameters<int>(name, i, count); }

					inline void					SetFloat(const int& index, const float& f)		{ SetParameter<float>(index, f); }
					inline void					SetVector(const int& index, const Vector2& v)	{ SetParameter<Vector2>(index, v); }
					inline void					SetVector(const int& index, const Vector3& v)	{ SetParameter<Vector3>(index, v); }
					inline void					SetVector(const int& index, const Vector4& v)	{ SetParameter<Vector4>(index, v); }
					inline void					SetColor(const int& index, const Color& c)		{ SetParameter<Color>(index, c); }
					inline void					SetMatrix(const int& index, const Matrix& v)	{ SetParameter<Matrix>(index, v); }
					inline void					SetInt(const int& index, const int& i)			{ SetParameter<int>(index, i); }

					inline void					SetFloats(const int& index, const float* f, const int count = 1)	{ SetParameters<float>(index, f, count); }
					inline void					SetVectors(const int& index, const Vector2* v, const int count = 1)	{ SetParameters<Vector2>(index, v, count); }
					inline void					SetVectors(const int& index, const Vector3* v, const int count = 1)	{ SetParameters<Vector3>(index, v, count); }
					inline void					SetVectors(const int& index, const Vector4* v, const int count = 1)	{ SetParameters<Vector4>(index, v, count); }
					inline void					SetMatrices(const int& index, const Matrix* v, const int count = 1)	{ SetParameters<Matrix>(index, v, count); }
					inline void					SetInts(const int& index, const int* i, const int count = 1)		{ SetParameters<int>(index, i, count); }

					template<class T>
					T							GetParameterValue(const char* name);

					const int					GetParameterIndex(const char* name);

					inline void					SetTexture(Texture* tex, const int unit = 0) { textures[unit] = tex; }

					const void					UploadShaderParameters() const;

					const bool					Equals(const MaterialProxy* other) const;

				protected:

					struct ParamEntry_t
					{
						ShaderProgram::UniformEntry_t* uniform;
						char*		data;
						int			size;
						int			version;

						ParamEntry_t()
						{
							uniform = NULL;
							data = NULL;
							size = 0;
							version = 0;
						}

						~ParamEntry_t()
						{
							Clear();
						}

						const void Clear()
						{
							if (data != NULL)
								delete[] data;
						}

						const void SetData(const void* data, const int size)
						{
							if (size > this->size)
							{
								Clear();
								this->data = new char[size];
								memset(this->data, 0, size);
								this->size = size;
							}

							memcpy(this->data, data, size);
							version++;
						}
					};

					Array<ParamEntry_t>	parameters;
					ParamEntry_t*		GetParameter(const char* name);
					ParamEntry_t*		GetParameter(const int& index);

					int					shaderVersion;

					template<class T>
					void SetParameter(const char* name, const T& value);

					template<class T>
					void SetParameters(const char* name, const T* values, const int count = 1);

					template<class T>
					void SetParameter(const int& index, const T& value);

					template<class T>
					void SetParameters(const int& index, const T* values, const int count = 1);

					void SyncParameters();

					long	parametersChecksum;
					void	AddToChecksum(const char* ptr, const int size);
					void	RemoveFromChecksum(const char* ptr, const int size);
				};

				template<class T>
				void MaterialProxy::SetParameter(const char* name, const T& value)
				{
					ParamEntry_t* p = GetParameter(name);
					if (p != NULL)
					{
						RemoveFromChecksum(p->data, p->size);
						p->SetData(&value, sizeof(T));
						AddToChecksum(p->data, p->size);
						IncrementVersion();
					}
				}	

				template<class T>
				void MaterialProxy::SetParameters(const char* name, const T* values, const int count /*= 1*/)
				{
					ParamEntry_t* p = GetParameter(name);
					if (p != NULL)
					{
						RemoveFromChecksum(p->data, p->size);
						p->SetData(values, sizeof(T)* count);
						AddToChecksum(p->data, p->size);
						IncrementVersion();
					}
				}

				template<class T>
				void MaterialProxy::SetParameter(const int& index, const T& value)
				{
					ParamEntry_t* p = GetParameter(name);
					if (p != NULL)
					{
						RemoveFromChecksum(p->data, p->size);
						p->SetData(&value, sizeof(T));
						AddToChecksum(p->data, p->size);
						IncrementVersion();
					}
				}

				template<class T>
				void MaterialProxy::SetParameters(const int& index, const T* values, const int count /*= 1*/)
				{
					ParamEntry_t* p = GetParameter(name);
					if (p != NULL)
					{
						RemoveFromChecksum(p->data, p->size);
						p->SetData(values, sizeof(T)* count);
						AddToChecksum(p->data, p->size);
						IncrementVersion();
					}
				}

				template<class T>
				T MaterialProxy::GetParameterValue(const char* name)
				{
					T ret;
					ParamEntry_t* p = GetParameter(name);
					if (p != NULL)
						memcpy(&ret, p->data, p->size);

					return ret;
				}
			}
		}
	}
}

#endif
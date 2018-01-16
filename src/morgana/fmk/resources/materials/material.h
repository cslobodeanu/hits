#ifndef __MORGANA_FMK_RESOURCES_MATERIALS_MATERIAL_H__
#define __MORGANA_FMK_RESOURCES_MATERIALS_MATERIAL_H__

#include "reflection.h"
#include "medisplay.h"
#include "../resourcefile.h"
#include "meparsers.h"
#include "../shaders/shaderproxy.h"

namespace MorganaEngine
{
	namespace Framework
	{
		namespace Renderers
		{
			class Renderer;
		}

		namespace Resources
		{
			using namespace Shaders;
			namespace Materials
			{
				class Material : public ResourceFile
				{
					__declare_class(Material, ResourceFile)
				public:
					Material();
					virtual ~Material();

					Material(const Material& other)
					{
						Copy(other);
					}

					void Copy(const Material& other)
					{
						depthTest = other.depthTest;
						depthWrite = other.depthWrite;
						blend = other.blend;
						cull = other.cull;
						shininess = other.shininess;
						specular = other.specular;
						emissive = other.emissive;
						textures.Copy(other.textures);
						shader = other.shader;
						name = other.name;

						currentParameterSet = ParameterSet::User;

						defaultParameters.Copy(other.defaultParameters);
						userParameters.Copy(other.userParameters);

						hash = other.hash;
					}

					const bool Equals(const Material* other) const
					{
						float silent = true;

						if (other == NULL) return false;

						if (hash != other->hash)
							return false;

						if (silent)
						{
							if (memcmp(&depthTest, &other->depthTest, 3 * sizeof(depthTest) + 12/*shiniess, specular, emissive*/) != 0) return false;
						}
						else
						{
							if (depthTest != other->depthTest)
							{
								if (!silent)
									DEBUG_OUT("Different depthStencil");
								return false;
							}

							if (blend != other->blend)
							{
								if (!silent)
									DEBUG_OUT("Different blend");
								return false;
							}

							if (cull != other->cull)
							{
								if (!silent)
									DEBUG_OUT("Different cull");
								return false;
							}
						}

						if (shader != other->shader)
						{
							if (!silent)
								DEBUG_OUT("Different shader");
							return false;
						}


						if (textures.Length() != other->textures.Length())
							return false;

						if (memcmp(textures.ptr(), other->textures.ptr(), textures.bufsize()) != 0)
						{
							if (!silent)
								DEBUG_OUT("Different textures");
							return false;
						}

						return true;
					}

					bool						depthWrite;
					const DepthState*			depthTest;
					const StencilState*			stencilTest;
					const BlendState*			blend;
					const RasterizerState*		cull;
					float						shininess;
					Color						specular;
					Color						emissive;

					ShaderProgram*				shader;

					Array<Texture*>				textures;

					int							renderQueueIndex;

					Renderers::Renderer*		caller;

					typedef enum
					{
						Background = -1,
						Opaque = 0,
						AlphaTested,
						Transparent,
						Foreground
					} RenderQueue;

					const RenderQueue			GetBaseRenderQueue() const;

					Texture*					SetTexture(Texture* t, const int stage = 0);

					const void					ChangeShader(ShaderProgram* s);


					const bool					HasParameter(const char* name);

					inline void					SetFloat(const char* name, const float& f)		{ SetParameter<float>(name, f); }
					inline void					SetVector(const char* name, const Vector2& v)	{ SetParameter<Vector2>(name, v); }
					inline void					SetVector(const char* name, const Vector3& v)	{ SetParameter<Vector3>(name, v); }
					inline void					SetVector(const char* name, const Vector4& v)	{ SetParameter<Vector4>(name, v); }
					inline void					SetColor(const char* name, const Color255& c)	{ SetParameter<Vector4>(name, c); }
					inline void					SetColor(const char* name, const Color& c)		{ SetParameter<Vector4>(name, c); }
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
					inline void					SetColor(const int& index, const Color255& c)	{ SetParameter<Vector4>(index, c); }
					inline void					SetColor(const int& index, const Color& c)		{ SetParameter<Vector4>(index, c); }
					inline void					SetMatrix(const int& index, const Matrix& v)	{ SetParameter<Matrix>(index, v); }
					inline void					SetInt(const int& index, const int& i)			{ SetParameter<int>(index, i); }

					inline void					SetFloats(const int& index, const float* f, const int count = 1)	{ SetParameters<float>(index, f, count); }
					inline void					SetVectors(const int& index, const Vector2* v, const int count = 1)	{ SetParameters<Vector2>(index, v, count); }
					inline void					SetVectors(const int& index, const Vector3* v, const int count = 1)	{ SetParameters<Vector3>(index, v, count); }
					inline void					SetVectors(const int& index, const Vector4* v, const int count = 1)	{ SetParameters<Vector4>(index, v, count); }
					inline void					SetMatrices(const int& index, const Matrix* v, const int count = 1)	{ SetParameters<Matrix>(index, v, count); }
					inline void					SetInts(const int& index, const int* i, const int count = 1)		{ SetParameters<int>(index, i, count); }

					enum ParameterSet
					{
						Default,
						User
					};

					void						UploadParameters(ParameterSet which);

					static const Material*		DefaultCanvas;
					static const Material*		DefaultMaskedCanvas;
					static const Material*		DefaultFontCanvas;

					static void					LoadDefaults();

				protected:
					virtual void				ImportFromStreams(const StreamList&  streams, const char* args = NULL);
					virtual const bool			SaveToStream(Stream* s);
					void						FillValuesFromParsedBlock(tinyxml2::XMLElement* elem);

				private:
					template<class T>
					void SetParameter(const char* name, const T& value)
					{
						int index = shader->GetParameterIndex(name);
						if (index >= 0)
							SetParameter(index, value);
					}

					template<class T>
					void SetParameters(const char* name, const T* values, const int count = 1)
					{
						int index = shader->GetParameterIndex(name);
						if (index >= 0)
							SetParameters(index, values, count);
					}

					template<class T>
					void SetParameter(const int& index, const T& value)
					{
						Array<ParameterData_t>& params = GetParameterSet(currentParameterSet);
						if (index < 0 || index >= params.Length()) return;
						ParameterData_t& pd = params[index];
						pd.user = currentParameterSet == ParameterSet::User;
						pd.UploadData<T>(&value);
					}

					template<class T>
					void SetParameters(const int& index, const T* values, const int count = 1)
					{
						Array<ParameterData_t>& params = GetParameterSet(currentParameterSet);
						if (index < 0 || index >= params.Length()) return;
						ParameterData_t& pd = params[index];
						pd.user = currentParameterSet == ParameterSet::User;
						pd.UploadData<T>(values, count);
					}

					struct  ParameterData_t
					{
					public:
						void*									data;
						int										dataSize;
						ShaderProgram::UniformEntry_t*			uniform;
						bool									user;

						ParameterData_t()
						{
							data = null;
							dataSize = 0;
							user = false;
						}

						template <class T>
						inline const void UploadData(const T* values, const int numElems = 1)
						{
							int newSz = numElems * sizeof(T);
							if (dataSize != newSz || data == null)
							{
								data = new char[newSz];
								dataSize = newSz;
							}
							if (values != null)
								memcpy(data, values, newSz);
							else
								data = null;
						}
					};

					Array<ParameterData_t> defaultParameters;
					Array<ParameterData_t> userParameters;

					ParameterSet currentParameterSet = ParameterSet::Default;

					inline Array<ParameterData_t>& GetParameterSet(const ParameterSet which)
					{
						return which == ParameterSet::Default ? defaultParameters : userParameters;
					}

					void BuildParameterInfo();

					unsigned long hash;
					void			ComputeHash();
				};
			}
		}
	}
}

#define PROJVIEW		"pvwMat"

#endif
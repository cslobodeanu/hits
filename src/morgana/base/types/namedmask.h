#ifndef __MORGANA_BASE_TYPES_NAMED_MASK_H__
#define __MORGANA_BASE_TYPES_NAMED_MASK_H__

#include "mearray.h"
#include "mestringhash.h"
#include "bits.h"
namespace MorganaEngine
{
	namespace Base
	{
		namespace Types
		{
			class NamedMask
			{
				bool singleBit;
				Bits<256> mask;
			public:

				NamedMask(const bool singleBit = false)
				{
					this->singleBit = singleBit;
				}

				inline const void SetSingleBit(const bool b)
				{
					singleBit = b;
				}

				inline const void SetLayer(const int l)
				{
					if (singleBit)
						mask.Clear();
					mask.SetBit(l);
				}

				inline const void SetLayer(const char* name)
				{
					int index = LayerFromName(name);
					if (index < 0)
						index = AddLayer(name);
					SetLayer(index);
				}

				inline const void DeleteLayer(const int l)
				{
					mask.DeleteBit(l);
				}

				inline const void DeleteLayer(const char* name)
				{
					DeleteLayer(LayerFromName(name));
				}

				inline const bool Contains(const int l) const
				{
					return this->mask.IsBitSet(l);
				}

				inline const bool Contains(const char* name) const
				{
					const int layer = LayerFromName(name);
					return Contains(layer);
				}

				inline const void MarkAll()
				{
					mask.SetAll();
				}

				inline const void MarkNone()
				{
					mask.Clear();
				}

				static const int LayerFromName(const char* name)
				{
					const int index = GetLayer(name);
					return (index >= 0) ? index : -1;
				}

				static const char* NameForLayer(const int index)
				{
					if (index < layers.Length())
						return layers[index].c_str();
					return NULL;
				}

				static const int AddLayer(const char* name)
				{
					const int index = GetLayer(name);
					if (index >= 0) return index;

					layers.Add(StringHash(name));

					return layers.Length() - 1;
				}

				static const int Count()
				{
					return layers.Length();
				}

			private:

				static Array<StringHash> layers;

				static const int GetLayer(const char* name)
				{
					const int sh = String::ComputeHashCode(name);
					for (int i = 0; i < layers.Length(); i++)
					{
						if (layers[i].Equals(name, sh))
						{
							return i;
						}
					}

					return -1;
				}
			};
		}
	}
}

#endif
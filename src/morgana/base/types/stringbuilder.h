#ifndef __MORGANA_BASE_TYPES_STRING_BUILDER_H__
#define __MORGANA_BASE_TYPES_STRING_BUILDER_H__

#include "mestring.h"
#include "mearray.h"

namespace MorganaEngine
{
	namespace Base
	{
		namespace Types
		{
			class FastStringLock;
			class FastString : public String
			{
				int myBufSize;
				char* myBuf;
				bool locked;
			protected:
				virtual void OnInitialized()
				{
					userData = -1;
					buf = null;
					myBuf = null;
					myBufSize = 0;
					locked = false;
				}

				virtual char* allocBuf(int size)
				{
					if (size > myBufSize)
					{
						if (myBuf != null)
							delete[] myBuf;

						myBuf = new char[size];
						myBufSize = size;
					}

					return myBuf;
				}

				virtual void freeBuf()
				{
					myBufSize = 0;
					buf = null;
				}

				void Lock()
				{
					ASSERT(locked == false);
					locked = true;
				}

				void Unlock()
				{
					ASSERT(locked);
					locked = false;
				}

			public:
				friend class FastStringLock;

				int userData;

				FastString() : String()
				{
				}

				FastString(const char* buf) : String(buf)
				{
				}

				void InitWithSize(const int size)
				{
					buf = allocBuf(size);
				}

				virtual ~FastString()
				{
					if (myBuf != null)
						delete[] myBuf;
					buf = null;
				}
			};

			class FastStringLock
			{
				FastString* src;
			public:
				FastStringLock(FastString& fs)
				{
					src = &fs;
					src->Lock();
				}

				~FastStringLock()
				{
					src->Unlock();
				}
			};

			class StringBuilder
			{
			private:
				static Array<FastString> allocatedStrings;
				static Array<int> dirty;

				static FastString initString;

				static void AddNewString()
				{
					const int lastIndex = allocatedStrings.Length();
					allocatedStrings.Add(initString);
					allocatedStrings.Last().userData = lastIndex;
					dirty.Add(lastIndex);
				}

			public:

				static FastString& Alloc()
				{
					if (dirty.IsEmpty())
					{
						AddNewString();
					}

					const int index = dirty.Pop();
					FastString& s = allocatedStrings[index];
					s[0] = 0;
					return s;
				}

				static FastString& Alloc(const char* src)
				{
					FastString& tmp = Alloc();
					tmp.InitWithBuffer(src);
					return tmp;
				}

				static FastString& Alloc(const String& src)
				{
					return Alloc(src.c_str());
				}

				static void Free(FastString& s)
				{
					ASSERT(s.userData >= 0 && s.userData < allocatedStrings.Length());
					if(s.userData >= 0)
						dirty.Add(s.userData);
				}
			};
		}
	}
}

#endif
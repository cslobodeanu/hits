#ifndef __MORGANA_BASE_TYPES_MESTRING_H__
#define __MORGANA_BASE_TYPES_MESTRING_H__

#include "mearray.h"

namespace MorganaEngine
{
	namespace Base
	{
		namespace Types
		{
			class String
			{
			protected:
				char* buf;
				virtual char* allocBuf(const int sz);
				virtual void freeBuf();
				void copy(const char* str);
				void wcopy(const wchar_t* str);
				virtual void OnStringChanged(){}
				virtual void OnInitialized() {}
			public:

				String();
				String(const char* s);
				String(const char* s, const int len);
				String(const String& s);
				String(const int& i);
				String(const float& f);
				String(const double& f);
				String(const bool& b);
				virtual ~String();

				const int			Length() const;
				const char*			c_str() const;
				operator			const char* () const;
				char&				operator[](int i) const;
				String				operator+ (const String a);
				String				operator+ (const char* a);
				String				operator+ (const int &val);
				bool				operator != (const String &s) const;
				bool				operator == (const String &s) const;
				bool				operator != (const char*const s) const;
				bool				operator == (const char*const s) const;

				const String&		operator = (const char* str);
				String&				operator = (String str);
				String&				operator += (String str);
				String&				operator += (int val);
				String&				operator += (float val);
				String&				operator += (char val);
				String				Substring(int i);
				String				Substring(int start, int end);

				static String		Substring(const char* src, int start, int end);

				char				CharAt(int i);
				void				ToUpper();
				void				ToLower();

				String				ToUpper() const;
				String				ToLower() const;

				void				Printf(const char* s,...);
				static String		FormatF(const char*& fmtStr);
				static String		Format(const char* s, ...);
				void				Printf(const wchar_t* s,...);
				bool				Equals(const char* s) const;
				bool				EqualsNC(const char* s) const;
				bool				Equals(const String& s) const;
				int					Find(char c);
				int					Find(const char* str) const;
				bool				Contains(char c);
				bool				Contains(const char* str) const;
				int					FindLast(char c);
				int					RemoveCharacter(char c);
				int					ReplaceCharacter(char c,char newc);
				void				ReplaceString(const char* str_to_replace, const char* new_str);
				int					ToInt() const;
				float				ToFloat() const;
				void				ToFloats(float& f1, float& f2) const;
				void				ToFloats(float& f1, float& f2, float& f3) const;
				void				ToFloats(float& f1, float& f2, float& f3, float& f4) const;
				bool				ToBool() const;
				void				Clear();
				void				Delete(const int& count);
				void				Delete(unsigned int start, unsigned int count);
				const bool			IsEmpty() const;

				char				First();
				char				Last();

				void				Append(const char* str);

				void				Split(const char* separators, Array<String>& out) const;
				static void			Split(const char* src, const char* separators, Array<String>& out);
				static Array<String> Split(const char* src, const char* separators);

				const int			ComputeHashCode();
				static const int	ComputeHashCode(const char* str);

				const bool			TestWildcard(const char* pattern) const;
				static bool			TestWildcard(const char* name, const char* pattern);

				static bool			IsNumber(const char c);
				const bool			IsNumber() const;

				static String		GetPrefix(const char* str, const char* separator = ":");
				static bool			HasPrefix(const char* str, const char* separator = ":");
				void				RemovePrefix(const char* separator = ":");

				bool				StartsWith(const char* str);

				static char			ToUpperCase(const char ch);
				static bool			IsUpperCase(const char ch);

				void				Trim(const char* chars=" \t"); // remove chars from beginning and ending

				void				InitWithBuffer(const char* src);
			};
		}
	}
}

#endif

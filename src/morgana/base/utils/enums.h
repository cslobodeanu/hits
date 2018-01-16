#ifndef __MORGANA_BASE_UTILS_ENUMS_H_INCLUDED__
#define __MORGANA_BASE_UTILS_ENUMS_H_INCLUDED__

#define MAKE_STRING_1(str) #str
#define MAKE_STRING_2(str,...) #str,MAKE_STRING_1(__VA_ARGS__)
#define MAKE_STRING_3(str,...) #str,MAKE_STRING_2(__VA_ARGS__)
#define MAKE_STRING_4(str,...) #str,MAKE_STRING_3(__VA_ARGS__)
#define MAKE_STRING_5(str,...) #str,MAKE_STRING_4(__VA_ARGS__)
#define MAKE_STRING_6(str,...) #str,MAKE_STRING_5(__VA_ARGS__)
#define MAKE_STRING_7(str,...) #str,MAKE_STRING_6(__VA_ARGS__)
#define MAKE_STRING_8(str,...) #str,MAKE_STRING_7(__VA_ARGS__)

#define PRIMITIVE_CAT(a, b) a##b
#define MAKE_STRING(N, ...) PRIMITIVE_CAT(MAKE_STRING_, N) (__VA_ARGS__)


#define VA_ARGS_NUM_PRIV(P1, P2, P3, P4, P5, P6, P7, P8, Pn, ...) Pn
#define VA_ARGS_NUM(...) VA_ARGS_NUM_PRIV(-1, ##__VA_ARGS__, 7, 6, 5, 4, 3, 2, 1, 0)

#define make_enum(NAME, ...) \
	struct NAME {													\
		enum __##NAME { __VA_ARGS__ };								\
		typedef __##NAME Values;									\
		static String ToString(const int et) \
		{	\
			static const char* NAME##Str = MAKE_STRING(VA_ARGS_NUM(__VA_ARGS__), __VA_ARGS__);	\
			int firstComma = -1; \
			int secondComma = -1; \
			char* tmp = (char*)NAME##Str; \
			int index = 0; \
			int commas = 0; \
			while (*tmp) \
			{ \
				if(*tmp == ',' || index == 0) \
				{ \
					if (firstComma >= 0) { secondComma = index; break; }\
					if(firstComma < 0 && commas == et) firstComma = index;\
					commas++; \
				} \
				index++; \
				tmp++; \
			} \
			return  String::Substring(NAME##Str, et == 0 ? 0 : firstComma + 2, secondComma);	\
		}	\
		static int FromString(const char* str, bool caseSensitive = true) \
		{				\
			static const char* NAME##Str = MAKE_STRING(VA_ARGS_NUM(__VA_ARGS__), __VA_ARGS__);	\
			int prevComma = 0; \
			char* tmp = (char*)NAME##Str; \
			char* tmp2 = tmp; \
			int index = 0; \
			int commas = 0; \
			const int strLen = strlen(str); \
			while (*tmp) \
			{ \
				if(*tmp == ',') \
				{ \
					const int subStringLen = index - prevComma; \
					if(subStringLen == strLen) \
					{ \
						if(caseSensitive == false) \
						{ \
							String s1(str); s1.ToLower(); \
							String s2(tmp2 + prevComma); s2[strLen] = 0; s2.ToLower(); \
							if(s1.Equals(s2)) \
								return commas; \
						} \
						else \
						if (memcmp(str, tmp2 + prevComma, strLen) == 0) \
						{ \
							return commas; \
						} \
					} \
					prevComma = index + 2; \
					commas++; \
				} \
				index++; \
				tmp++; \
			} \
			return  -1; \
		}	\
    };

#endif
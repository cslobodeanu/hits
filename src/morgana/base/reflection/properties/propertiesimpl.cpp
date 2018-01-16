#include "propertiesimpl.h"
#include "memath.h"

using namespace MorganaEngine::Base::Reflection;

String	_property<int>::ToString() const { return String(value); }
void	_property<int>::FromString(const char* s) { value = String(s).ToInt(); InvalidateValue(); }

String	_property<float>::ToString() const { return String(value); }
void	_property<float>::FromString(const char* s) { value = String(s).ToFloat(); InvalidateValue(); }

String	_property<bool>::ToString() const { return String(value); }
void	_property<bool>::FromString(const char* s) { value = String(s).ToBool(); InvalidateValue(); }

String	_property<String>::ToString() const { return value; }
void	_property<String>::FromString(const char* s) { value = s; InvalidateValue(); }

String	_property<Quaternion>::ToString() const { return String::Format("%f, %f, %f, %f", value.x, value.y, value.z, value.w); }
void	_property<Quaternion>::FromString(const char* s) { sscanf_s(s, "%f, %f, %f, %f", &value.x, &value.y, &value.z, &value.w); InvalidateValue(); }

String	_property<Rectf>::ToString() const { return String::Format("%f, %f, %f, %f", value.x, value.y, value.width, value.height); }
void	_property<Rectf>::FromString(const char* s) { sscanf_s(s, "%f, %f, %f, %f", &value.x, &value.y, &value.width, &value.height); InvalidateValue(); }

String	_property<Vector2>::ToString() const { return String::Format("%f, %f", value.x, value.y); }
void	_property<Vector2>::FromString(const char* s) { sscanf_s(s, "%f, %f", &value.x, &value.y); InvalidateValue(); }

String	_property<Vector3>::ToString() const { return String::Format("%f, %f, %f", value.x, value.y, value.z); }
void	_property<Vector3>::FromString(const char* s) { sscanf_s(s, "%f, %f, %f", &value.x, &value.y, &value.z); InvalidateValue(); }

String	_property<Color>::ToString() const { return String::Format("%f, %f, %f, %f", value.r, value.g, value.b, value.a); }
void	_property<Color>::FromString(const char* s) { sscanf_s(s, "%f, %f, %f, %f", &value.r, &value.g, &value.b, &value.a); InvalidateValue(); }
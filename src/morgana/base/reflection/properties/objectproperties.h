#ifndef __MORGANA_BASE_REFLECTION_PROPERTIES_PROPERTIES_FOR_OBJECT_H__
#define __MORGANA_BASE_REFLECTION_PROPERTIES_PROPERTIES_FOR_OBJECT_H__

#include "metypes.h"
#include "properties.h"

namespace MorganaEngine
{
	namespace Base
	{
		namespace Reflection
		{
			struct object_property_data
			{
				StringHash			name;			
				String				displayName;	
				unsigned long		locationOffset;	
				unsigned long		flags;

				bool				isRanged, isReadonly, isHiddenFromView;

				float				rangeMin, rangeMax;

				Array<String>		enumValuesNames;
				Array<int>			enumValues;

				object_property_data()
				{
					name = "[NOT SET]";
					displayName = "[NOT SET]";
					locationOffset = 0;
					flags = 0;

					isRanged = false;
					isReadonly = false;
					isHiddenFromView = false;

					rangeMin = rangeMax = 0.0f;
				}

				object_property_data(const object_property_data& other)
				{
					name = other.name;
					displayName = other.displayName;
					locationOffset = other.locationOffset;
					flags = other.flags;

					isRanged = other.isRanged;
					isReadonly = other.isReadonly;
					isHiddenFromView = other.isHiddenFromView;

					rangeMin = other.rangeMin;
					rangeMax = other.rangeMax;

					enumValuesNames.Copy(other.enumValuesNames);
					enumValues.Copy(other.enumValues);
					{

					};
				}

				object_property_data& setReadOnly(bool set = true)
				{
					isReadonly = set;
					return *this;
				}

				object_property_data& setHiddenFromView(bool set = true)
				{
					isHiddenFromView = set;
					return *this;
				}

				object_property_data& setRange(const float& min, const float& max)
				{
					isRanged = true;
					rangeMin = min;
					rangeMax = max;
					return *this;
				}

				object_property_data& addEnumValue(const char* name, const int& value)
				{
					enumValuesNames.Add(name);
					enumValues.Add(value);
					return *this;
				}

				object_property_data& removeEnumValue(const char* name)
				{
					const int index = enumValuesNames.Find(name);
					if (index >= 0)
					{
						enumValuesNames.RemoveNo(index);
						enumValues.RemoveNo(index);
					}
					return *this;
				}
			};

			class _object_properties
			{
			public:

				static _propertyDef*			find(MEObject* owner, const char* name);
				static long						getLocationOffset(MEObject* owner, _propertyDef* p);
				static int						getIndex(MEObject* owner, _propertyDef* p);
				static bool						locationExists(MEObject* owner, _propertyDef* p);
				static void						copy(MEObject* owner, MEObject* mrs);
				static object_property_data&	add(MEObject* owner, const char* dispname, const char* varname, _propertyDef* var);
				static object_property_data&	add1(MEObject* owner, const char* varname, _propertyDef* var);

				static _propertyDef*			propertyAt(MEObject* owner, const int& index);
				static object_property_data&	getData(MEObject* owner, _propertyDef* p);

			};
		}
	}
}

#define __add_property1(var)			_object_properties::add1(this, #var, &(var))
#define __add_property(name, var)		_object_properties::add(this, name, #var, &(var))
#define __add_property_ro(name, var)      \
    _object_properties::add(name, #var, &(var)).setReadOnly();

#define __add_property_ro1(var)      \
    _object_properties::add(#var, #var, &(var)).setReadOnly();

#define __add_property_hid(name, var)      \
    _object_properties::add(name, #var, &(var)).setHiddenFromView();

#define __add_enum_value(var, enumValue) \
	_object_properties::getData(&(var)).addEnumValue(#enumValue, enumValue);

#define __add_enum_value2(var, enumValue1, enumValue2) \
	_object_properties::getData(&(var)).addEnumValue(#enumValue1, enumValue1).addEnumValue(#enumValue2, enumValue2);

#define __add_enum_value3(var, enumValue1, enumValue2, enumValue3) \
	_object_properties::getData(&(var)).addEnumValue(#enumValue1, enumValue1).addEnumValue(#enumValue2, enumValue2).addEnumValue(#enumValue3, enumValue3);

#define __add_enum_value4(var, enumValue1, enumValue2, enumValue3, enumValue4) \
	_object_properties::getData(&(var)).addEnumValue(#enumValue1, enumValue1).addEnumValue(#enumValue2, enumValue2).addEnumValue(#enumValue3, enumValue3).addEnumValue(#enumValue4, enumValue4);

#endif
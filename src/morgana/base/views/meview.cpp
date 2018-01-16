#include "meview.h"
#include "meio.h"

using namespace MorganaEngine::Base::Views;

__implement_class_abstract(MEView, MEObject);

MEView* MEView::main = NULL;

MEView::MEView() : MEObject()
{
	left = 0;
	top = 0;
	width = 640;
	height = 480;
	isMaximized = false;
	caption = "MEView";
	keyIdentifier = "MEView";
	DoAddProperties();

	main = this;
}

MEView::~MEView()
{

}

void MEView::DoAddProperties()
{
	super::DoAddProperties();

	__add_property1(left);
	__add_property1(top);
	__add_property1(width);
	__add_property1(height);
	__add_property1(isMaximized);
	__add_property1(caption);
}

void MEView::OnPropertySet( _propertyDef* p )
{
	super::OnPropertySet(p);

	if(/*p == &m_iLeft || p == &m_iTop || */p == &width || p == &height || p == &isMaximized)
	{
		ResizeReposition();
	}
}

void MEView::Close()
{
	Show(false);
}

void MEView::Show( const bool show /*= true*/ )
{
	if(show)
	{
		ReadFromRegistry();
	}
	else
	{
		WriteToRegistry();
	}
}


String MEView::GetRegistryKey()
{
	String str;
	str.Printf("reg://%s_view_info", keyIdentifier.ToString().c_str());
	return str;
}

void MEView::ReadFromRegistry()
{
	String key = GetRegistryKey();
	Stream* s = singleton<MFiles>()->GetStream(key);
	if(s && s->Open(Stream::READ))
	{
		if(s->Size() != 0)
		{
			left = s->ReadInt32();
			top = s->ReadInt32();
			width = s->ReadInt32();
			height = s->ReadInt32();
			isMaximized = s->ReadUInt8() != 0;
		}
		s->Close();

		ResizeReposition();
	}
}

void MEView::WriteToRegistry()
{
	String key = GetRegistryKey();
	Stream* s = singleton<MFiles>()->GetStream(key);
	if(s && !s->IsOpened() && s->Open(Stream::CREATE))
	{
		s->WriteInt32(left);
		s->WriteInt32(top);
		s->WriteInt32(width);
		s->WriteInt32(height);
		s->WriteUInt8(isMaximized ? 1 : 0);
		s->Close();
	}
}

void MEView::ResizeReposition()
{

}

unsigned long MEView::GetHandle()
{
	return handle;
}
#ifndef SOLUNAR_EDITOR_WINDOW_H
#define SOLUNAR_EDITOR_WINDOW_H

namespace solunar 
{
	class World;
}

namespace solunar
{
	class IEditorWindow 
	{
	public:
		virtual ~IEditorWindow(void) {}

		virtual void Init(void) = 0;
		virtual void Draw(World* pWorld) = 0;
		virtual const char* GetName(void) const = 0;
		virtual bool IsShow(void) const = 0;
		virtual void SetShow(bool status) = 0;
	};
}

#endif
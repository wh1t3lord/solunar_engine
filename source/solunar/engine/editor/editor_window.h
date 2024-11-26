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

		virtual void Draw(World* pWorld) = 0;
	};
}
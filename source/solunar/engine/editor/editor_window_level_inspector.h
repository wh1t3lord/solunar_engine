#include "engine/editor/editor_window.h"

namespace solunar
{
	class EditorWindow_LevelInspector : public IEditorWindow
	{
	public:
		EditorWindow_LevelInspector();
		~EditorWindow_LevelInspector();

		void Draw(World* pWorld) override;
		const char* GetName(void) const override;
	};
}
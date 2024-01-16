#ifndef GRAPHICS_GRAPHICSOBJECT_H
#define GRAPHICS_GRAPHICSOBJECT_H

namespace engine
{

enum GraphicsObjectType
{
	GOT_None,
	GOT_Texture,
	GOT_Material,
	GOT_StaticMesh,
	GOT_SkyMesh,
	GOT_Decal
};

//! Graphics object, wrapper on any gpu resource.
class GraphicsObject : public SerializableObject
{
	ImplementObject(GraphicsObject, SerializableObject);
public:
	GraphicsObject();
	virtual ~GraphicsObject();

	//! Create hardware resource
	virtual void createHw() = 0;
	
	//! Release hardware resource
	virtual void releaseHw() = 0;

protected:
	GraphicsObjectType m_type;
};

}

#endif // !GRAPHICS_GRAPHICSOBJECT_H

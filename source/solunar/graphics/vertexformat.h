#ifndef VERTEXFORMAT_H
#define VERTEXFORMAT_H

#define MAX_VERTEX_ATTRIBUTE 15

namespace engine
{

struct VertexAttribute
{
	const char* m_name;
	size_t m_size;
	size_t m_offset;
};

class VertexFormat
{
public:
	VertexFormat();
	~VertexFormat();

	void addPosition();
	void addNormal();
	void addTexcoord();

	void addPosition_Float2();
	void addColor_Float4();
	void addColor_DWORD();

	VertexAttribute* begin() { return m_attribues; }
	VertexAttribute* end() { return m_attribues + m_attributeCount; }

	size_t count() { return m_attributeCount; }
	size_t size() { return m_attributeSize; }

private:
	VertexAttribute m_attribues[MAX_VERTEX_ATTRIBUTE];
	size_t m_attributeCount;
	size_t m_attributeSize;
};
	
}

#endif // !VERTEXFORMAT_H

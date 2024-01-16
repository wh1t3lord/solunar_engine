#include "graphicspch.h"
#include "graphics/vertexformat.h"

namespace engine
{

VertexFormat::VertexFormat()
{
	memset(m_attribues, 0, sizeof(m_attribues));
	m_attributeCount = 0;
	m_attributeSize = 0;
}

VertexFormat::~VertexFormat()
{
	m_attributeSize = 0;
	m_attributeCount = 0;
	memset(m_attribues, 0, sizeof(m_attribues));
}

void VertexFormat::addPosition()
{
	VertexAttribute attribute;
	attribute.m_name = "position";
	attribute.m_size = 3;
	attribute.m_offset = m_attributeCount;

	m_attribues[m_attributeCount++] = attribute;
	m_attributeSize += attribute.m_size;
}

void VertexFormat::addNormal()
{
	VertexAttribute attribute;
	attribute.m_name = "normal";
	attribute.m_size = 3;
	attribute.m_offset = m_attributeCount;

	m_attribues[m_attributeCount++] = attribute;
	m_attributeSize += attribute.m_size;
}

void VertexFormat::addTexcoord()
{
	VertexAttribute attribute;
	attribute.m_name = "texcoord";
	attribute.m_size = 2;
	attribute.m_offset = m_attributeCount;

	m_attribues[m_attributeCount++] = attribute;
	m_attributeSize += attribute.m_size;
}

void VertexFormat::addPosition_Float2()
{
	VertexAttribute attribute;
	attribute.m_name = "texcoord";
	attribute.m_size = 2;
	attribute.m_offset = m_attributeCount;

	m_attribues[m_attributeCount++] = attribute;
	m_attributeSize += attribute.m_size;
}

void VertexFormat::addColor_Float4()
{
	VertexAttribute attribute;
	attribute.m_name = "color";
	attribute.m_size = 4;
	attribute.m_offset = m_attributeCount;

	m_attribues[m_attributeCount++] = attribute;
	m_attributeSize += attribute.m_size;
}

void VertexFormat::addColor_DWORD()
{
	VertexAttribute attribute;
	attribute.m_name = "color";
	attribute.m_size = 4;
	attribute.m_offset = m_attributeCount;

	m_attribues[m_attributeCount++] = attribute;
	m_attributeSize += attribute.m_size;
}

	
}
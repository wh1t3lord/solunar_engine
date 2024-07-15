#include "graphicspch.h"

#include "core/file/datastream.h"

// Graphics core
#include "graphics/core/device.h"
#include "graphics/core/texture.h"
#include "graphics/core/buffer.h"

#include "graphics/material.h"
#include "graphics/animatedmodel.h"

#define CGLTF_IMPLEMENTATION
#include <cgltf.h>

namespace engine
{

// UnpackValue
template <typename T>
bool gltfUnpackValues(const cgltf_primitive& primitive, int64_t index, int64_t vtxCount, std::vector<T>& data)
{
	if (index == -1) {
		Core::msg("gltfUnpackValue: Invalid index %i", index);
		return false;
	}

	const auto& Attributes = primitive.attributes[index];
	const size_t NumComponents = cgltf_num_components(Attributes.data->type);
	if (NumComponents != (sizeof(T) / sizeof(float))) {
		Core::msg("gltfUnpackValue: Mesh doesn't contain invalid attribute %s information.", Attributes.name);
		return false;
	}

	data.resize(vtxCount);
	cgltf_accessor_unpack_floats(Attributes.data, (float*)data.data(), vtxCount * NumComponents);
	return true;
}

// Object registering
void AnimatedModel::registerObject()
{
	TypeManager::getInstance()->registerObject<AnimatedModel>();
}

AnimatedModel::AnimatedModel()
{
	m_boneCount = 0;
}

AnimatedModel::~AnimatedModel()
{
	releaseHw();
}

void AnimatedModel::load(const std::shared_ptr<DataStream>& stream)
{
	load_GLTF(stream);
}

void AnimatedModel::load_GLTF(const std::shared_ptr<DataStream>& stream)
{
	stream->seek(Seek_End, 0);
	size_t filesize = stream->tell();
	stream->seek(Seek_Begin, 0);

	uint8_t* fileData = (uint8_t*)malloc(filesize);
	stream->read(fileData, filesize);

	cgltf_data* data = NULL;
	cgltf_options options = {};
	cgltf_result result = cgltf_parse(&options, fileData, filesize, &data);

	if (result != cgltf_result_success) {
		Core::msg("AnimatedModel: Failed to parse GLTF asset.");
		return;
	}

	result = cgltf_load_buffers(&options, data, nullptr);
	if (result != cgltf_result_success) {
		cgltf_free(data);
		Core::msg("AnimatedModel: Failed to load buffers for GLTF asset .");
		return;
	}

	for (int i = 0; i < data->meshes_count; i++) {
		const cgltf_mesh& mesh = data->meshes[i];
		
		// #TODO: more than one primitive !!!
		const cgltf_primitive& primitive = mesh.primitives[0];

		const size_t vtxCount = primitive.attributes[0].data->count;
		const size_t idxCount = primitive.indices->count;
		
		int64_t attributePosition = -1;
		int64_t attributeNormal = -1;
		int64_t attributeTexcoord = -1;
		int64_t attributeTangent = -1;
		int64_t attributeBoneIds = -1;
		int64_t attributeWeights = -1;

		for (int j = 0; j < primitive.attributes_count; j++) {
			const auto& attribute = primitive.attributes[j];
			std::string name = std::string(attribute.name);
			if (attribute.data->count != vtxCount) {
				Core::msg("AnimatedModel: Invalid attribute count in mesh %s for attribute %i.", mesh.name, (int)attribute.type);
				return;
			}

			if (attribute.type == cgltf_attribute_type_position) {
				attributePosition = j;
			}
			
			if (attribute.type == cgltf_attribute_type_normal) {
				attributeNormal = j;
			}
			
			if (attribute.type == cgltf_attribute_type_texcoord) {
				attributeTexcoord = j;
			}

			if (attribute.type == cgltf_attribute_type_tangent) {
				attributeTangent = j;
			}

			if (attribute.type == cgltf_attribute_type_joints) {
				attributeBoneIds = j;
			}

			if (attribute.type == cgltf_attribute_type_weights) {
				attributeWeights = j;
			}
		}

		std::vector<uint32_t> indices;
		std::vector<glm::vec3> positions;
		std::vector<glm::vec2> texcoords;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec4> tangents;
		std::vector<glm::ivec4> joints;
		std::vector<glm::vec4> weights;

		indices.resize(idxCount);
		cgltf_accessor_unpack_indices(primitive.indices, indices.data(), sizeof(uint32_t), idxCount);

		gltfUnpackValues(primitive, attributePosition, vtxCount, positions);
		gltfUnpackValues(primitive, attributeTexcoord, vtxCount, texcoords);
		gltfUnpackValues(primitive, attributeNormal, vtxCount, normals);
		gltfUnpackValues(primitive, attributeTangent, vtxCount, tangents);
		gltfUnpackValues(primitive, attributeBoneIds, vtxCount, joints);
		gltfUnpackValues(primitive, attributeWeights, vtxCount, weights);

		BoundingBox AABB;
		AABB.setIdentity();

		for (size_t o = 0; o < vtxCount; o++) {
			//positions[o].z = -positions[o].z;  // Sparkle uses LH Y+
			AABB.m_min = glm::min(positions[o], AABB.m_min);
			AABB.m_max = glm::max(positions[o], AABB.m_max);
		}

		// calculate vertices
		std::vector<AnimatedVertex> vertices;
		for (size_t o = 0; o < vtxCount; o++) {
			AnimatedVertex vertex;
			vertex.m_position = positions[o];
			vertex.m_normal = normals[o];
			vertex.m_texcoord = texcoords[o];
			vertex.m_tangent = glm::vec3(tangents[o]);
			vertex.m_bitangent = glm::cross(vertex.m_normal, vertex.m_tangent);
			vertex.m_boneIDs = joints[o];
			vertex.m_weights = weights[o];
			vertices.push_back(vertex);
		}

		AnimatedSubMesh* submesh = mem_new<AnimatedSubMesh>();
		submesh->m_vertices = vertices;
		submesh->m_verticesCount = vertices.size();
		submesh->m_indices = indices;
		submesh->m_indicesCount = indices.size();
		submesh->m_materialName = "materials/default_material.xml";
		m_subMeshes.push_back(submesh);
	}

	for (int i = 0; i < data->animations_count; i++) {
		Core::msg("AnimatedModel: animation %s channels %i", data->animations[i].name, data->animations[i].channels_count);
	}

	cgltf_free(data);
	free(fileData);

	createHw();
}

void AnimatedModel::createHw()
{
	for (int i = 0; i < m_subMeshes.size(); i++)
	{
		AnimatedSubMesh* submesh = m_subMeshes[i];

		BufferDesc bufferDesc;
		memset(&bufferDesc, 0, sizeof(bufferDesc));
		bufferDesc.m_bufferType = BufferType::VertexBuffer;
		bufferDesc.m_bufferAccess = BufferAccess::Static;
		bufferDesc.m_bufferMemorySize = submesh->m_vertices.size() * sizeof(AnimatedVertex);

		SubresourceDesc subresourceDesc;
		memset(&subresourceDesc, 0, sizeof(subresourceDesc));
		subresourceDesc.m_memory = submesh->m_vertices.data();
		subresourceDesc.m_memoryPitch = sizeof(AnimatedVertex);

		submesh->m_vertexBuffer = g_renderDevice->createBuffer(bufferDesc, subresourceDesc);
		submesh->m_vertices.clear();

		memset(&bufferDesc, 0, sizeof(bufferDesc));
		bufferDesc.m_bufferType = BufferType::IndexBuffer;
		bufferDesc.m_bufferAccess = BufferAccess::Static;
		bufferDesc.m_bufferMemorySize = submesh->m_indices.size() * sizeof(uint32_t);

		memset(&subresourceDesc, 0, sizeof(subresourceDesc));
		subresourceDesc.m_memory = submesh->m_indices.data();
		subresourceDesc.m_memoryPitch = sizeof(uint32_t);

		submesh->m_indexBuffer = g_renderDevice->createBuffer(bufferDesc, subresourceDesc);
		submesh->m_indices.clear();

		submesh->m_material = g_contentManager->loadObject<Material>(submesh->m_materialName);
	}
}

void AnimatedModel::releaseHw()
{
	for (int i = 0; i < m_subMeshes.size(); i++)
	{
		AnimatedSubMesh* submesh = m_subMeshes[i];
		submesh->m_material.reset();

		mem_delete(submesh->m_indexBuffer);
		mem_delete(submesh->m_vertexBuffer);

		mem_delete(submesh);
	}

	m_subMeshes.clear();
}

///////////////////////////////////////////////////////////
// Animated Model Renderer

AnimatedModelRenderer AnimatedModelRenderer::ms_instance;

AnimatedModelRenderer::AnimatedModelRenderer()
{
	m_jointTexture = nullptr;
	m_jointSampler = nullptr;
}

AnimatedModelRenderer::~AnimatedModelRenderer()
{
}

void AnimatedModelRenderer::init()
{
	// create a dynamic joint-palette texture and sampler

	TextureDesc textureDesc;
	memset(&textureDesc, 0, sizeof(textureDesc));
	textureDesc.m_textureType = TextureType::Texture2D;
	textureDesc.m_width = MAX_JOINTS * 3;
	textureDesc.m_height = MAX_INSTANCES;
	textureDesc.m_mipmapLevel = 1;
	textureDesc.m_format = ImageFormat::RGBA32F;
	textureDesc.m_isStreamTexture = true;

	SubresourceDesc subresourceDesc;
	memset(&subresourceDesc, 0, sizeof(subresourceDesc));
	subresourceDesc.m_memoryPitch = textureDesc.m_width * 4;

	m_jointTexture = g_renderDevice->createTexture2D(textureDesc, subresourceDesc);

	SamplerDesc samplerDesc;
	memset(&samplerDesc, 0, sizeof(samplerDesc));
	samplerDesc.m_minFilter = TextureFilter::Nearest;
	samplerDesc.m_magFilter = TextureFilter::Nearest;
	samplerDesc.m_wrapS = TextureWrap::ClampToEdge;
	samplerDesc.m_wrapT = TextureWrap::ClampToEdge;
	samplerDesc.m_anisotropyLevel = 1.0f;

	m_jointSampler = g_renderDevice->createSamplerState(samplerDesc);
}

void AnimatedModelRenderer::shutdown()
{
	if (m_jointSampler)
	{
		mem_delete(m_jointSampler);
		m_jointSampler = nullptr;
	}

	if (m_jointTexture)
	{
		mem_delete(m_jointTexture);
		m_jointTexture = nullptr;
	}
}

void AnimatedModelRenderer::render(AnimatedModel* model)
{
	// compute skinning matrices and write to joint texture upload buffer

	glm::mat4* skinningMatrices = mem_array<glm::mat4>(256);
	for (int i = 0; i < 256; i++) {

	}
}

}
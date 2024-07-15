#include "graphicspch.h"

#include "core/file/datastream.h"

// Graphics core
#include "graphics/core/device.h"
#include "graphics/core/texture.h"
#include "graphics/core/buffer.h"

#include "graphics/material.h"
#include "graphics/animatedmodel.h"

#define ANI_DEBUG

#ifdef ANI_DEBUG
#include "engine/camera.h"
#include "graphics/ifontmanager.h"
#endif // ANI_DEBUG

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

inline InterpolationType getInterpolationType(cgltf_interpolation_type type)
{
	//switch (gltf_animationSampler.interpolation)
	//{
	//case cgltf_interpolation_type_linear:
	//	animationSampler.m_interpolationType = InterpolationType_Linear;
	//	break;

	//case cgltf_interpolation_type_step:
	//	animationSampler.m_interpolationType = InterpolationType_Step;
	//	break;

	//case cgltf_interpolation_type_cubic_spline:
	//	animationSampler.m_interpolationType = InterpolationType_CubicSpline;
	//	break;
	//}

	switch (type)
	{
	case cgltf_interpolation_type_linear:
		return InterpolationType_Linear;
	
	case cgltf_interpolation_type_step:
		return InterpolationType_Step;
	
	case cgltf_interpolation_type_cubic_spline:
		return InterpolationType_CubicSpline;

	default:
		break;
	}

	return InterpolationType_Unknown;
}

inline AnimationPathType getAnimationPathType(cgltf_animation_path_type type)
{
	switch (type)
	{
	case cgltf_animation_path_type_invalid:
		break;
	
	case cgltf_animation_path_type_translation:
		return AnimationPathType_Translation;
	
	case cgltf_animation_path_type_rotation:
		return AnimationPathType_Rotation;
	
	case cgltf_animation_path_type_scale:
		return AnimationPathType_Scale;
	
	case cgltf_animation_path_type_weights:
		return AnimationPathType_Weights;

	default:
		break;
	}

	return AnimationPathType_Unknown;
}

// Object registering
void AnimatedModel::registerObject()
{
	TypeManager::getInstance()->registerObject<AnimatedModel>();
}

AnimatedModel::AnimatedModel()
{
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

	// loading animations

	for (int i = 0; i < data->animations_count; i++) {
		const cgltf_animation& gltf_animation = data->animations[i];

		Animation animation;
		animation.m_name = gltf_animation.name;

		for (int j = 0; j < gltf_animation.samplers_count; j++) {
			const cgltf_animation_sampler& gltf_animationSampler = gltf_animation.samplers[j];

			AnimationSampler animationSampler;
			animationSampler.m_interpolationType = getInterpolationType(gltf_animationSampler.interpolation);

			Assert(animationSampler.m_interpolationType != InterpolationType_Unknown);

			size_t inputCount = gltf_animationSampler.input->count;
			animationSampler.m_inputs.resize(inputCount);
			cgltf_accessor_read_float(gltf_animationSampler.input, 0, animationSampler.m_inputs.data(), inputCount);

			size_t outputCount = gltf_animationSampler.output->count;
			animationSampler.m_outputs.resize(outputCount);
			cgltf_accessor_unpack_floats(gltf_animationSampler.input, (float*)animationSampler.m_outputs.data(), outputCount);

			animation.m_samplers.push_back(animationSampler);
		}

		for (const auto& samplers : animation.m_samplers) {
			for (const auto& input : samplers.m_inputs) {
				if (input < animation.m_startTime) animation.m_startTime = input;
				if (input > animation.m_endTime) animation.m_endTime = input;
			}
		}

		for (int j = 0; j < gltf_animation.channels_count; j++) {
			const cgltf_animation_channel& gltf_animationChannel = gltf_animation.channels[j];

			AnimationChannel animationChannel;
			animationChannel.m_pathType = getAnimationPathType(gltf_animationChannel.target_path);
			
			Assert(animationChannel.m_pathType != AnimationPathType_Unknown);

			animationChannel.m_nodeId = cgltf_node_index(data, gltf_animationChannel.target_node);
			animationChannel.m_samplerId = cgltf_animation_sampler_index(&gltf_animation, gltf_animationChannel.sampler);

 			animation.m_channels.push_back(animationChannel);
		}

		m_animations.push_back(animation);

		Core::msg("AnimatedModel: animation %s channels %i", gltf_animation.name, gltf_animation.channels_count);
	}

	// loading model skin
	for (int i = 0; i < data->skins_count; i++) {
		const cgltf_skin& gltf_skin = data->skins[i];
		m_joints.resize(gltf_skin.joints_count);

		AnimationSkin skin;
		skin.m_name = gltf_skin.name;

		// load joint too
		for (int j = 0; j < gltf_skin.joints_count; j++) {
			const cgltf_node* joint = gltf_skin.joints[j];

			int jointIndex = cgltf_node_index(data, joint);
			m_joints[jointIndex].m_name = joint->name;

			if (joint->has_translation)
				m_joints[jointIndex].m_translation = glm::vec3(joint->translation[0], joint->translation[1], joint->translation[2]);
			else
				m_joints[jointIndex].m_translation = glm::vec3(0.0f);

			if (joint->has_rotation)
				m_joints[jointIndex].m_rotation = glm::quat(joint->rotation[3], joint->rotation[0], joint->rotation[1], joint->rotation[2]);
			else
				m_joints[jointIndex].m_rotation = glm::identity<glm::quat>();

			if (joint->has_scale)
				m_joints[jointIndex].m_scale = glm::vec3(joint->scale[0], joint->scale[1], joint->scale[2]);
			else
				m_joints[jointIndex].m_scale = glm::vec3(0.0f);
			
			if (joint->has_matrix)
				memcpy(&m_joints[jointIndex].m_matrix[0][0], &joint->matrix[0], 16 * sizeof(float));
			else
				m_joints[jointIndex].m_matrix = glm::mat4(1.0f);

			skin.m_joints.push_back(jointIndex);
		}

		// load the inverse bind matrices
		size_t inverseBindMatricesCount = gltf_skin.inverse_bind_matrices->count;
		skin.m_inverseBindMatrices.resize(inverseBindMatricesCount);
		cgltf_accessor_unpack_floats(gltf_skin.inverse_bind_matrices, (float*)skin.m_inverseBindMatrices.data(), inverseBindMatricesCount);

		if (gltf_skin.skeleton)
			skin.m_skeletonRootId = cgltf_node_index(data, gltf_skin.skeleton);

		m_skins.push_back(skin);
	}

	m_nodes.resize(data->nodes_count);

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

int AnimatedModel::getAnimationByName(const std::string& name)
{
	for (int i = 0; i < m_animations.size(); i++)
		if (m_animations[i].m_name == name)
			return i;

	return -1;
}

void AnimatedModel::setPlayAnimation(int index, bool looped)
{
	m_currentAnimation = &m_animations[index];
	m_playLooped = looped;
}

void AnimatedModel::testPlay(float dt)
{
	float animationLengthTime = m_currentAnimation->m_endTime - m_currentAnimation->m_startTime;
	
	m_currentTime += 1.0f * dt;
	if (m_currentTime > animationLengthTime) {
		
		if (m_playLooped)
			m_currentTime = m_currentTime - animationLengthTime;
		else
			m_currentTime = m_currentAnimation->m_endTime;
	}

#ifdef ANI_DEBUG
	char buf[256];
	snprintf(buf, sizeof(buf), "Animation: %s", m_currentAnimation->m_name.c_str());
	g_fontManager->drawSystemFontShadowed(buf, 100, 100, glm::vec4(1.0f));

	snprintf(buf, sizeof(buf), "Current time: %.2f", m_currentTime);
	g_fontManager->drawSystemFontShadowed(buf, 100, 120, glm::vec4(1.0f));
#endif // ANI_DEBUG

	float animationCurrentTime = m_currentTime + m_currentAnimation->m_startTime; // смещаем к началу анимации
	for (int i = 0; i < m_currentAnimation->m_channels.size(); i++) {
		const AnimationChannel& channel = m_currentAnimation->m_channels[i];
		const AnimationSampler& sampler = m_currentAnimation->m_samplers[channel.m_samplerId];

		for (size_t j = 0; j < sampler.m_inputs.size() - 1; ++j) {
			if ((animationCurrentTime >= sampler.m_inputs[j]) && (animationCurrentTime <= sampler.m_inputs[j + 1])) {  // find time sampler
				float mixValue = std::max(0.0f, animationCurrentTime - sampler.m_inputs[j]) / (sampler.m_inputs[j + 1] - sampler.m_inputs[j]);
				if (mixValue <= 1.0f) {
					if (channel.m_pathType == AnimationPathType_Translation) {
						glm::vec3 A = sampler.m_outputs[j];
						glm::vec3 B = sampler.m_outputs[j + 1];
						glm::vec3 translation = glm::lerp(A, B, mixValue);
						m_nodes[channel.m_nodeId].m_translation = translation;
					}
					else if (channel.m_pathType == AnimationPathType_Scale) {
						glm::vec3 A = sampler.m_outputs[j];
						glm::vec3 B = sampler.m_outputs[j + 1];
						glm::vec3 scale = glm::lerp(A, B, mixValue);
						m_nodes[channel.m_nodeId].m_scale = scale;
					}
					else if (channel.m_pathType == AnimationPathType_Rotation) {
						glm::quat A = (0.0f, sampler.m_outputs[j]);
						glm::quat B = (0.0f, sampler.m_outputs[j + 1]);
						glm::quat rotate = glm::slerp(A, B, mixValue);
						m_nodes[channel.m_nodeId].m_rotation = rotate;
					}

#ifdef ANI_DEBUG
					snprintf(buf, sizeof(buf), "Sampler: %i", j);
					g_fontManager->drawSystemFontShadowed(buf, 100, 140, glm::vec4(1.0f));

					static const char* animationPathStr[AnimationPathType_Count] =
					{
						"AnimationPathType_Unknown",
						"AnimationPathType_Translation",
						"AnimationPathType_Rotation",
						"AnimationPathType_Scale",
						"AnimationPathType_Weights"
					};

					snprintf(buf, sizeof(buf), "Channel animation path: %s", animationPathStr[channel.m_pathType]);
					g_fontManager->drawSystemFontShadowed(buf, 100, 160, glm::vec4(1.0f));
#endif // ANI_DEBUG
				}
			}
		}
	}

}

///////////////////////////////////////////////////////////
// Animated Model Renderer

#include "graphics/shaderconstantmanager.h"

struct BonesCB
{
	glm::mat4 matrices[256];
};

//ConstantBufferProxy g_bonesConstantBuffer;

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
	//g_bonesConstantBuffer = ShaderConstantManager::getInstance()->create<BonesCB>("BonesCB");

#if 0
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
#endif
}

void AnimatedModelRenderer::shutdown()
{
#if 0
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
#endif
}

void AnimatedModelRenderer::render(AnimatedModel* model)
{
	// compute skinning matrices and write to joint texture upload buffer

	//glm::mat4* skinningMatrices = mem_array<glm::mat4>(256);
	//for (int i = 0; i < 256; i++) {
	//
	//}
}

}
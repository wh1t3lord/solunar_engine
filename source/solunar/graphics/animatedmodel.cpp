#include "graphicspch.h"

#include "core/file/datastream.h"

// Graphics core
#include "graphics/core/device.h"
#include "graphics/core/texture.h"
#include "graphics/core/buffer.h"

#include "graphics/renderer.h"
#include "graphics/material.h"
#include "graphics/animatedmodel.h"
#include "graphics/mesh.h"
#include "graphics/shaderconstantmanager.h"
#include "graphics/debugrenderer.h"

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
		std::vector<glm::vec4> joints;
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

			if (normals.size() > 0) {
				vertex.m_normal = normals[o];
			} else {
				vertex.m_normal = glm::vec3(0.0f);
			}

			if (texcoords.size() > 0) {
				vertex.m_texcoord = texcoords[o];
			} else {
				vertex.m_texcoord = glm::vec2(0.0f);
			}

			if (tangents.size() > 0) {
				vertex.m_tangent = glm::vec3(tangents[o]);
			} else {
				vertex.m_tangent = glm::vec3(0.0f);
			}

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
	m_animations.resize( data->animations_count);

	for (int i = 0; i < data->animations_count; i++) {
		const cgltf_animation& gltf_animation = data->animations[i];

		Animation& animation = m_animations[i];
		animation.m_name = "unknowed animation name";
		if (gltf_animation.name)
			animation.m_name = gltf_animation.name;

		animation.m_samplers.resize(gltf_animation.samplers_count);
		for (int j = 0; j < gltf_animation.samplers_count ; j++) {
			const cgltf_animation_sampler& gltf_animationSampler = gltf_animation.samplers[j];

			AnimationSampler& animationSampler = animation.m_samplers[j];

			animationSampler.m_interpolationType = getInterpolationType(gltf_animationSampler.interpolation);

			Assert(animationSampler.m_interpolationType != InterpolationType_Unknown);

			size_t inputCount = gltf_animationSampler.input->count;
			animationSampler.m_inputs.resize(inputCount);
			cgltf_accessor_unpack_floats(gltf_animationSampler.input, (float*)animationSampler.m_inputs.data(), inputCount);

			size_t outputCount = gltf_animationSampler.output->count;
			animationSampler.m_outputs.resize(outputCount);
			
			if (gltf_animationSampler.output->type == cgltf_type_vec3) {
				for (int k = 0; k < outputCount ; k++) {
					glm::vec3 outputValue = glm::vec3(0.0f);
					cgltf_accessor_read_float(gltf_animationSampler.output, k, (float*)&outputValue, sizeof(glm::vec3));
					animationSampler.m_outputs[k] = glm::vec4(outputValue, 0.0f);
				}	
			} else if (gltf_animationSampler.output->type == cgltf_type_vec4) {
				cgltf_accessor_unpack_floats(gltf_animationSampler.output, (float*)animationSampler.m_outputs.data(), outputCount * sizeof(glm::vec4));
			}
		}

		for (const auto& samplers : animation.m_samplers) {
			for (const auto& input : samplers.m_inputs) {
				if (input < animation.m_startTime) animation.m_startTime = input;
				if (input > animation.m_endTime) animation.m_endTime = input;
			}
		}

		animation.m_channels.resize(gltf_animation.channels_count);
		for (int j = 0; j < gltf_animation.channels_count; j++) {
			const cgltf_animation_channel& gltf_animationChannel = gltf_animation.channels[j];

			AnimationChannel& animationChannel = animation.m_channels[j];
			animationChannel.m_pathType = getAnimationPathType(gltf_animationChannel.target_path);
			
			Assert(animationChannel.m_pathType != AnimationPathType_Unknown);

			animationChannel.m_nodeId = cgltf_node_index(data, gltf_animationChannel.target_node);
			animationChannel.m_samplerId = cgltf_animation_sampler_index(&gltf_animation, gltf_animationChannel.sampler);
		}

		Core::msg("AnimatedModel: animation %s channels %i", gltf_animation.name, gltf_animation.channels_count);
	}

	// loading model skin
	m_skins.resize(data->skins_count);
	for (int i = 0; i < data->skins_count; i++) {
		const cgltf_skin& gltf_skin = data->skins[i];
	//	m_joints.resize(gltf_skin.joints_count);

		AnimationSkin& skin = m_skins[i];
		skin.m_name = "unknowed skin name";
		if (gltf_skin.name)
			skin.m_name = gltf_skin.name;

		skin.m_joints.resize(gltf_skin.joints_count);

		// load joint too
		for (int j = 0; j < gltf_skin.joints_count; j++) {
			const cgltf_node* joint = gltf_skin.joints[j];

			int jointIndex = cgltf_node_index(data, joint);
			skin.m_joints[j] = jointIndex;
#if 0
			if (joint->name) {
				m_joints[jointIndex].m_name = joint->name;
			} else {
				char buf[32];
				snprintf(buf, sizeof(buf), "Joint %i", j);
				m_joints[jointIndex].m_name = buf;
			}

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
#endif	
		}

		// load the inverse bind matrices
		size_t inverseBindMatricesCount = gltf_skin.inverse_bind_matrices->count;
		skin.m_inverseBindMatrices.resize(inverseBindMatricesCount);
		cgltf_accessor_unpack_floats(gltf_skin.inverse_bind_matrices, (float*)skin.m_inverseBindMatrices.data(), inverseBindMatricesCount * 16);

		if (gltf_skin.skeleton)
			skin.m_skeletonRootId = cgltf_node_index(data, gltf_skin.skeleton);

		m_skins.push_back(skin);
	}

	m_nodes.resize(data->nodes_count);

	for (int i = 0; i < data->nodes_count; i++) {
		auto& node = m_nodes[i];
		node.m_name = data->nodes[i].name;

		if (data->nodes[i].has_translation)
			node.m_translation = glm::vec3(data->nodes[i].translation[0], data->nodes[i].translation[1], data->nodes[i].translation[2]);
		else
			node.m_translation = glm::vec3(0.0f);

		if (data->nodes[i].has_rotation)
			node.m_rotation = glm::quat(data->nodes[i].rotation[3], data->nodes[i].rotation[0], data->nodes[i].rotation[1], data->nodes[i].rotation[2]);
		else
			node.m_rotation = glm::identity<glm::quat>();

		if (data->nodes[i].has_scale)
			node.m_scale = glm::vec3(data->nodes[i].scale[0], data->nodes[i].scale[1], data->nodes[i].scale[2]);
		else
			node.m_scale = glm::vec3(1.0f);

		if (data->nodes[i].has_matrix)
			memcpy(&node.m_matrix[0][0], &data->nodes[i].matrix[0], 16 * sizeof(float));
		else
			node.m_matrix = glm::mat4(1.0f);

		if (data->nodes[i].parent)
			node.m_parentId = cgltf_node_index(data, data->nodes[i].parent);

		if (data->nodes[i].children_count > 0) {
			for (int j = 0; j < data->nodes[i].children_count; j++) {
				node.m_children.push_back(cgltf_node_index(data, data->nodes[i].children[j]));
			}
		}

		if (data->nodes[i].skin)
			node.m_skinId = cgltf_skin_index(data, data->nodes[i].skin);
	}

#if 0
	int rootNode = m_nodes.size();
	m_nodes.resize(m_nodes.size() + 1);
	
	for (int i = 0; i < m_nodes.size() - 1; ++i) {
		if (m_nodes[i].m_parentId == -1) {
			m_nodes[i].m_parentId = rootNode;
			m_nodes[rootNode].m_children.push_back(i);
		}
	}
	// main rotate Y
	m_nodes[rootNode].m_name = "Root Node";
	m_nodes[rootNode].m_parentId = -1;
	m_nodes[rootNode].m_matrix = glm::mat4(1.0f);
	m_nodes[rootNode].m_translation = glm::vec3(0.0f);
	m_nodes[rootNode].m_scale = glm::vec3(0.01f);
	m_nodes[rootNode].m_rotation.x = 0;
	m_nodes[rootNode].m_rotation.y = sinf(180.0 / 2.0 / 180.0 * maths::PI);
	m_nodes[rootNode].m_rotation.z = 0;
	m_nodes[rootNode].m_rotation.w = cosf(180.0 / 2.0 / 180.0 * maths::PI);
#endif
	//for (auto it : m_joints)
	//	m_bones.emplace(it.m_name, it);

	/// todo !!!!!!!!!!!!!! cgltf_free(data);

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
	m_currentTime = 0.0f;
}

inline glm::quat samplerRotationToGlm(const glm::vec4& v)
{
	return glm::quat(v.w, v.x, v.y, v.z);
}

void AnimatedModel::testPlay(float dt)
{
	m_currentTime += 1.0f * dt;
	
	bool updated = false;
	Animation& animation = *m_currentAnimation;
	float time = std::fmod(static_cast<float>(m_currentTime), animation.m_endTime - animation.m_startTime);

	for (auto& channel : animation.m_channels)
	{
		AnimationSampler& sampler = animation.m_samplers[channel.m_samplerId];
		if (sampler.m_inputs.size() > sampler.m_outputs.size())
			continue;

		for (std::size_t i = 0; i < sampler.m_inputs.size() - 1; ++i)
		{
			if ((time >= sampler.m_inputs[i]) && (time <= sampler.m_inputs[i + 1]))
			{
				float u = std::max(0.0f, time - sampler.m_inputs[i]) / (sampler.m_inputs[i + 1] - sampler.m_inputs[i]);
				if (u <= 1.0f)
				{
					/* NOTE: use the previous sampler output information of the current frame              */
					/* TODO: interpolate the sampler output information before and after the current frame */
					/*       using the interpolation information stored in the sampler                     */
					if (channel.m_pathType == AnimationPathType_Translation)
					{
						auto A = sampler.m_outputs[i];
						auto B = sampler.m_outputs[i + 1];
						auto translation = glm::lerp(A, B, u);
						m_nodes[channel.m_nodeId].m_translation = translation;
#if 0
						auto translation = sampler.m_outputs[i];
						m_nodes[channel.m_nodeId].m_translation = glm::vec3(translation);
#endif
					}
					else if (channel.m_pathType == AnimationPathType_Scale)
					{
						auto A = sampler.m_outputs[i];
						auto B = sampler.m_outputs[i + 1];
						auto scale = glm::lerp(A, B, u);
						m_nodes[channel.m_nodeId].m_scale = scale;
#if 0
						auto scale = sampler.m_outputs[i];
						m_nodes[channel.m_nodeId].m_scale = glm::vec3(scale);
#endif
					}
					else if (channel.m_pathType == AnimationPathType_Rotation)
					{
						auto A = samplerRotationToGlm(sampler.m_outputs[i]);
						auto B = samplerRotationToGlm(sampler.m_outputs[i + 1]);
						auto rotate = glm::slerp(A, B, u);
						m_nodes[channel.m_nodeId].m_rotation = glm::normalize(rotate);
#if 0
						auto rotation = sampler.m_outputs[i];
						m_nodes[channel.m_nodeId].m_rotation = glm::quat(rotation.w, rotation.x, rotation.y, rotation.z);
#endif
					}

					updated = true;
				}
			}
		}
	}

	if (updated == true)
	{
		//for (int i = 0; i < m_nodes.size(); i++) {
		//	updateNode(i);
		//}

		updateNode(m_nodes.size() - 1);
	}
}

#define MAX_BONES 64

static glm::mat4 s_boneInfoTest[MAX_BONES];

void AnimatedModel::updateNode(int node_id)
{
	auto& node = m_nodes[node_id];
	if (node.m_skinId != -1) {
		auto& skin = m_skins[node.m_skinId];
		auto inverse_transform = glm::inverse(getNodeMatrix(node_id));
		unsigned int num_joints = skin.m_joints.size();//m_joints.size(); //std::min(static_cast<unsigned int>(skin.joints.size()), MAX_NUM_JOINTS);
		for (unsigned int i = 0; i < num_joints; ++i)
		{
			/* NOTE: Reference: https://github.com/KhronosGroup/glTF-Tutorials/blob/master/gltfTutorial/gltfTutorial_020_Skins.md */
			auto joint_mat	=skin.m_inverseBindMatrices[i];
			joint_mat		= joint_mat * getNodeMatrix(skin.m_joints[i]);
			joint_mat		= joint_mat * inverse_transform;
			s_boneInfoTest[i] = joint_mat;

		/*	BoneInfo boneInfo;
			boneInfo.id = skin.m_joints[i];
			boneInfo.offset = joint_mat;
			s_boneInfoTest[s_boneInfoTestCounter++] = boneInfo;*/

			//mesh.joint_matrices[i] = joint_mat;
		}
	}

	for (int i = 0; i < node.m_children.size(); i++)
		updateNode(node.m_children[i]);

}

glm::mat4 makeRotationMatrix(const glm::quat& u)
{
	float wx, wy, wz, xx, yy, yz, xy, xz, zz;

	// adapted from Shoemake
	xx = u.x * u.x;
	xy = u.x * u.y;
	xz = u.x * u.z;
	yy = u.y * u.y;
	zz = u.z * u.z;
	yz = u.y * u.z;

	wx = u.w * u.x;
	wy = u.w * u.y;
	wz = u.w * u.z;

	// make identity matrix
	float matrix[16];

	matrix[0] = 1.0f - 2.0f * (yy + zz);
	matrix[4] = 2.0f * (xy - wz);
	matrix[8] = 2.0f * (xz + wy);
	matrix[12] = 0.0;

	matrix[1] = 2.0f * (xy + wz);
	matrix[5] = 1.0f - 2.0f * (xx + zz);
	matrix[9] = 2.0f * (yz - wx);
	matrix[13] = 0.0;

	matrix[2] = 2.0f * (xz - wy);
	matrix[6] = 2.0f * (yz + wx);
	matrix[10] = 1.0f - 2.0f * (xx + yy);
	matrix[14] = 0.0;

	matrix[3] = 0;
	matrix[7] = 0;
	matrix[11] = 0;
	matrix[15] = 1;

	return glm::mat4(	matrix[0],	matrix[1],	matrix[2],	matrix[3],
						matrix[4],	matrix[5],	matrix[6],	matrix[7],
						matrix[8],	matrix[9],	matrix[10], matrix[11],
						matrix[12], matrix[13], matrix[14], matrix[15]);
}

glm::mat4 AnimatedModel::getNodeMatrix(int nodeId)
{
	Assert(nodeId != -1);
	AnimationNode& node = m_nodes[nodeId];
	glm::mat4 tranlation = glm::mat4(1.0f);
	tranlation =  tranlation * glm::scale(glm::mat4(1.0f), node.m_scale);
	//tranlation = glm::scale(glm::mat4(1.0f), node.m_scale) *  tranlation ;
	tranlation = tranlation * glm::toMat4(node.m_rotation);
	//tranlation = tranlation * makeRotationMatrix(node.m_rotation);
	//tranlation = glm::toMat4(node.m_rotation) * tranlation;
	tranlation = tranlation * glm::translate(glm::mat4(1.0f), node.m_translation);
	//tranlation = glm::translate(glm::mat4(1.0f), node.m_translation) * tranlation ;
	

	for (auto id = node.m_parentId; id != -1; ) {
		auto& nodeParent = m_nodes[id];
		glm::mat4 parentTranlation = glm::mat4(1.0f);
		parentTranlation = parentTranlation * glm::scale(glm::mat4(1.0f), nodeParent.m_scale);
		//parentTranlation = glm::scale(glm::mat4(1.0f), nodeParent.m_scale) * parentTranlation;
		parentTranlation = parentTranlation * glm::toMat4(nodeParent.m_rotation);
		//parentTranlation = parentTranlation * makeRotationMatrix(nodeParent.m_rotation);
		//parentTranlation = glm::toMat4(nodeParent.m_rotation) * parentTranlation;
		parentTranlation = parentTranlation * glm::translate(glm::mat4(1.0f), nodeParent.m_translation);
		//parentTranlation = glm::translate(glm::mat4(1.0f), nodeParent.m_translation) * parentTranlation ;

		tranlation = tranlation * parentTranlation;
		//tranlation = parentTranlation *  tranlation;
		id = nodeParent.m_parentId;
	}

	return tranlation;
	//return glm::transpose( tranlation);
}


///////////////////////////////////////////////////////////
// Animated Model Renderer

struct BonesCB
{
	glm::mat4 matrices[256];
};

ConstantBufferProxy g_bonesConstantBuffer;

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
	g_bonesConstantBuffer = ShaderConstantManager::getInstance()->create<BonesCB>("BonesCB");

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

void AnimatedModelRenderer::render(AnimatedMeshComponent* model)
{
	glm::mat4* data = (glm::mat4*)g_bonesConstantBuffer->map(BufferMapping::WriteOnly);
	memcpy(data, s_boneInfoTest, sizeof(s_boneInfoTest));
	g_bonesConstantBuffer->unmap();

	g_renderDevice->setConstantBufferIndex(ConstantBufferBindings_Skinning, g_bonesConstantBuffer.get());

	//std::shared_ptr<ModelBase> modelBase = model->lockModel();
	//AnimatedModel* animatedModel = dynamicCast<AnimatedModel>(modelBase.get());

	// compute skinning matrices and write to joint texture upload buffer

	//glm::mat4* skinningMatrices = mem_array<glm::mat4>(256);
	//for (int i = 0; i < 256; i++) {
	//
	//}
}

}
#ifndef ANIMATEDMODEL_H
#define ANIMATEDMODEL_H

#include "graphics/model.h"
#include "graphics/material.h"

namespace engine
{

// the upper limit for joint palette size is 256 (because the mesh joint indices
// are stored in packed byte-size vertex formats), but the example mesh only needs less than 64
#define MAX_JOINTS (64)

// this defines the size of the instance-buffer and height of the joint-texture
#define MAX_INSTANCES (512)

// this defines the size of the vertex weight
#define MAX_BONE_WEIGHT 4

// Forward declaration
class ITexture2D;
class ISamplerState;
class IBufferBase;

struct AnimatedVertex
{
	glm::vec3 m_position;
	glm::vec3 m_normal;
	glm::vec2 m_texcoord;
	glm::vec3 m_tangent;
	glm::vec3 m_bitangent;
	int32_t m_boneIDs[MAX_BONE_WEIGHT];
	float m_weights[MAX_BONE_WEIGHT];
};

struct AnimatedModelBoneInfo
{
	/*id is index in finalBoneMatrices*/
	int m_id;

	/*offset matrix transforms vertex from model space to bone space*/
	glm::mat4 m_offset;
};

using BoneInfoMap = std::map<std::string, AnimatedModelBoneInfo>;

//struct AnimatedModelSkin
//{
//    std::string m_name;
//    std::vector<int32_t> m_joints;
//	std::vector<glm::mat4> m_inverseBindMatrices;
//	int32_t m_skeletonRootId;
//
//	void init()
//	{
//		m_joints = {};
//		m_inverseBindMatrices = {};
//		m_skeletonRootId = -1;
//	}
//};

struct AnimatedSubMesh
{
	IBufferBase* m_vertexBuffer;
	std::weak_ptr<Material> m_material;
	std::string m_materialName;
};

class AnimatedModel : public ModelBase
{
	ImplementObject(AnimatedModel, ModelBase);
public:
	AnimatedModel();
	~AnimatedModel();

	static void registerObject();

	void load(const std::shared_ptr<DataStream>& dataStream) override;

	void createHw() override;
	void releaseHw() override;

	BoneInfoMap& getBoneMapInfo() { return m_boneInfoMap; }
	int& getBoneCount() { return m_boneCount; }

private:
	std::vector<AnimatedSubMesh*> m_subMeshes;
	BoneInfoMap m_boneInfoMap;
	int m_boneCount;
};

class AnimatedModelRenderer : public Singleton<AnimatedModelRenderer>
{
private:
	static AnimatedModelRenderer ms_instance;

public:
	AnimatedModelRenderer();
	~AnimatedModelRenderer();

	void init();
	void shutdown();

	void render(AnimatedModel* model);

private:
	ITexture2D* m_jointTexture;
	ISamplerState* m_jointSampler;

};

}

#endif // !ANIMATEDMODEL_H

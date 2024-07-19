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

enum InterpolationType
{
	InterpolationType_Unknown,
	InterpolationType_Linear,
	InterpolationType_Step,
	InterpolationType_CubicSpline,
	InterpolationType_Count
};

enum AnimationPathType {
	AnimationPathType_Unknown,
	AnimationPathType_Translation,
	AnimationPathType_Rotation,
	AnimationPathType_Scale,
	AnimationPathType_Weights,
	AnimationPathType_Count
};

struct AnimatedVertex
{
	glm::vec3 m_position;
	glm::vec3 m_normal;
	glm::vec2 m_texcoord;
	glm::vec3 m_tangent;
	glm::vec3 m_bitangent;
	glm::vec4 m_boneIDs;
	glm::vec4 m_weights;
};

struct AnimatedSubMesh
{
	std::vector<AnimatedVertex> m_vertices;
	std::vector<uint32_t> m_indices;
	IBufferBase* m_vertexBuffer;
	IBufferBase* m_indexBuffer;
	std::weak_ptr<Material> m_material;
	std::string m_materialName;
	uint32_t m_verticesCount;
	uint32_t m_indicesCount;
};

struct AnimationSampler
{
	InterpolationType m_interpolationType = InterpolationType_Unknown;
	std::vector<float> m_inputs = {};
	std::vector<glm::vec4> m_outputs = {};
};

struct AnimationChannel {
	AnimationPathType m_pathType = AnimationPathType_Unknown;
	int m_nodeId = -1;
	int m_samplerId = -1;
};

struct Animation
{
	std::string m_name;
	std::vector<AnimationSampler> m_samplers;
	std::vector<AnimationChannel> m_channels;
	float m_startTime = std::numeric_limits<float>::max();
	float m_endTime = std::numeric_limits<float>::min();
};

struct AnimationSkin
{
	std::string m_name = "";
	std::vector<int> m_joints = {};
	std::vector<glm::mat4> m_inverseBindMatrices = {};
	int m_skeletonRootId = -1;
};

struct AnimationNode
{
	std::string m_name;
	glm::vec3 m_translation;
	glm::quat m_rotation;
	glm::vec3 m_scale;
	glm::mat4 m_matrix;
	std::vector<int> m_children;
	int m_parentId = -1;
	int m_skinId = -1;
};

struct BoneInfo
{
	/*id is index in finalBoneMatrices*/
	int id;

	/*offset matrix transforms vertex from model space to bone space*/
	glm::mat4 offset;
};

class AnimatedModel : public ModelBase
{
	ImplementObject(AnimatedModel, ModelBase);
public:
	AnimatedModel();
	~AnimatedModel();

	static void registerObject();

	void load(const std::shared_ptr<DataStream>& dataStream) override;
	void load_GLTF(const std::shared_ptr<DataStream>& dataStream);

	void createHw() override;
	void releaseHw() override;

	std::vector<AnimatedSubMesh*>& getAnimatedSubmehes() { return m_subMeshes; }

	int getAnimationByName(const std::string& name);
	void setPlayAnimation(int index, bool looped = false);

	void testPlay(float dt);

	void updateNode(int node_id);
	void updateNodePreCasheFrow(int node_id);

	glm::mat4 getNodeMatrix(int nodeId);

	const BoundingBox& getBoundingBox();

private:
	std::vector<AnimatedSubMesh*> m_subMeshes;
	std::vector<Animation> m_animations;
	std::vector<AnimationSkin> m_skins;
	//std::vector<AnimationNode> m_joints;
	std::vector<AnimationNode> m_nodes;
	std::map<std::string, BoneInfo> m_bones;
	Animation* m_currentAnimation;
	bool m_playLooped = false;
	float m_currentTime = 0.0f;
	
	BoundingBox m_boundingBox;
};

class AnimatedMeshComponent;

class AnimatedModelRenderer : public Singleton<AnimatedModelRenderer>
{
private:
	static AnimatedModelRenderer ms_instance;

public:
	AnimatedModelRenderer();
	~AnimatedModelRenderer();

	void init();
	void shutdown();

	void render(AnimatedMeshComponent* model);

private:
	ITexture2D* m_jointTexture;
	ISamplerState* m_jointSampler;

};

}

#endif // !ANIMATEDMODEL_H

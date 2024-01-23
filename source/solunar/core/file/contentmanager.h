#ifndef CONTENTMANAGER_H
#define CONTENTMANAGER_H

#include <string>
#include <unordered_map>

#include "core/utils/thread.h"
#include "core/file/contentdevice.h"
#include "core/file/datastream.h"
#include "core/object/serializableobject.h"

namespace engine
{

class ContentManager : public Singleton<ContentManager>
{
public:
	void init();
	void shutdown();

	void mountDevice(ContentDevice* contentDevice, const std::string& name);
	void unountDevice(const std::string& name);

	// #TODO: remove
	// Little hack for using legacy code
	DataStreamPtr openStream(const std::string& path);
	DataStreamPtr openStream(const std::string& name, const std::string& path);

	std::weak_ptr<SerializableObject> load(const std::string& filename, const TypeInfo* pTypeInfo);

	template <typename T>
	std::weak_ptr<T> loadObject(const std::string& filename);
	
	//void loadExisted(const std::string& filename, std::shared_ptr<SerializableObject> object);

	//std::shared_ptr<TextureMap> loadTexture(const std::string& textureName) { return dynamicCastPtr<TextureMap>(load(textureName, TextureMap::getStaticTypeInfo())); }
	//std::shared_ptr<ModelBase> loadModelOld(const std::string& modelName) { return dynamicCastPtr<ModelBase>(load(modelName, ModelBase::getStaticTypeInfo())); }
	//std::shared_ptr<Material> loadMaterial(const std::string& materialName) { return dynamicCastPtr<Material>(load(materialName, Material::getStaticTypeInfo())); }
	//std::shared_ptr<Model> loadModel(const std::string& modelName) { return dynamicCastPtr<Model>(load(modelName, Model::getStaticTypeInfo())); }

private:
	ContentDevice* findContentDevice(const std::string& name);

private:
	std::unordered_map<std::string, std::shared_ptr<SerializableObject>> m_content;
	std::unordered_map<std::string, ContentDevice*> m_devices;

};

extern ContentManager* g_contentManager;

template<typename T>
inline std::weak_ptr<T> ContentManager::loadObject(const std::string& filename)
{
	return dynamicCastPtr<T>(load(filename, T::getStaticTypeInfo()));
}

}

#endif // !CONTENTMANAGER_H

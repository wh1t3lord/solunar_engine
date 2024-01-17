namespace engine
{

template<typename T>
inline T* Entity::createComponent()
{
	return (T*)createComponentByTypeInfo(T::getStaticTypeInfo());
}

template<typename T>
inline T* Entity::getComponent()
{
	return (T*)getComponentByTypeInfo(T::getStaticTypeInfo());
}

template<typename T>
inline std::vector<T*> Entity::getComponents()
{
	std::vector<T*> components;

	for (auto it : m_components)
	{
		if (it->isA(T::getStaticTypeInfo()))
			components.push_back(static_cast<T*>(it));
	}

	return components;
}

}
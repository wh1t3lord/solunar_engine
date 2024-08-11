namespace solunar
{

template<typename T>
inline T* Entity::CreateComponent()
{
	return (T*)CreateComponentByTypeInfo(T::GetStaticTypeInfo());
}

template<typename T>
inline T* Entity::GetComponent()
{
	return (T*)GetComponentByTypeInfo(T::GetStaticTypeInfo());
}

template<typename T>
inline std::vector<T*> Entity::GetComponents()
{
	std::vector<T*> components;

	for (auto it : m_components)
	{
		if (it->IsA(T::GetStaticTypeInfo()))
			components.push_back(static_cast<T*>(it));
	}

	return components;
}

}
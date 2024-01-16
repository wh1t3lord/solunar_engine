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

}
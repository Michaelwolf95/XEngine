#pragma once
#include <string>
#include <vector>
#include <map>
template<typename K, class T>
class AssetLibrary
{
public:
	virtual T& GetAsset(K key);
protected:
	std::map<K, T> library;
	virtual T& LoadAsset(K key) = 0;
};

template<typename K, class T>
inline T & AssetLibrary<K,T>::GetAsset(K key)
{
	auto search = this->library.find(key);
	if (search == this->library.end())
	{
		// Not found.
		return LoadAsset(key);
	}
	else
	{
		// Found
		return this->library[key];
	}
}

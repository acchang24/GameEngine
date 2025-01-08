#pragma once
#include <string>
#include <unordered_map>
#include <iostream>

class AssetManager;

// Cache is a template class used by the AssetManager.
// It helps store assets into its respective templated map
// for on demand storing and loading
template <class T>
class Cache
{
public:
	// Cache constructor
	// @param - AssetManager* for the manager
	Cache(AssetManager* manager) : mManager(manager)
	{
	}

	~Cache()
	{
		std::cout << "Delete cache" << std::endl;
		Clear();
	}

	// StoreCache takes in a key and value pair and stores them
	// into the templated asset's asset map if it doesn't exist.
	// @param - const std::string& for the asset's name
	// @param - T* for the templated asset that's going to be stored
	void StoreCache(const std::string& key, T* asset)
	{
		if (mAssetMap.find(key) == mAssetMap.end())
		{
			mAssetMap[key] = asset;
		}
	}

	// Retrieves an asset of type T* from the asset map by name, returns nullptr if it doesn't exist
	// @param - const std::string& for the asset's name
	// @return - T* for the templated data
	T* Get(const std::string& name)
	{
		auto iter = mAssetMap.find(name);

		if (iter != mAssetMap.end())
		{
			return iter->second;
		}
		return nullptr;
	}

	// Loops through and calls delete on each element in the asset map, then clears the map
	void Clear()
	{
		for (auto& a : mAssetMap)
		{
			delete a.second;
		}
		mAssetMap.clear();
	}

	// Removes an asset by name, and free the memory
	// @param - const std::string& for the asset name
	void Delete(const std::string& name)
	{
		auto iter = mAssetMap.find(name);
		if (iter != mAssetMap.end())
		{
			delete iter->second;
			mAssetMap.erase(iter);
		}
	}

	// Gets the asset map as a reference (data can change)
	// @returns - std::unordered_map<std::string, T*>& for the reference of the asset map
	std::unordered_map<std::string, T*>& GetAssetMap() { return mAssetMap; }

private:
	// Pointer to a static AssetManager
	AssetManager* mManager;

	// Unordered map for the cached assets
	std::unordered_map<std::string, T*> mAssetMap;
};

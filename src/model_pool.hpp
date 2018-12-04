#pragma once

#include <string_view>
#include <vector>
#include <optional>
//#include <d3d12.h>

#include "util/defines.hpp"
#include "material_pool.hpp"

#undef min
#undef max
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "util/log.hpp"
#include "vertex.hpp"

struct aiScene;
struct aiNode;

namespace wr
{
	class ModelPool;

	struct Mesh
	{
		ModelPool* m_model_pool;
		MaterialHandle* m_material;
	};

	template<typename TV, typename TI = std::uint32_t>
	struct MeshData
	{
		std::vector<TV> m_vertices;
		std::optional<std::vector<TI>> m_indices;
	};

	struct Model
	{
		std::vector<Mesh*> m_meshes;
	};

	enum class ModelType
	{
		FBX,
		CUSTOM
	};

	class ModelPool
	{
	public:
		explicit ModelPool(std::size_t vertex_buffer_pool_size_in_mb,
			std::size_t index_buffer_pool_size_in_mb);
		virtual ~ModelPool() = default;

		ModelPool(ModelPool const &) = delete;
		ModelPool& operator=(ModelPool const &) = delete;
		ModelPool(ModelPool&&) = delete;
		ModelPool& operator=(ModelPool&&) = delete;

		template<typename TV, typename TI = std::uint32_t>
		[[nodiscard]] Model* Load(std::string_view path, ModelType type);
		template<typename TV, typename TI = std::uint32_t>
		[[nodiscard]] std::pair<Model*, std::vector<MaterialHandle*>> LoadWithMaterials(MaterialPool* material_pool, std::string_view path, ModelType type);
		template<typename TV, typename TI = std::uint32_t>
		[[nodiscard]] Model* LoadCustom(std::vector<MeshData<TV, TI>> meshes);

		void Destroy(Model* model);
		void Destroy(Mesh* mesh);

		virtual void Evict() = 0;
		virtual void MakeResident() = 0;

	protected:
		virtual Model* LoadFBX(std::string_view path, ModelType type) = 0;
		virtual Mesh* LoadCustom_VerticesAndIndices(void* vertices_data, std::size_t num_vertices, std::size_t vertex_size, void* indices_data, std::size_t num_indices, std::size_t index_size) = 0;
		virtual Mesh* LoadCustom_VerticesOnly(void* vertices_data, std::size_t num_vertices, std::size_t vertex_size) = 0;

		virtual void DestroyModel(Model* model) = 0;
		virtual void DestroyMesh(Mesh* mesh) = 0;

		template<typename TV, typename TI = std::uint32_t>
		int LoadNodeMeshes(const aiScene* scene, aiNode* node, Model* model);
		template<typename TV, typename TI = std::uint32_t>
		int LoadNodeMeshesWithMaterials(const aiScene* scene, aiNode* node, Model* model, std::vector<MaterialHandle*> materials);

		std::size_t m_vertex_buffer_pool_size_in_mb;
		std::size_t m_index_buffer_pool_size_in_mb;
	};

	template<typename TV, typename TI>
	Model* ModelPool::LoadCustom(std::vector<MeshData<TV, TI>> meshes)
	{
		IS_PROPER_VERTEX_CLASS(TV);

		auto model = new Model();

		for (auto& data : meshes)
		{
			if (data.m_indices.has_value())
			{
				model->m_meshes.push_back(LoadCustom_VerticesAndIndices(data.m_vertices.data(), data.m_vertices.size(), sizeof(TV), data.m_indices.value().data(), data.m_indices.value().size(), sizeof(TI)));
			}
			else
			{
				void* ptr = data.m_vertices.data();
				model->m_meshes.push_back(LoadCustom_VerticesOnly(ptr, data.m_vertices.size(), sizeof(TV)));
			}
		}

		return model;
	}

	//! Loads a model without materials
	template<typename TV, typename TI>
	Model* ModelPool::Load(std::string_view path, ModelType type)
	{
		IS_PROPER_VERTEX_CLASS(TV)

		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path.data(),
			aiProcess_Triangulate |
			aiProcess_CalcTangentSpace |
			aiProcess_JoinIdenticalVertices |
			aiProcess_OptimizeMeshes |
			aiProcess_ImproveCacheLocality |
			aiProcess_MakeLeftHanded);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			LOGW(std::string("Loading model ") +
				path.data() +
				std::string(" failed with error ") +
				importer.GetErrorString());
			return nullptr;
		}

		Model* model = new Model;

		int ret = LoadNodeMeshes<TV, TI>(scene, scene->mRootNode, model);

		if (ret == 1)
		{
			DestroyModel(model);
			return nullptr;
		}

		return model;

		return new Model();
	}

	//! Loads a model with materials
	template<typename TV, typename TI>
	std::pair<Model*, std::vector<MaterialHandle*>> ModelPool::LoadWithMaterials(MaterialPool* material_pool, std::string_view path, ModelType type)
	{
		IS_PROPER_VERTEX_CLASS(TV)

		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path.data(),
			aiProcess_Triangulate |
			aiProcess_CalcTangentSpace |
			aiProcess_JoinIdenticalVertices |
			aiProcess_OptimizeMeshes |
			aiProcess_ImproveCacheLocality |
			aiProcess_MakeLeftHanded);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			LOGW(std::string("Loading model ") +
				path.data() +
				std::string(" failed with error ") +
				importer.GetErrorString());
			return std::pair<Model*, std::vector<MaterialHandle*>>();
		}

		Model* model = new Model;
		std::vector<MaterialHandle*> material_handles;

		for (int i = 0; i < scene->mNumMaterials; ++i)
		{
			material_handles.push_back(material_pool->Load(scene->mMaterials[i]));
		}

		int ret = LoadNodeMeshesWithMaterials<TV, TI>(scene, scene->mRootNode, model, material_handles);

		if (ret == 1)
		{
			DestroyModel(model);
			return std::pair<Model*, std::vector<MaterialHandle*>>(nullptr, {});
		}

		return std::pair<Model*, std::vector<MaterialHandle*>>();
	}

	
} /* wr */

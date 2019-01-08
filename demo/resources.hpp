#pragma once

#include "wisp.hpp"

namespace resources
{

	static std::shared_ptr<wr::ModelPool> model_pool;
	static std::shared_ptr<wr::TexturePool> texture_pool;
	static std::shared_ptr<wr::MaterialPool> material_pool;

	static wr::Model* cube_model;
	static wr::Model* plane_model;
	static wr::Model* light_model;
	static wr::Model* test_model;
	static wr::MaterialHandle rusty_metal_material;
	static wr::MaterialHandle rock_material;
	static wr::MaterialHandle light_material;

	void CreateResources(wr::RenderSystem* render_system)
	{
		texture_pool = render_system->CreateTexturePool(16, 14);
		material_pool = render_system->CreateMaterialPool(8);

		// Load Texture.
		wr::TextureHandle black = texture_pool->Load("resources/materials/black.png", false, true);

		wr::TextureHandle metal_splotchy_albedo = texture_pool->Load("resources/materials/metal-splotchy-albedo.png", false, true);
		wr::TextureHandle metal_splotchy_normal = texture_pool->Load("resources/materials/metal-splotchy-normal-dx.png", false, true);
		wr::TextureHandle metal_splotchy_roughness = texture_pool->Load("resources/materials/metal-splotchy-rough.png", false, true);
		wr::TextureHandle metal_splotchy_metallic = texture_pool->Load("resources/materials/metal-splotchy-metal.png", false, true);

		wr::TextureHandle bamboo_albedo = texture_pool->Load("resources/materials/bamboo/bamboo-wood-semigloss-albedo.png", false, true);
		wr::TextureHandle bamboo_normal = texture_pool->Load("resources/materials/bamboo/bamboo-wood-semigloss-normal.png", false, true);
		wr::TextureHandle bamboo_roughness = texture_pool->Load("resources/materials/bamboo/bamboo-wood-semigloss-roughness.png", false, true);
		wr::TextureHandle bamboo_metallic = texture_pool->Load("resources/materials/bamboo/bamboo-wood-semigloss-metal.png", false, true);

		// Create Material
		light_material = material_pool->Create();

		wr::Material* light_internal = material_pool->GetMaterial(light_material.m_id);

		light_internal->SetAlbedo(black);
		light_internal->SetNormal(black);
		light_internal->SetRoughness(black);
		light_internal->SetMetallic(black);

		// Create Material
		rusty_metal_material = material_pool->Create();

		wr::Material* rusty_metal_internal = material_pool->GetMaterial(rusty_metal_material.m_id);

		rusty_metal_internal->SetAlbedo(metal_splotchy_albedo);
		rusty_metal_internal->SetNormal(metal_splotchy_normal);
		rusty_metal_internal->SetRoughness(metal_splotchy_roughness);
		rusty_metal_internal->SetMetallic(metal_splotchy_metallic);

		// Create Material
		rock_material = material_pool->Create();

		wr::Material* rock_material_internal = material_pool->GetMaterial(rock_material.m_id);

		rock_material_internal->SetAlbedo(bamboo_albedo);
		rock_material_internal->SetNormal(bamboo_normal);
		rock_material_internal->SetRoughness(bamboo_roughness);
		rock_material_internal->SetMetallic(bamboo_metallic);

		model_pool = render_system->CreateModelPool(16, 16);

		// Load Cube.
		{
			cube_model = render_system->GetSimpleShape(wr::RenderSystem::SimpleShapes::CUBE);

			for (auto& m : cube_model->m_meshes)
			{
				m.second = &rock_material;
			}
		}

		{
			plane_model = render_system->GetSimpleShape(wr::RenderSystem::SimpleShapes::PLANE);

			light_model = plane_model;

			for (auto& m : plane_model->m_meshes)
			{
				m.second = &rock_material;
			}
		}


		{
			test_model = model_pool->Load<wr::Vertex>(material_pool.get(), texture_pool.get(), "resources/models/xbot.fbx");

			for (auto& m : test_model->m_meshes)
			{
				m.second = &rusty_metal_material;
			}
		}
	}

}

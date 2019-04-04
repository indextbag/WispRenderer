#pragma once

#include "../frame_graph/frame_graph.hpp"
#include "../d3d12/d3d12_renderer.hpp"
#include "../d3d12/d3d12_functions.hpp"
#include "../d3d12/d3d12_constant_buffer_pool.hpp"
#include "../render_tasks/d3d12_deferred_main.hpp"
#include "../render_tasks/d3d12_deferred_composition.hpp"
#include "../render_tasks/d3d12_dof_bokeh_postfilter.hpp"
#include "../render_tasks/d3d12_post_processing.hpp"

#include "d3d12_raytracing_task.hpp"

namespace wr
{
	struct DoFCompositionData
	{
		d3d12::RenderTarget* out_source_rt_comp;
		d3d12::RenderTarget* out_source_bokeh_filtered;
		d3d12::RenderTarget* out_source_coc;
		d3d12::PipelineState* out_pipeline;
		ID3D12Resource* out_previous;
		DescriptorAllocator* out_allocator;
		DescriptorAllocation out_allocation;
	};

	namespace internal
	{

		template<typename T, typename T1, typename T2>
		inline void SetupDoFCompositionTask(RenderSystem& rs, FrameGraph& fg, RenderTaskHandle handle, bool resize)
		{
			auto& n_render_system = static_cast<D3D12RenderSystem&>(rs);
			auto& data = fg.GetData<DoFCompositionData>(handle);
			auto n_render_target = fg.GetRenderTarget<d3d12::RenderTarget>(handle);

			data.out_allocator = new DescriptorAllocator(n_render_system, wr::DescriptorHeapType::DESC_HEAP_TYPE_CBV_SRV_UAV);
			data.out_allocation = data.out_allocator->Allocate(5);

			auto& ps_registry = PipelineRegistry::Get();
			data.out_pipeline = ((D3D12Pipeline*)ps_registry.Find(pipelines::dof_composition))->m_native;

			auto source_rt_comp = data.out_source_rt_comp = static_cast<d3d12::RenderTarget*>(fg.GetPredecessorRenderTarget<T>());
			auto source_rt_bokeh_filtered = data.out_source_bokeh_filtered = static_cast<d3d12::RenderTarget*>(fg.GetPredecessorRenderTarget<T1>());
			auto source_coc = data.out_source_coc = static_cast<d3d12::RenderTarget*>(fg.GetPredecessorRenderTarget<T2>());

			for (auto frame_idx = 0; frame_idx < versions; frame_idx++)
			{
				// Destination
				{
					auto cpu_handle = data.out_allocation.GetDescriptorHandle(COMPILATION_EVAL(rs_layout::GetHeapLoc(params::dof_composition, params::DoFCompositionE::OUTPUT)));
					d3d12::CreateUAVFromSpecificRTV(n_render_target, cpu_handle, frame_idx, n_render_target->m_create_info.m_rtv_formats[frame_idx]);
				}
				// Source
				{
					auto cpu_handle = data.out_allocation.GetDescriptorHandle(COMPILATION_EVAL(rs_layout::GetHeapLoc(params::dof_composition, params::DoFCompositionE::SOURCE)));
					d3d12::CreateSRVFromSpecificRTV(source_rt_comp, cpu_handle, 0, source_rt_comp->m_create_info.m_rtv_formats[0]);
				}
				// Bokeh near
				{
					auto cpu_handle = data.out_allocation.GetDescriptorHandle(COMPILATION_EVAL(rs_layout::GetHeapLoc(params::dof_composition, params::DoFCompositionE::BOKEH_NEAR)));
					d3d12::CreateSRVFromSpecificRTV(source_rt_bokeh_filtered, cpu_handle, 0, source_rt_bokeh_filtered->m_create_info.m_rtv_formats[0]);
				}
				// Bokeh far
				{
					auto cpu_handle = data.out_allocation.GetDescriptorHandle(COMPILATION_EVAL(rs_layout::GetHeapLoc(params::dof_composition, params::DoFCompositionE::BOKEH_FAR)));
					d3d12::CreateSRVFromSpecificRTV(source_rt_bokeh_filtered, cpu_handle, 1, source_rt_bokeh_filtered->m_create_info.m_rtv_formats[1]);
				}
				// Cone of confusion
				{
					auto cpu_handle = data.out_allocation.GetDescriptorHandle(COMPILATION_EVAL(rs_layout::GetHeapLoc(params::dof_composition, params::DoFCompositionE::COC)));
					d3d12::CreateSRVFromSpecificRTV(source_coc, cpu_handle, frame_idx, source_coc->m_create_info.m_rtv_formats[frame_idx]);
				}
			}
		}

		template<typename T, typename T1, typename T2>
		inline void ExecuteDoFCompositionTask(RenderSystem& rs, FrameGraph& fg, SceneGraph& sg, RenderTaskHandle handle)
		{
			auto& n_render_system = static_cast<D3D12RenderSystem&>(rs);
			auto& device = n_render_system.m_device;
			auto& data = fg.GetData<DoFCompositionData>(handle);
			auto n_render_target = fg.GetRenderTarget<d3d12::RenderTarget>(handle);
			auto frame_idx = n_render_system.GetFrameIdx();
			auto cmd_list = fg.GetCommandList<d3d12::CommandList>(handle);
			const auto viewport = n_render_system.m_viewport;

			d3d12::BindComputePipeline(cmd_list, data.out_pipeline);

			auto source_rt_comp = data.out_source_rt_comp = static_cast<d3d12::RenderTarget*>(fg.GetPredecessorRenderTarget<T>());
			auto source_rt_bokeh_filtered = data.out_source_bokeh_filtered = static_cast<d3d12::RenderTarget*>(fg.GetPredecessorRenderTarget<T1>());
			auto source_coc = data.out_source_coc = static_cast<d3d12::RenderTarget*>(fg.GetPredecessorRenderTarget<T2>());

			for (auto frame_idx = 0; frame_idx < versions; frame_idx++)
			{
				// Destination
				{
					auto cpu_handle = data.out_allocation.GetDescriptorHandle(COMPILATION_EVAL(rs_layout::GetHeapLoc(params::dof_composition, params::DoFCompositionE::OUTPUT)));
					d3d12::CreateUAVFromSpecificRTV(n_render_target, cpu_handle, frame_idx, n_render_target->m_create_info.m_rtv_formats[frame_idx]);
				}
				// Source
				{
					auto cpu_handle = data.out_allocation.GetDescriptorHandle(COMPILATION_EVAL(rs_layout::GetHeapLoc(params::dof_composition, params::DoFCompositionE::SOURCE)));
					d3d12::CreateSRVFromSpecificRTV(source_rt_comp, cpu_handle, 0, source_rt_comp->m_create_info.m_rtv_formats[0]);
				}
				// Bokeh near
				{
					auto cpu_handle = data.out_allocation.GetDescriptorHandle(COMPILATION_EVAL(rs_layout::GetHeapLoc(params::dof_composition, params::DoFCompositionE::BOKEH_NEAR)));
					d3d12::CreateSRVFromSpecificRTV(source_rt_bokeh_filtered, cpu_handle, 0, source_rt_bokeh_filtered->m_create_info.m_rtv_formats[0]);
				}
				// Bokeh far
				{
					auto cpu_handle = data.out_allocation.GetDescriptorHandle(COMPILATION_EVAL(rs_layout::GetHeapLoc(params::dof_composition, params::DoFCompositionE::BOKEH_FAR)));
					d3d12::CreateSRVFromSpecificRTV(source_rt_bokeh_filtered, cpu_handle, 1, source_rt_bokeh_filtered->m_create_info.m_rtv_formats[1]);
				}
				// Cone of confusion
				{
					auto cpu_handle = data.out_allocation.GetDescriptorHandle(COMPILATION_EVAL(rs_layout::GetHeapLoc(params::dof_composition, params::DoFCompositionE::COC)));
					d3d12::CreateSRVFromSpecificRTV(source_coc, cpu_handle, frame_idx, source_coc->m_create_info.m_rtv_formats[frame_idx]);
				}
			}

			cmd_list->m_dynamic_descriptor_heaps[D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV]->StageDescriptors(0, 0, 5, data.out_allocation.GetDescriptorHandle());

			cmd_list->m_native->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::UAV(data.out_source_rt_comp->m_render_targets[frame_idx % versions]));

			d3d12::Dispatch(cmd_list,
				static_cast<int>(std::ceil(n_render_system.m_viewport.m_viewport.Width / 16.f)),
				static_cast<int>(std::ceil(n_render_system.m_viewport.m_viewport.Height / 16.f)),
				1);
		}

	} /* internal */

	template<typename T, typename T1, typename T2>
	inline void AddDoFCompositionTask(FrameGraph& frame_graph)
	{
		std::wstring name(L"DoF composition");

		RenderTargetProperties rt_properties
		{
			RenderTargetProperties::IsRenderWindow(false),
			RenderTargetProperties::Width(std::nullopt),
			RenderTargetProperties::Height(std::nullopt),
			RenderTargetProperties::ExecuteResourceState(ResourceState::UNORDERED_ACCESS),
			RenderTargetProperties::FinishedResourceState(ResourceState::COPY_SOURCE),
			RenderTargetProperties::CreateDSVBuffer(false),
			RenderTargetProperties::DSVFormat(Format::UNKNOWN),
			RenderTargetProperties::RTVFormats({ d3d12::settings::back_buffer_format}),
			RenderTargetProperties::NumRTVFormats(1),
			RenderTargetProperties::Clear(false),
			RenderTargetProperties::ClearDepth(false),
			RenderTargetProperties::ResourceName(name)
		};

		RenderTaskDesc desc;
		desc.m_setup_func = [](RenderSystem& rs, FrameGraph& fg, RenderTaskHandle handle, bool resize) {
			internal::SetupDoFCompositionTask<T, T1, T2>(rs, fg, handle, resize);
		};
		desc.m_execute_func = [](RenderSystem& rs, FrameGraph& fg, SceneGraph& sg, RenderTaskHandle handle) {
			internal::ExecuteDoFCompositionTask<T, T1, T2>(rs, fg, sg, handle);
		};
		desc.m_destroy_func = [](FrameGraph& fg, RenderTaskHandle handle, bool resize) {
		};
		desc.m_properties = rt_properties;
		desc.m_type = RenderTaskType::COMPUTE;
		desc.m_allow_multithreading = true;

		frame_graph.AddTask<DoFCompositionData>(desc);
	}

} /* wr */
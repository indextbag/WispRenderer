#pragma once

#include <dxgi1_6.h>

namespace d3d12
{

	enum class PipelineType
	{
		GRAPHICS_PIPELINE,
		COMPUTE_PIPELINE,
	};

	enum class ShaderType
	{
		VERTEX_SHADER,
		PIXEL_SHADER,
		DOMAIN_SHADER,
		GEOMETRY_SHADER,
		HULL_SHADER,
		DIRECT_COMPUTE_SHADER,
	};

	enum class CmdListType
	{
		CMD_LIST_DIRECT = (int)D3D12_COMMAND_LIST_TYPE_DIRECT,
		CMD_LIST_COMPUTE = (int)D3D12_COMMAND_LIST_TYPE_COMPUTE,
		CMD_LIST_COPY = (int)D3D12_COMMAND_LIST_TYPE_COPY,
	};

	enum HeapOptimization
	{
		SMALL_BUFFERS = 0,
		SMALL_STATIC_BUFFERS = 1,
		BIG_BUFFERS = 2,
		BIG_STATIC_BUFFERS = 3,
	};

	enum class HeapType
	{
		HEAP_DEFAULT = (int)D3D12_HEAP_TYPE_DEFAULT,
		HEAP_READBACK = (int)D3D12_HEAP_TYPE_READBACK,
		HEAP_UPLOAD = (int)D3D12_HEAP_TYPE_UPLOAD,
		HEAP_CUSTOM = (int)D3D12_HEAP_TYPE_CUSTOM,
	};

	enum class ResourceType
	{
		BUFFER,
		TEXTURE,
		RT_DS,
	};

	enum class TopologyType
	{
		TRIANGLE = (int)D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE,
		PATCH = (int)D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH,
		POINT = (int)D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT,
		LINE = (int)D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE,
	};

	enum class CullMode
	{
		CULL_FRONT = (int)D3D12_CULL_MODE_FRONT,
		CULL_BACK = (int)D3D12_CULL_MODE_BACK,
		CULL_NONE = (int)D3D12_CULL_MODE_NONE,
	};

	enum class TextureFilter
	{
		FILTER_LINEAR = (int)D3D12_FILTER_MIN_MAG_MIP_LINEAR,
		FILTER_POINT = (int)D3D12_FILTER_MIN_MAG_MIP_POINT,
		FILTER_LINEAR_POINT = (int)D3D12_FILTER_MIN_POINT_MAG_MIP_LINEAR,
		FILTER_ANISOTROPIC = (INT)D3D12_FILTER_ANISOTROPIC,
	};

	enum class SRVDimension
	{
		DIMENSION_BUFFER = (int)D3D12_SRV_DIMENSION_BUFFER,
		DIMENSION_TEXTURE1D = (int)D3D12_SRV_DIMENSION_TEXTURE1D,
		DIMENSION_TEXTURE1DARRAY = (int)D3D12_SRV_DIMENSION_TEXTURE1DARRAY,
		DIMENSION_TEXTURE2D = (int)D3D12_SRV_DIMENSION_TEXTURE2D,
		DIMENSION_TEXTURE2DARRAY = (int)D3D12_SRV_DIMENSION_TEXTURE2DARRAY,
		DIMENSION_TEXTURE2DMS = (int)D3D12_SRV_DIMENSION_TEXTURE2DMS,
		DIMENSION_TEXTURE2DMSARRAY = (int)D3D12_SRV_DIMENSION_TEXTURE2DMSARRAY,
		DIMENSION_TEXTURE3D = (int)D3D12_SRV_DIMENSION_TEXTURE3D,
		DIMENSION_TEXTURECUBE = (int)D3D12_SRV_DIMENSION_TEXTURECUBE,
		DIMENSION_TEXTURECUBEARRAY = (int)D3D12_SRV_DIMENSION_TEXTURECUBEARRAY,
	};

	enum class UAVDimension
	{
		DIMENSION_BUFFER = (int)D3D12_UAV_DIMENSION_BUFFER,
		DIMENSION_TEXTURE1D = (int)D3D12_UAV_DIMENSION_TEXTURE1D,
		DIMENSION_TEXTURE1DARRAY = (int)D3D12_UAV_DIMENSION_TEXTURE1DARRAY,
		DIMENSION_TEXTURE2D = (int)D3D12_UAV_DIMENSION_TEXTURE2D,
		DIMENSION_TEXTURE2DARRAY = (int)D3D12_UAV_DIMENSION_TEXTURE2DARRAY,
		DIMENSION_TEXTURE3D = (int)D3D12_UAV_DIMENSION_TEXTURE3D,
	};

	enum class TextureAddressMode
	{
		TAM_MIRROR_ONCE = (int)D3D12_TEXTURE_ADDRESS_MODE_MIRROR_ONCE,
		TAM_MIRROR = (int)D3D12_TEXTURE_ADDRESS_MODE_MIRROR,
		TAM_CLAMP = (int)D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
		TAM_BORDER = (int)D3D12_TEXTURE_ADDRESS_MODE_BORDER,
		TAM_WRAP = (int)D3D12_TEXTURE_ADDRESS_MODE_WRAP,
	};

	enum class BorderColor
	{
		BORDER_TRANSPARENT = (int)D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK,
		BORDER_BLACK = (int)D3D12_STATIC_BORDER_COLOR_OPAQUE_BLACK,
		BORDER_WHITE = (int)D3D12_STATIC_BORDER_COLOR_OPAQUE_WHITE,
	};

	enum class DescriptorHeapType
	{
		DESC_HEAP_TYPE_CBV_SRV_UAV = (int)D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
		DESC_HEAP_TYPE_SAMPLER = (int)D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER,
		DESC_HEAP_TYPE_RTV = (int)D3D12_DESCRIPTOR_HEAP_TYPE_RTV,
		DESC_HEAP_TYPE_DSV = (int)D3D12_DESCRIPTOR_HEAP_TYPE_DSV,
		DESC_HEAP_TYPE_NUM_TYPES = (int)D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES,
	};

	enum class ResourceState
	{
		VERTEX_AND_CONSTANT_BUFFER = (int)D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER,
		INDEX_BUFFER = (int)D3D12_RESOURCE_STATE_INDEX_BUFFER,
		PRESENT = (int)D3D12_RESOURCE_STATE_PRESENT,
		RENDER_TARGET = (int)D3D12_RESOURCE_STATE_RENDER_TARGET,
		PIXEL_SHADER_RESOURCE = (int)D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		UNORDERED_ACCESS = (int)D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
		COPY_SOURCE = (int)D3D12_RESOURCE_STATE_COPY_SOURCE,
		COPY_DEST = (int)D3D12_RESOURCE_STATE_COPY_DEST,
	};

	enum class BufferUsageFlag
	{
		INDEX_BUFFER = (int)D3D12_RESOURCE_STATE_INDEX_BUFFER,
		VERTEX_BUFFER = (int)D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER,
	};

	enum class Format
	{
		UNKNOWN = (int)DXGI_FORMAT_UNKNOWN,
		R32G32B32A32_FLOAT = (int)DXGI_FORMAT_R32G32B32A32_FLOAT,
		R32G32B32A32_UINT = (int)DXGI_FORMAT_R32G32B32A32_UINT,
		R32G32B32A32_SINT = (int)DXGI_FORMAT_R32G32B32A32_SINT,
		R32G32B32_FLOAT = (int)DXGI_FORMAT_R32G32B32_FLOAT,
		R32G32B32_UINT = (int)DXGI_FORMAT_R32G32B32_UINT,
		R32G32B32_SINT = (int)DXGI_FORMAT_R32G32B32_SINT,
		R16G16B16A16_FLOAT = (int)DXGI_FORMAT_R16G16B16A16_FLOAT,
		R16G16B16A16_UINT = (int)DXGI_FORMAT_R16G16B16A16_UINT,
		R16G16B16A16_SINT = (int)DXGI_FORMAT_R16G16B16A16_SINT,
		R16G16B16A16_UNORM = (int)DXGI_FORMAT_R16G16B16A16_UNORM,
		R16G16B16A16_SNORM = (int)DXGI_FORMAT_R16G16B16A16_SNORM,
		R32G32_FLOAT = (int)DXGI_FORMAT_R32G32_FLOAT,
		R32G32_UINT = (int)DXGI_FORMAT_R32G32_UINT,
		R32G32_SINT = (int)DXGI_FORMAT_R32G32_SINT,
		//R10G10B10_UNORM = (int)DXGI_FORMAT_R10G10B10_UNORM,
		//R10G10B10_UINT = (int)vk::Format::eA2R10G10B10UintPack32, //FIXME: Their are more vulcan variants?
		R8G8B8A8_UNORM = (int)DXGI_FORMAT_R8G8B8A8_UNORM,
		R8G8B8A8_UNORM_SRGB = (int)DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
		R8G8B8A8_SNORM = (int)DXGI_FORMAT_R8G8B8A8_SNORM,
		R8G8B8A8_UINT = (int)DXGI_FORMAT_R8G8B8A8_UINT,
		R8G8B8A8_SINT = (int)DXGI_FORMAT_R8G8B8A8_SINT,
		R16G16_FLOAT = (int)DXGI_FORMAT_R16G16_FLOAT,
		R16G16_UNORM = (int)DXGI_FORMAT_R8G8B8A8_UNORM,
		R16G16_UINT = (int)DXGI_FORMAT_R16G16_UINT,
		R16G16_SNORM = (int)DXGI_FORMAT_R16G16_SNORM,
		R16G16_SINT = (int)DXGI_FORMAT_R16G16_SINT,
		D32_FLOAT = (int)DXGI_FORMAT_D32_FLOAT,
		R32_UINT = (int)DXGI_FORMAT_R32_UINT,
		R32_SINT = (int)DXGI_FORMAT_R32_SINT,
		R32_FLOAT = (int)DXGI_FORMAT_R32_FLOAT,
		R16_UNORM = (int)DXGI_FORMAT_R16_UNORM,
		D24_UNFORM_S8_UINT = (int)DXGI_FORMAT_D24_UNORM_S8_UINT,
		//R8G8_UNORM = (int)vk::Format::eR8G8Unorm,
		//R8G8_UINT = (int)vk::Format::eR8G8Uint,
		//R8G8_SNORM = (int)vk::Format::eR8G8Snorm,
		//R8G8_SINT = (int)vk::Format::eR8G8Sint,
		//R16_FLOAT = (int)vk::Format::eR16Sfloat,
		//D16_UNORM = (int)vk::Format::eD16Unorm,
		//R16_UNORM = (int)vk::Format::eR16Unorm,
		//R16_UINT = (int)vk::Format::eR16Uint,
		//R16_SNORM = (int)vk::Format::eR16Snorm,
		//R16_SINT = (int)vk::Format::eR16Sint,
		R8_UNORM = (int)DXGI_FORMAT_R8_UNORM,
		/*R8_UINT = (int)vk::Format::eR8Uint,
		R8_SNORM = (int)vk::Format::eR8Snorm,
		R8_SINT = (int)vk::Format::eR8Sint,
		BC1_UNORM = (int)vk::Format::eBc1RgbUnormBlock, //FIXME: is this correct?
		BC1_UNORM_SRGB = (int)vk::Format::eBc1RgbSrgbBlock, //FIXME: is this correct?
		BC2_UNORM = (int)vk::Format::eBc2UnormBlock,
		BC2_UNORM_SRGB = (int)vk::Format::eBc2SrgbBlock,
		BC3_UNORM = (int)vk::Format::eBc3UnormBlock,
		BC3_UNORM_SRGB = (int)vk::Format::eBc3SrgbBlock,
		BC4_UNORM = (int)vk::Format::eBc4UnormBlock,
		BC4_SNORM = (int)vk::Format::eBc4SnormBlock,
		BC5_UNORM = (int)vk::Format::eBc5UnormBlock,
		BC5_SNORM = (int)vk::Format::eBc5SnormBlock,
		B5G6R5_UNORM = (int)vk::Format::eB5G6R5UnormPack16,
		B5G5R5A1_UNORM = (int)vk::Format::eB5G5R5A1UnormPack16,
		B8G8R8A8_UNORM = (int)vk::Format::eB8G8R8A8Unorm,
		B8G8R8A8_UNORM_SRGB = (int)vk::Format::eB8G8R8A8Srgb,
		B8G8R8A8_SNORM = (int)vk::Format::eB8G8R8A8Snorm,
		B8G8R8A8_UINT = (int)vk::Format::eB8G8R8A8Uint,
		B8G8R8A8_SINT = (int)vk::Format::eB8G8R8A8Sint,
		BC6H_UF16 = (int)vk::Format::eBc6HUfloatBlock,
		BC6H_SF16 = (int)vk::Format::eBc6HSfloatBlock,
		BC7_UNORM = (int)vk::Format::eBc7UnormBlock,
		BC7_UNORM_SRGB = (int)vk::Format::eBc7SrgbBlock,
		B4G4R4A4_UNORM = (int)vk::Format::eB4G4R4A4UnormPack16,*/
		D32_FLOAT_S8X24_UINT = (int)DXGI_FORMAT_D32_FLOAT_S8X24_UINT,
	};

} /* d3d12 */
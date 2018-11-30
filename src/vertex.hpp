#pragma once

#include <d3d12.h>
#include <vector>

#include "util/defines.hpp"

namespace wr
{

	//! Optimized 2D vertex (in clip space); -1.f to 1.f; -128 to 127
	//	2 Bpv instead of 8 Bpv (25% of original)
	struct ClipVertex2D
	{
		int8_t m_pos[2];

		static std::vector<D3D12_INPUT_ELEMENT_DESC> GetInputLayout();
	};

	//! 2D vertex
	//	8 Bpv
	struct Vertex2D 
	{
		float m_pos[2];

		static std::vector<D3D12_INPUT_ELEMENT_DESC> GetInputLayout();
	};

	//! Default Vertex
	//	32 Bpv
	struct Vertex
	{
		float m_pos[3];
		float m_uv[2];
		float m_normal[3];
		float m_tangent[3];
		float m_bitangent[3];

		static std::vector<D3D12_INPUT_ELEMENT_DESC> GetInputLayout();
	};

	//! Compressed Vertex
	//	16 Bpv instead of 32 Bpv (50% of original)
	struct CompressedVertex
	{
		uint16_t m_pos[3];
		uint16_t m_uv_x;

		uint16_t m_normal[3];
		uint16_t m_uv_y;

		uint16_t m_tangent[3];			//m_tangent[4] on GPU is m_bitangent[0]
		uint16_t m_bitangent[3];		//m_bitangent[1] and m_bitangent[2] is stored as bitangent on GPU

		//Requirement for rendering a compressed vertex (PerMesh data)
		//m_uv_start + m_uv * m_uv_length
		//m_pos_start + m_pos * m_pos_length
		//If you want to supply your own Details (like for when vertices need to be compressed the same way; if they are part of the same mesh)
		//you can set m_is_defined to 1. This does require you to fill in the start pos/uv and length (AABB for pos & uv)
		struct Details
		{
			float m_pos_start[3];
			uint32_t m_is_defined = 0;

			float m_pos_length[3];
			float m_pad_1 = 0;

			float m_uv_start[2];
			float m_uv_length[2];
		};

		static std::vector<D3D12_INPUT_ELEMENT_DESC> GetInputLayout();

		static void DetectBoundsPosition(float* pos, float* min_pos, float* max_pos);	//float[3]'s
		static void DetectBoundsUv(float* uv, float* min_uv, float* max_uv);			//float[2]'s

		static void CompressDirection(float* src, uint16_t* dst);						//float[3] and u16[3]
		static void CompressPosition(float* src, uint16_t* dst, Details& det);			//float[3] and u16[3]
		static void CompressUv(float* src, uint16_t& x, uint16_t& y, Details& det);		//float[2]

		static void CompressTBN(Vertex& in_vertex, CompressedVertex& out_vertex);
		static void CompressPositionAndUv(Vertex& in_vertex, CompressedVertex& out_vertex, Details& compression_details);

		static void CompressVertices(std::vector<Vertex>& in_vertices, std::vector<CompressedVertex>& out_vertices, Details& compression_details);

	};

	IS_PROPER_VERTEX_CLASS(Vertex)
	IS_PROPER_VERTEX_CLASS(Vertex2D)
	IS_PROPER_VERTEX_CLASS(ClipVertex2D)
	IS_PROPER_VERTEX_CLASS(CompressedVertex)

} /* wr */
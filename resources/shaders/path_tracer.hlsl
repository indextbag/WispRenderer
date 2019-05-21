#define LIGHTS_REGISTER register(t2)
#include "util.hlsl"
#include "pbr_util.hlsl"
#include "material_util.hlsl"
#include "lighting.hlsl"
#include "gi_util.hlsl"
#include "rt_structs.hlsl"

RWTexture2D<float4> output : register(u0); // xyz: reflection, a: shadow factor
ByteAddressBuffer g_indices : register(t1);
StructuredBuffer<Vertex> g_vertices : register(t3);
StructuredBuffer<Material> g_materials : register(t4);
StructuredBuffer<Offset> g_offsets : register(t5);

Texture2D g_textures[1000] : register(t10);
Texture2D gbuffer_albedo : register(t1010);
Texture2D gbuffer_normal : register(t1011);
Texture2D gbuffer_emissive : register(t1012);
Texture2D gbuffer_depth : register(t1013);
Texture2D skybox : register(t6);
TextureCube irradiance_map : register(t9);
SamplerState s0 : register(s0);

typedef BuiltInTriangleIntersectionAttributes MyAttributes;

cbuffer CameraProperties : register(b0)
{
	float4x4 inv_view;
	float4x4 inv_projection;
	float4x4 inv_vp;

	float2 padding;
	float frame_idx;
	float intensity;
};

struct Ray
{
	float3 origin;
	float3 direction;
};

#include "rt_shadow_shaders.hlsl"

[shader("raygeneration")]
void RaygenEntry()
{
	uint rand_seed = initRand((DispatchRaysIndex().x + DispatchRaysIndex().y * DispatchRaysDimensions().x), frame_idx);

	// Texture UV coordinates [0, 1]
	float2 uv = float2(DispatchRaysIndex().xy) / float2(DispatchRaysDimensions().xy - 1);

	// Screen coordinates [0, resolution] (inverted y)
	int2 screen_co = DispatchRaysIndex().xy;

	// Get g-buffer information
	float4 albedo_roughness = gbuffer_albedo[screen_co];
	float4 normal_metallic = gbuffer_normal[screen_co];
	float4 emissive_ao = gbuffer_emissive[screen_co];

	// Unpack G-Buffer
	float depth = gbuffer_depth[screen_co].x;
	float3 albedo = albedo_roughness.rgb;
	float3 wpos = unpack_position(float2(uv.x, 1.f - uv.y), depth, inv_vp);
	float3 normal = normalize(normal_metallic.xyz);
	float metallic = normal_metallic.w;
	float roughness = albedo_roughness.w;
	float3 emissive = emissive_ao.xyz;
	float ao = emissive_ao.w;

	// Do lighting
	float3 cpos = float3(inv_view[0][3], inv_view[1][3], inv_view[2][3]);
	float3 V = normalize(cpos - wpos);

	normal = lerp(normal, -normal, dot(normal, V) < 0);

	float3 result = float3(0, 0, 0);

	nextRand(rand_seed);
	const float3 rand_dir = getCosHemisphereSample(rand_seed, normal);
	const float cos_theta = cos(dot(rand_dir, normal));
	result = TraceColorRay(wpos + (EPSILON * normal), rand_dir, 0, rand_seed);
	//result += ggxIndirect(wpos, normal, normal, V, albedo, metallic, roughness, ao, rand_seed, 0);
	//result += ggxDirect(wpos, normal, normal, V, albedo, metallic, roughness, rand_seed, 0);
	//result += emissive;

	if (any(isnan(result)))
	{
		result = 0;
	}

	result = clamp(result, 0, 100);
	
	output[DispatchRaysIndex().xy] = float4(result, 1);
}

//Reflections
[shader("closesthit")]
void ReflectionHit(inout HitInfo payload, in MyAttributes attr)
{
	// Calculate the essentials
	const Offset offset = g_offsets[InstanceID()];
	const Material material = g_materials[offset.material_idx];
	const float3 hit_pos = HitWorldPosition();
	const float index_offset = offset.idx_offset;
	const float vertex_offset = offset.vertex_offset;

	// Find first index location
	const uint index_size = 4;
	const uint indices_per_triangle = 3;
	const uint triangle_idx_stride = indices_per_triangle * index_size;

	uint base_idx = PrimitiveIndex() * triangle_idx_stride;
	base_idx += index_offset * 4; // offset the start

	uint3 indices = Load3x32BitIndices(g_indices, base_idx);
	indices += float3(vertex_offset, vertex_offset, vertex_offset); // offset the start

	// Gather triangle vertices
	const Vertex v0 = g_vertices[indices.x];
	const Vertex v1 = g_vertices[indices.y];
	const Vertex v2 = g_vertices[indices.z];

	// Variables
	const float3 V = normalize(payload.origin - hit_pos);

	// Calculate actual "fragment" attributes.
	const float3 frag_pos = HitAttribute(v0.pos, v1.pos, v2.pos, attr);
	const float3 normal = normalize(HitAttribute(v0.normal, v1.normal, v2.normal, attr));
	const float3 tangent = HitAttribute(v0.tangent, v1.tangent, v2.tangent, attr);
	const float3 bitangent = HitAttribute(v0.bitangent, v1.bitangent, v2.bitangent, attr);

	float2 uv = HitAttribute(float3(v0.uv, 0), float3(v1.uv, 0), float3(v2.uv, 0), attr).xy;
	uv.y = 1.0f - uv.y;

	float mip_level = payload.depth + 1;

	OutputMaterialData output_data = InterpretMaterialDataRT(material.data,
		g_textures[material.albedo_id],
		g_textures[material.normal_id],
		g_textures[material.roughness_id],
		g_textures[material.metalicness_id],
		g_textures[material.emissive_id],
		g_textures[material.ao_id],
		mip_level,
		s0,
		uv);

	float3 albedo = output_data.albedo;
	float roughness = output_data.roughness;
	float metal = output_data.metallic;
	float3 emissive = output_data.emissive;
	float ao = output_data.ao;

	float3 N = normalize(mul(ObjectToWorld3x4(), float4(normal, 0)));
	float3 T = normalize(mul(ObjectToWorld3x4(), float4(tangent, 0)));
#define CALC_B
#ifndef CALC_B
	const float3 B = normalize(mul(ObjectToWorld3x4(), float4(bitangent, 0)));
#else
	T = normalize(T - dot(T, N) * N);
	float3 B = cross(N, T);
#endif
	const float3x3 TBN = float3x3(T, B, N);

	//float3 fN = N;
	float3 fN = normalize(mul(output_data.normal, TBN));
	fN = lerp(fN, -fN, dot(fN, V) < 0);

	// #################### GGX #####################
	nextRand(payload.seed);
	payload.color = ggxIndirect(hit_pos, fN, N, V, albedo, metal, roughness, ao, payload.seed, payload.depth + 1);
	payload.color += ggxDirect(hit_pos, fN, N, V, albedo, metal, roughness, payload.seed, payload.depth + 1);
	payload.color += emissive;
}

//Reflection skybox
[shader("miss")]
void ReflectionMiss(inout HitInfo payload)
{
	payload.color = skybox.SampleLevel(s0, SampleSphericalMap(WorldRayDirection()), 0);
}

#pragma once
#include <DirectXMath.h>
#include <limits>
#include <algorithm>

namespace wr
{

	struct Box
	{

		union
		{

			struct
			{
				DirectX::XMVECTOR m_xmin, m_xmax, m_ymin, m_ymax, m_zmin, m_zmax;
			};

			DirectX::XMVECTOR m_corners[6];

		};

		//Max bounds on each corner
		Box();

		Box(DirectX::XMVECTOR (&corners)[6]);

		DirectX::XMVECTOR &operator[](size_t i);

		//Expand bounds using position
		void ExpandFromVector(DirectX::XMVECTOR pos);

		void Expand(float(&pos)[3]);

	};

	struct AABB
	{
		
		union
		{

			struct
			{
				DirectX::XMVECTOR m_min, m_max;
			};

			DirectX::XMVECTOR m_bounds[2];

		};

		AABB();
		AABB(DirectX::XMVECTOR min, DirectX::XMVECTOR max);

		DirectX::XMVECTOR& operator[](size_t i);

		//Uses position to expand the AABB
		void Expand(DirectX::XMVECTOR pos);

		//Check if the frustum planes intersect with the AABB
		bool InFrustum(DirectX::XMVECTOR(&planes)[6]);

		//Generates AABB from transform and box
		static AABB FromTransform(Box box, DirectX::XMMATRIX transform);

	};

}
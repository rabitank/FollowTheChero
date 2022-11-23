#include "hzpch.h"
#include "Math.h"
#include <glm/gtx/matrix_decompose.hpp>

namespace Hazel
{ 



	bool Math::DecomposeTransform(const glm::mat4& TransformMat, glm::vec3& ToTranslation, glm::vec3& ToRotation, glm::vec3& ToScale)
	{

		using namespace glm;
		using T = float;

		glm::mat4 LocalMatrix(TransformMat);

		if (epsilonEqual(LocalMatrix[3][3], static_cast<T>(0), epsilon<T>()))
			return false;
		if (
			epsilonNotEqual(LocalMatrix[0][3], static_cast<T>(0), epsilon<T>()) ||
			epsilonNotEqual(LocalMatrix[1][3], static_cast<T>(0), epsilon<T>()) ||
			epsilonNotEqual(LocalMatrix[2][3], static_cast<T>(0), epsilon<T>()))
		{

				// Clear the perspective partition
				LocalMatrix[0][3] = LocalMatrix[1][3] = LocalMatrix[2][3] = static_cast<T>(0);
				LocalMatrix[3][3] = static_cast<T>(1);
		}
		
		// Next take care of translation (easy).
		ToTranslation= glm::vec3(LocalMatrix[3]);
		LocalMatrix[3]= glm::vec4(0, 0, 0, LocalMatrix[3].w);

	
		glm::vec3 Row[3];

		

		// Now get scale and shear.
		for (uint32_t i = 0; i < 3; ++i)
			for (uint32_t j = 0; j < 3; ++j)
				Row[i][j] = TransformMat[i][j];

		// Compute X scale factor and normalize first row.
		ToScale.x = length(Row[0]);// v3Length(Row[0]);
		Row[0] = detail::scale(Row[0], static_cast<T>(1));
		ToScale.y = length(Row[1]);// v3Length(Row[0]);
		Row[1] = detail::scale(Row[1], static_cast<T>(1));
		ToScale.z = length(Row[2]);// v3Length(Row[0]);
		Row[2] = detail::scale(Row[2], static_cast<T>(1));

#if 0
		// At this point, the matrix (in rows[]) is orthonormal.
		// Check for a coordinate system flip.  If the determinant
		// is -1, then negate the matrix and the scaling factors.
		glm::vec3, Pdum3;
		Pdum3 = cross(Row[1], Row[2]); // v3Cross(row[1], row[2], Pdum3);
		if (dot(Row[0], Pdum3) < 0)
		{
			for (length_t i = 0; i < 3; i++)
			{
				ToScale[i] *= static_cast<T>(-1);
				Row[i] *= static_cast<T>(-1);
			}
		}

#endif
		ToRotation.y = asin(-Row[0][2]);
		if (cos(ToRotation.y) != 0)
		{
			ToRotation.x = atan2(Row[1][2], Row[2][2]);
			ToRotation.z = atan2(Row[0][1], Row[0][0]);

		}

		else
		{
			ToRotation.x = atan2(-Row[2][0], Row[1][1]);
			ToRotation.z = 0;
		}

		return true;
	
	}

}

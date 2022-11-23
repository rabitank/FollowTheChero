#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Hazel{namespace Math
{
	bool DecomposeTransform(const glm::mat4& TransformMat, glm::vec3& ToTranslation, glm::vec3& ToRotation, glm::vec3& ToScale);
		






}
}


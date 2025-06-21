#pragma once

#include <glm/glm.hpp>

namespace Arcane
{
	struct CompVecs {
		bool operator()(const glm::vec3& a, const glm::vec3& b) const {
			return a.x < b.x && a.y < b.y && a.z < b.z;
		}

		bool operator()(const glm::vec2& a, const glm::vec2& b) const {
			return a.x < b.x && a.y < b.y;
		}

		bool operator()(const glm::ivec3& a, const glm::ivec3& b) const {
			return a.x < b.x && a.y < b.y && a.z < b.z;
		}

		bool operator()(const glm::ivec2& a, const glm::ivec2& b) const {
			return a.x < b.x && a.y < b.y;
		}
	};
}
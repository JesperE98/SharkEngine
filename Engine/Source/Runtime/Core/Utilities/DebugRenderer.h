#ifndef DEBUG_RENDERER_H
#define DEBUG_RENDERER_H

#include "Math/Vector3.h"
#include <vector>
#include <glm/glm.hpp>

namespace Shark::Graphics { class Shader; }

namespace Shark::Core {
	struct DebugLine {
		Shark::Math::Vector3 start;
		Shark::Math::Vector3 startColor;
		Shark::Math::Vector3 end;
		Shark::Math::Vector3 endColor;
	};

	class DebugRenderer {
	public:
		static DebugRenderer& Get();

		void Init();
		void AddAABB(const Shark::Math::Vector3& min, const Shark::Math::Vector3& max, const Shark::Math::Vector3& color);
		void AddLine(const Shark::Math::Vector3& start, const Shark::Math::Vector3& end, const Shark::Math::Vector3& color);
		void Render(const glm::mat4& view, const glm::mat4& projection);
		void Clear();

	private:
		DebugRenderer() = default;
		~DebugRenderer() = default;

		std::vector<DebugLine> lines;
		unsigned int VBO = 0;
		unsigned int VAO = 0;
		bool bInitialized = false;
		Shark::Graphics::Shader* shader;
	};
}

#endif // DEBUG_RENDERER_H

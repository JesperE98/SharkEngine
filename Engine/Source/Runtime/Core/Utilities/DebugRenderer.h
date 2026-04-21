#ifndef DEBUG_RENDERER_H
#define DEBUG_RENDERER_H

#include "Math/Vector3.h"
#include <vector>
#include <glm/fwd.hpp>

namespace Shark::Graphics { class Shader; }

namespace Shark::Core {
	struct DebugLine {
		Math::Vector3 start;
		Math::Vector3 startColor;
		Math::Vector3 end;
		Math::Vector3 endColor;
	};

	class DebugRenderer {
	public:
		static DebugRenderer& Get();

		void Init();
		void AddAABB(const Math::Vector3& min, const Math::Vector3& max, const Math::Vector3& color);
		void AddLine(const Math::Vector3& start, const Math::Vector3& end, const Math::Vector3& color);
		void Render(const glm::mat4& view, const glm::mat4& projection);
		void Clear();

	private:
		DebugRenderer() = default;
		~DebugRenderer() = default;

		std::vector<DebugLine> lines;
		unsigned int VBO = 0;
		unsigned int VAO = 0;
		bool bInitialized = false;
		Graphics::Shader* shader;
	};
}

#endif // DEBUG_RENDERER_H

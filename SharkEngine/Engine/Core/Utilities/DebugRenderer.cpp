#include "DebugRenderer.h"
#include "Debug.h"
#include "Managers/ShaderManager.h"
#include "Graphics/Resources/Shader.h"
#include <glad/glad.h>

namespace Shark::Core {

	using Shark::Math::Vector3;
	using Shark::Managers::ShaderManager;
	using Shark::Graphics::Shader;

	DebugRenderer& DebugRenderer::Get()
	{
		static DebugRenderer instance;
		return instance;
	}

	void DebugRenderer::Init()
	{
		if (bInitialized) return;

		shader = ShaderManager::Get().LoadShader(
			"SE_DebugLine",
			"Shaders/SE_Debug.vert.glsl",
			"Shaders/SE_Debug.frag.glsl"
		);

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3) * 2, (void*)0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3) * 2, (void*)(sizeof(Vector3)));

		glBindVertexArray(0);
		bInitialized = true;

		SE_LOG(Rendering, "DebugRenderer initialized.");
	}

	void DebugRenderer::AddAABB(const Shark::Math::Vector3& min, const Shark::Math::Vector3& max, const Shark::Math::Vector3& color)
	{
		// Bottom
		AddLine({ min.x, min.y, min.z }, { max.x, min.y, min.z }, color);
		AddLine({ max.x, min.y, min.z }, { max.x, min.y, max.z }, color);
		AddLine({ max.x, min.y, max.z }, { min.x, min.y, max.z }, color);
		AddLine({ min.x, min.y, max.z }, { min.x, min.y, min.z }, color);

		// Top
		AddLine({ min.x, max.y, min.z }, { max.x, max.y, min.z }, color);
		AddLine({ max.x, max.y, min.z }, { max.x, max.y, max.z }, color);
		AddLine({ max.x, max.y, max.z }, { min.x, max.y, max.z }, color);
		AddLine({ min.x, max.y, max.z }, { min.x, max.y, min.z }, color);

		// Vertical
		AddLine({ min.x, min.y, min.z }, { min.x, max.y, min.z }, color);
		AddLine({ max.x, min.y, min.z }, { max.x, max.y, min.z }, color);
		AddLine({ max.x, min.y, max.z }, { max.x, max.y, max.z }, color);
		AddLine({ min.x, min.y, max.z }, { min.x, max.y, max.z }, color);
	}

	void DebugRenderer::AddLine(const Shark::Math::Vector3 & start, const Shark::Math::Vector3 & end, const Shark::Math::Vector3 & color)
	{
		lines.push_back({ start, color, end, color });
	}

	void DebugRenderer::Render(const glm::mat4 & view, const glm::mat4 & projection)
	{
		if (lines.empty()) return;

		//const Shader* shader = ShaderManager::Get().GetShader("SE_DebugLine");
		if(!shader){
			SE_WARN(Rendering, "SE_DebugLine shader not found!");
			return;
		}

		shader->Use();
		shader->SetMatrix4("uView", view);
		shader->SetMatrix4("uProjection", projection);

		// Upload line data
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(
			GL_ARRAY_BUFFER,
			lines.size() * sizeof(DebugLine),
			lines.data(),
			GL_DYNAMIC_DRAW
		);

		// Disable depth test so lines are always visible through geometry (optional)
		//GLboolean depthWasEnabled;
		//glGetBooleanv(GL_DEPTH_TEST, &depthWasEnabled);
		//glDisable(GL_DEPTH_TEST);

		glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(lines.size() * 2));
		glBindVertexArray(0);
	}

	void DebugRenderer::Clear()
	{
		lines.clear();
	}
}


#include "QuadBuffer.h"

namespace Shark::Graphics {
	QuadBuffer::QuadBuffer(int w, int h) : IRenderTarget(w, h)
	{
		CreateQuad();
	}

	QuadBuffer::~QuadBuffer()
	{
		glDeleteVertexArrays(1, &m_VAO);
		glDeleteBuffers(1, &m_VBO);
	}

	void QuadBuffer::CreateQuad()
	{
		float vertices[] = {
			-1.0f,  3.0f,
			-1.0f, -1.0f,
			 3.0f, -1.0f
		};

		glGenVertexArrays(1, &m_VAO);
		glGenBuffers(1, &m_VBO);

		glBindVertexArray(m_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

		glBindVertexArray(0);
	}

	void QuadBuffer::Draw() const
	{		
		// Draw fullscreen triangle
		glBindVertexArray(m_VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);
	}

}
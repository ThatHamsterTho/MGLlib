#pragma once
#include "Test.hpp"

#include "glm/glm.hpp"

#include "Renderer.hpp"

#include "Shader.hpp"
#include "Texture.hpp"

#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"

namespace test {
	class TestBasicTexture : public Test {
		public:
			TestBasicTexture();
			~TestBasicTexture();

			void onUpdate(float deltaTime) override;
			void onRender() override;
			void onImGuiRender() override;
		private:
			float m_Color[4];
			float increment = 0.05f;

			float width;
			float height;

			glm::mat4 mvp;

			glm::mat4 model;
			glm::mat4 view;
			glm::mat4 proj;

			glm::vec3 translation1;
			glm::vec3 translation2;

			VertexArray* va;
			IndexBuffer* ib;

			Renderer* renderer;
			Shader* shader;
			Texture* texture;
	};
}
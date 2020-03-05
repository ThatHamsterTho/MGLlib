#pragma once
#include "Test.hpp"

#include "glm/glm.hpp"

#include "Renderer.hpp"

#include "Shader.hpp"
#include "Texture.hpp"

#include "VertexArrayObject.hpp"
#include "VertexBufferObject.hpp"
#include "IndexBufferObject.hpp"
#include "GenericAbstractShape.h"

// http://www.opengl-tutorial.org/beginners-tutorials/tutorial-3-matrices/

namespace test {
	class TestGAS : public Test {
		public:
			TestGAS();
			~TestGAS();

			void onUpdate() override;
			void onRender() override;
			void onImGuiRender() override;
		private:
			float m_Color[4];
			float increment = 0.05f;

			float width;
			float height;

			float scale;
			
			float Camera[3];
			float CameraLookat[3];
			float Perspective[4];

			glm::mat4 mvp;

			glm::mat4 model;
			glm::mat4 view;
			glm::mat4 proj;

			glm::mat4 orth;
			glm::mat4 pers;


			glm::vec3 translation1;
			glm::vec3 translation2;

			GenericAbstractShape<float>* GAShape;
			IndexBufferObject* ib;
			
			Renderer* renderer;
			Shader* shader;
			Texture* texture;
	};
}
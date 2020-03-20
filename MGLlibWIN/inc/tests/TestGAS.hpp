#pragma once
#include "Test.hpp"

#include "glm/glm.hpp"

#include "Core/3Drender/Camera.h"

#include "Core/Shapes/Shader.hpp"
#include "Core/Shapes/Texture.hpp"

#include "Core/Primitives/VertexArrayObject.hpp"
#include "Core/Primitives/VertexBufferObject.hpp"
#include "Core/Primitives/IndexBufferObject.hpp"
#include "Core/Shapes/GenericShape.h"

// http://www.opengl-tutorial.org/beginners-tutorials/tutorial-3-matrices/

namespace MGLlib {
	namespace test {
		class TestGAS : public Test {
			public:
				TestGAS(Window* window);
				~TestGAS();

				void onUpdate() override;
				void onRender() override;
				void onImGuiRender() override;
			private:
				float m_Color[4] = {1.0f, 1.0f, 1.0f, 1.0f};
				float increment = 0.01f;

				float width;
				float height;

				float scale;

				Camera *cam;

				glm::mat4 mvp;
				glm::mat4 model;

				glm::vec3 translation1;
				glm::vec3 translation2;

				GenericShape* GShape;
				
				Shader* shader;
				Texture* texture;
		};
	}
}
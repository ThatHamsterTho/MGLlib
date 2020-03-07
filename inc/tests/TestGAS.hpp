#pragma once
#include "Test.hpp"

#include "glm/glm.hpp"

#include "Camera.h"

#include "Shader.hpp"
#include "Texture.hpp"

#include "VertexArrayObject.hpp"
#include "VertexBufferObject.hpp"
#include "IndexBufferObject.hpp"
#include "GenericAbstractShape.h"
#include "GenericShape.h"

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
				float m_Color[4];
				float increment = 0.05f;

				float width;
				float height;

				float scale;

				Camera *cam;

				glm::mat4 mvp;
				glm::mat4 model;

				glm::vec3 translation1;
				glm::vec3 translation2;

				GenericShape* GShape;
				
				Primitives::Shader* shader;
				Primitives::Texture* texture;
		};
	}
}
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
		class TestShapeHandler : public Test {
			public:
				TestShapeHandler(Window* window);
				~TestShapeHandler();

				void onUpdate() override;
				void onRender() override;
				void onImGuiRender() override;
			private:
				float m_Color[4];
				float increment = 0.05f;

				float width;
				float height;

				float scale = 1.0f;
				float FOV = 90.0f;
				float aspect = 1.0f;
				float Z[2] = {0.1f, 20.0f};
				float up[3] = {0.0f, 1.0f, 0.0f};
				float dim[2] = {800.0f, 800.0f};

				glm::vec3 translation1;
				glm::vec3 translation2;
				glm::vec3 translation3;

				GenericShape* GSH;
				Shape* SH;	
				Rectangle* rec;	
				Circle* circ;	
				Cube* cube;	
				Texture* texture[4];
		};
	}
}
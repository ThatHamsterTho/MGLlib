#ifndef GASHEADERGUARD
#define GASHEADERGUARD

#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "VertexArray.hpp"
#include "Texture.hpp"
#include "Shader.hpp"

class GenericAbstractShape
{
	public:
		enum usage {
			STATIC_DRAW,		// one time data pass
			DYNAMIC_DRAW		// data will change a lot
		};

		GenericAbstractShape(VertexBuffer* VB, VertexBufferLayout* VBL, IndexBuffer* IB, usage type = STATIC_DRAW);
		~GenericAbstractShape();

		void Draw(void);
		void SetTexture(Texture* texture, int slot = 0);
		void RemoveTexture(void);
		void SetScale(float x, float y, float z);
		void SetShader(Shader* shader);
		Shader* GetShader(void);

	private:
		VertexArray* 	VA;
		VertexBuffer*	VB;
		IndexBuffer* 	IB;
		Texture* 		texture;
		Shader*			shader;
		bool 			use_texture;
		usage			type;
};

#endif

// NOTICE: IMPORTANT, DO NOT INCLUDE THIS FILE

#include "VertexBufferObject.hpp"
#include "IndexBufferObject.hpp"
#include "VertexArrayObject.hpp"
#include "Texture.hpp"
#include "Shader.hpp"

#include "VertexBufferStruct.hpp"
#include "Debugger.hpp"

template<typename type>
class GenericAbstractShape
{
	public:
		GenericAbstractShape(const GenericAbstractShape& GAS);
		GenericAbstractShape(Shader* shader, DrawBuffer<type>* data);
		~GenericAbstractShape();

		void Draw(void);
		void SetTexture(Texture* texture, int slot = 0);
		void RemoveTexture(void);
		void SetScale(type x, type y, type z);
		void SetShader(Shader* shader);
		Shader* GetShader(void);

	private:
		VertexArrayObject* 	VA;
		IndexBufferObject* 	IB;
		Texture* 		texture;
		Shader*			shader;
		// used for when no IBO is given
		unsigned int	Vertices;
		bool 			use_texture;
};

#ifndef CAMERAHEADERGUARD
#define CAMERAHEADERGUARD

#include "DefineDebug.h"

#include <GL/glew.h>
#include "glm/gtc/matrix_transform.hpp"

#include <vector>
#include <array>

namespace MGLlib { 

class Camera
{
	public:
		Camera(std::array<float, 3>  pos, std::array<float, 3> lookat);
		~Camera();

		void Position(glm::vec3 pos);
		void Lookat(glm::vec3 lookat);
		void SetUp(glm::vec3 up);
		void SetFOV(float FOV);
		void SetPlane(float NearPlane, float FarPlane);

		void UpdateAspectRatio(float width, float height);
		void UpdatePerspective(void);
		void UpdateCamera(void);
		bool CheckUpdated(void);

		float* getPosition(void) { return pos; }
		float* getLookat(void) { return lookat; }
		glm::mat4 getProjectionMatrix(void){return pers;}
		glm::mat4 getViewMatrix(void){return view;}
	private:
		glm::mat4 pers;
		glm::mat4 view;
		float pos[3], lookat[3], up[3];
		float FOV;
		float aspectratio;
		std::array<float, 2> Zplane;
		bool updated = true;

};

} // namespace
#endif
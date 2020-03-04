#ifndef WINDOWHEADGUARD
#define WINDOWHEADERGUARD

#include <GL/glew.h>		// sudo apt-get install libglew-dev
#include <GLFW/glfw3.h>		// sudo apt-get install libglfw3-dev
#include <GL/gl.h>			
#include <glm/glm.hpp>		// sudo apt-get install libglm-dev

namespace MGLlib {	
	class Window
	{
		private:
			GLFWwindow* window;

			// used for enabling functions
			typedef void (Window::*WindowVoidFunc)(void);

			glm::vec4 ClearColor = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
			//Camera *cam; TODO: add camera class
			//TODO: add lights

			bool use_custom_callback = true;

			void (*render)(void);
			void (*update)(void);

			void enableLighting(void);
			void enable3D(void);
			void enableAA(void);
			void enableAlpha(void);
			void enableVsync(void);

		public:
			typedef enum __features__ {
				F_3D 			= 0x0001,
				F_AA 			= 0x0002,
				F_ALPHA 		= 0x0004,
				F_LIGHTING 		= 0x0008,
				F_VSYNC 		= 0x0010,
			} feature;

			Window(const char* title, int width = 500, int height = 500, int offsetx = 100, int offsety = 100);
			~Window();

			/*! @brief enable extra window features
			*  @param[in] f enable multiple features by | them together
			*  @remark example: WindowEnableFeatures(F_3D | F_AA);
			*  call before Show();
			*/
			void WindowEnableFeatures(unsigned int f);
			/*! @brief set Render and Update callback loop
			*  @param[in] render draws objects
			*  @param[in] update changes objects values
			*/
			void SetCallback(void (*render)(void), void (*update)(void) = nullptr);
			/*! @brief use show when using the default render callback loop.
			 *	Clears screen, pollsevents and Calls set Render function after Update function.
			 *	Closes window on [x] button and ESC key.
			 * 	For other behaviour create custom own GLFW3 loop.
			*/
			void Show(void);
			void Exit(void);
			
			void SetClearColor(glm::vec4 RGBA){this->ClearColor = RGBA;}
			void getDimensions(int *width, int *height){return glfwGetWindowSize(window, width, height);}
			/*! @brief Use this when a function requires a GLFW3 Window
			 *	THIS IS NOT THE WINDOW OBJECT!
			 */
			GLFWwindow* getGLFWwindow(void){return this->window;}
	};

}
#endif
#ifndef TESTHEADERGUARD
#define TESTHEADERGUARD

#include <functional>
#include <vector>
#include "Window.h"

// for testframeworks
namespace MGLlib {
namespace test {

	class Test
	{
		public:
			Test(MGLlib::Window* window){this->window = window;}
			virtual ~Test() {}

			virtual void onUpdate() {}
			virtual void onRender() {}
			virtual void onImGuiRender() {}
		protected:
			MGLlib::Window* window;
	};

	class TestMenu: public Test
	{	
		private:
			Test*& m_CurrentTest;
			std::vector<std::pair<std::string, std::function<Test*()>>> m_Tests;

		public:
			TestMenu(Test*& currentTestPointer, MGLlib::Window* window);
			~TestMenu();

			void onUpdate() override;
			void onRender() override;
			void onImGuiRender() override;

			template<typename T>
			void AddTest(const std::string& name){
				#ifdef SHOWINFO
					printf("%-10s Adding test %s\n", "[INFO]:", name.c_str());
				#endif
				// add test type by using the lambda
				m_Tests.push_back(std::make_pair(name, [this]() {return new T(window); }));
			}
	};

}}	// namespace

#endif
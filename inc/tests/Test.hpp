#pragma once
#include <functional>
#include <vector>

// for testframeworks
namespace test {

	class Test
	{
		public:
			Test() {}
			virtual ~Test() {}

			virtual void onUpdate(float deltaTime) {}
			virtual void onRender() {}
			virtual void onImGuiRender() {}
	};

	class TestMenu: public Test
	{	
		private:
			Test*& m_CurrentTest;
			std::vector<std::pair<std::string, std::function<Test*()>>> m_Tests;

		public:
			TestMenu(Test*& currentTestPointer);
			~TestMenu();

			void onUpdate(float deltaTime) override;
			void onRender() override;
			void onImGuiRender() override;

			template<typename T>
			void AddTest(const std::string& name){
				printf("Adding test %s\n", name.c_str());
				// add test type by using the lambda
				m_Tests.push_back(std::make_pair(name, []() {return new T(); }));
			}
	};
}
#pragma once
#include "Test.hpp"

namespace test {
	class TestClearColor : public Test {
		public:
			TestClearColor();
			~TestClearColor();

			void onUpdate() override;
			void onRender() override;
			void onImGuiRender() override;
		private:
			float m_ClearColor[4];
	};
}
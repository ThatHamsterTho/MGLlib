#pragma once

namespace test {

	class Test
	{
		public:
			Test() {}
			virtual ~Test() {}

			virtual void onUpdate() {}
			virtual void onRender() {}
	};
}
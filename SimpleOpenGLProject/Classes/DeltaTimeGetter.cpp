#include "DeltaTimeGetter.hpp"

DeltaTimeGetter DeltaTimeGetter::defaultGetter() {
	return {[start = glfwGetTime()]() mutable {
		const auto oldTime = start;
		start = glfwGetTime();
		return start - oldTime;
	}};
}

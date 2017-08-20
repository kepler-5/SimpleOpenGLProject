#ifndef DeltaTimeGetter_hpp
#define DeltaTimeGetter_hpp

#include <functional>

struct DeltaTimeGetter {
	DeltaTimeGetter(std::function<double()> in_getter = nullptr)
	: getter(std::move(in_getter))
	{}
	double getDeltaTime() const { return getter(); }
	
	static DeltaTimeGetter defaultGetter();
private:
	std::function<double()> getter;
};

#endif /* DeltaTimeGetter_hpp */

#include "InspectCOS2.h"

InspectCOS2::InspectCOS2()
{
	Init();
}

InspectCOS2::~InspectCOS2()
{
}

void InspectCOS2::Create(int width, int height, bool isInit)
{
	Inspect::Create(width, height, isInit);
}

void InspectCOS2::Release()
{

}

void InspectCOS2::Run()
{
	SpotInspect();
	ScInspect();
}
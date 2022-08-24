#include "RopeSimulationFactory.h"


//const FName IRopeSimulationFactoryClassProvider::FeatureName = TEXT("RopeSimulationFactoryClassProvider");

namespace URopeSimulationFactoryConsoleVariables
{
	TAutoConsoleVariable<FString> CVarDefaultRopeSimulationFactoryClass(
		TEXT("p.Cloth.DefaultClothingSimulationFactoryClass"),
		TEXT("PDRopeSimulationFactory"),  // Chaos is the default provider when Chaos Cloth is enabled
		TEXT("The class name of the default clothing simulation factory.\n")
		TEXT("Known providers are:\n")
		TEXT("sjw plugin\n")
		, ECVF_Cheat);
}
//
// TSubclassOf<class UClothingSimulationFactory> UClothingSimulationFactory::GetDefaultClothingSimulationFactoryClass()
// {
// 	TSubclassOf<UClothingSimulationFactory> DefaultClothingSimulationFactoryClass = nullptr;
//
// 	const FString DefaultClothingSimulationFactoryClassName = ClothingSimulationFactoryConsoleVariables::CVarDefaultClothingSimulationFactoryClass.GetValueOnAnyThread();
//
// 	const TArray<IClothingSimulationFactoryClassProvider*> ClassProviders = IModularFeatures::Get().GetModularFeatureImplementations<IClothingSimulationFactoryClassProvider>(IClothingSimulationFactoryClassProvider::FeatureName);
// 	for (const auto& ClassProvider : ClassProviders)
// 	{
// 		if (ClassProvider)
// 		{
// 			const TSubclassOf<UClothingSimulationFactory> ClothingSimulationFactoryClass = ClassProvider->GetClothingSimulationFactoryClass();
// 			if (ClothingSimulationFactoryClass.Get() != nullptr)
// 			{
// 				// Always set the default to the last non null factory class (in case the search for the cvar doesn't yield any results)
// 				DefaultClothingSimulationFactoryClass = ClothingSimulationFactoryClass;
//
// 				// Early exit if the cvar string matches
// 				if (ClothingSimulationFactoryClass->GetName() == DefaultClothingSimulationFactoryClassName)
// 				{
// 					break;
// 				}
// 			}
// 		}
// 	}
// 	return DefaultClothingSimulationFactoryClass;
// }
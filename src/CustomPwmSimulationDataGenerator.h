#ifndef CUSTOMPWM_SIMULATION_DATA_GENERATOR
#define CUSTOMPWM_SIMULATION_DATA_GENERATOR

#include <SimulationChannelDescriptor.h>
#include <string>
class CustomPwmAnalyzerSettings;

class CustomPwmSimulationDataGenerator
{
public:
	CustomPwmSimulationDataGenerator();
	~CustomPwmSimulationDataGenerator();

	void Initialize( U32 simulation_sample_rate, CustomPwmAnalyzerSettings* settings );
	U32 GenerateSimulationData( U64 newest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channel );

protected:
	CustomPwmAnalyzerSettings* mSettings;
	U32 mSimulationSampleRateHz;

protected:
	void CreateSerialByte();
	std::string mSerialText;
	U32 mStringIndex;

	SimulationChannelDescriptor mSerialSimulationData;

};
#endif //CUSTOMPWM_SIMULATION_DATA_GENERATOR
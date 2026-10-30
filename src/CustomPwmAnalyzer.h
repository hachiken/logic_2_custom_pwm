#ifndef CUSTOM_PWM_ANALYZER_H
#define CUSTOM_PWM_ANALYZER_H

#include <Analyzer.h>
#include "CustomPwmAnalyzerSettings.h"
#include "CustomPwmAnalyzerResults.h"
#include "CustomPwmSimulationDataGenerator.h"

class CustomPwmAnalyzer : public Analyzer2
{
public:
    CustomPwmAnalyzer();
    virtual ~CustomPwmAnalyzer();

    virtual void SetupResults();
    virtual void WorkerThread();

    virtual U32 GenerateSimulationData( U64 newest_sample_requested, U32 sample_rate,
                                        SimulationChannelDescriptor** simulation_channels );
    virtual U32 GetMinimumSampleRateHz();

    virtual const char* GetAnalyzerName() const;
    virtual bool NeedsRerun();

#pragma warning( push )
#pragma warning( disable : 4251 ) // warning C4251: 'CustomPwmAnalyzer::<...>' : class <...> needs to have dll-interface to be used by clients of class

protected:
    std::auto_ptr< CustomPwmAnalyzerSettings > mSettings;
    std::auto_ptr< CustomPwmAnalyzerResults > mResults;

    AnalyzerChannelData* mInput;

    U64 mCurrentSample;
    BitState mLastState;
    U64 mLastTransitionSample;

    void MeasurePulse();
};

#pragma warning( pop )

#endif // CUSTOM_PWM_ANALYZER_H
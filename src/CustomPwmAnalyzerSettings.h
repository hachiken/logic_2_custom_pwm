#ifndef CUSTOMPWM_ANALYZER_SETTINGS
#define CUSTOMPWM_ANALYZER_SETTINGS

#include <AnalyzerSettings.h>
#include <AnalyzerTypes.h>

class CustomPwmAnalyzerSettings : public AnalyzerSettings
{
public:
	CustomPwmAnalyzerSettings();
	virtual ~CustomPwmAnalyzerSettings();

	virtual bool SetSettingsFromInterfaces();
	void UpdateInterfacesFromSettings();
	virtual void LoadSettings( const char* settings );
	virtual const char* SaveSettings();

	
	Channel mInputChannel;
	U32 mBitRate;

protected:
	AnalyzerSettingInterfaceChannel	mInputChannelInterface;
	AnalyzerSettingInterfaceInteger	mBitRateInterface;
};

#endif //CUSTOMPWM_ANALYZER_SETTINGS

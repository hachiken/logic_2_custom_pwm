#include "CustomPwmAnalyzer.h"
#include "CustomPwmAnalyzerSettings.h"
#include <AnalyzerChannelData.h>

CustomPwmAnalyzer::CustomPwmAnalyzer()
:   Analyzer2(),
    mSettings( new CustomPwmAnalyzerSettings() ),
    mResults( new CustomPwmAnalyzerResults( this, mSettings.get() ) ),
    mInput( NULL ),
    mCurrentSample( 0 ),
    mLastState( BIT_LOW ),
    mLastTransitionSample( 0 )
{
    SetAnalyzerSettings( mSettings.get() );
    UseFrameV2(); // Обязательно для поддержки HLA
}

CustomPwmAnalyzer::~CustomPwmAnalyzer()
{
    KillThread();
}

void CustomPwmAnalyzer::SetupResults()
{
    mResults.reset( new CustomPwmAnalyzerResults( this, mSettings.get() ) );
    SetAnalyzerResults( mResults.get() );
    mResults->AddChannelBubblesWillAppearOn( mSettings->mInputChannel );
}

void CustomPwmAnalyzer::WorkerThread()
{
    mInput = GetAnalyzerChannelData( mSettings->mInputChannel );

    // Начинаем с первого перехода в HIGH, чтобы измерить первый импульс
    if( mInput->GetBitState() == BIT_LOW )
        mInput->AdvanceToNextEdge();

    mLastState = BIT_HIGH;
    mLastTransitionSample = mInput->GetSampleNumber();

    for( ; ; )
    {
        // Ждём следующего фронта (падение в LOW)
        mInput->AdvanceToNextEdge();
        U64 risingEdgeSample = mInput->GetSampleNumber();
        U64 highDurationSamples = risingEdgeSample - mLastTransitionSample;

        // Преобразуем длительность в секунды, используя частоту дискретизации
        double sampleRate = (double)GetSampleRate();
        double highDurationSec = (double)highDurationSamples / sampleRate;

        // Создаём FrameV2, который будет доступен для HLA
        FrameV2 frameV2;
        frameV2.AddDouble( "high_duration", highDurationSec );
        frameV2.AddInteger( "high_duration_samples", highDurationSamples );

        // Также создаём обычный Frame для отображения "пузырька" на таймлайне
        Frame frame;
        frame.mStartingSampleInclusive = mLastTransitionSample;
        frame.mEndingSampleInclusive = risingEdgeSample;
        frame.mData1 = (U64)(highDurationSec * 1000000.0); // в микросекундах для отображения
        frame.mFlags = 0;
        frame.mType = 0;

        mResults->AddFrame( frame );
        mResults->AddFrameV2( frameV2, "pulse", mLastTransitionSample, risingEdgeSample );
        mResults->CommitResults();

        ReportProgress( risingEdgeSample );

        // Теперь ждём следующего фронта (подъём в HIGH)
        mInput->AdvanceToNextEdge();
        U64 fallingEdgeSample = mInput->GetSampleNumber();
        U64 lowDurationSamples = fallingEdgeSample - risingEdgeSample;

        // Можно также добавить информацию о низком уровне, если нужно
        // FrameV2 lowFrameV2;
        // lowFrameV2.AddFloat( "low_duration", (double)lowDurationSamples / sampleRate );

        mLastTransitionSample = fallingEdgeSample;
        mLastState = BIT_HIGH;

        // Проверяем, не завершилась ли передача (например, по длинной паузе)
        // В вашем протоколе нет явного конца, поэтому мы просто продолжаем
        // Пакет завершается после 24 бит, но это обрабатывается в HLA.
    }
}

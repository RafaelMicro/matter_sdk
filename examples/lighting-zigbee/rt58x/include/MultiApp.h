#pragma once

#include "timers.h"
#include "task.h"
#include "init_device_environment.h"
#include "EnhancedFlashDataset.h"
#include "hosal_rf.h"

class MultiApp
{
public:
    enum class CurrentProtocol : uint8_t
    {
        kNone    = 0,
        kZigbee  = 1,
        kMatter  = 2,
    };

    static MultiApp & GetInstance();

    void Init();

    CurrentProtocol GetCurrentProtocol() const { return mCurrentProtocol; }
    void SendBleMsgToZigbeeApp(uint8_t* buf, uint8_t len);
    void SetCurrentProtocol(CurrentProtocol protocol);
    void SuspendMatterTask(void);
    void SuspendZigbeeTask(void);
    void ResumeZigbeeTask(void);

private:
    friend MultiApp & GetMultiApp(void);
    static MultiApp sInstance;
    MultiApp() = default;

    static void Button0HoldTimerCallback(TimerHandle_t xTimer);
    static void ButtonIsrHandler(uint32_t pin, void * isr_param);

    TimerHandle_t mButton0HoldTimer        = NULL;
    CurrentProtocol mCurrentProtocol       = CurrentProtocol::kNone;
    uint8_t mZigbeeTaskRunning = 0;
    uint8_t mMatterTaskRunning = 0;
};

inline MultiApp & GetMultiApp(void)
{
    return MultiApp::sInstance;
}

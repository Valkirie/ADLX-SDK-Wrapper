#include "../SDK/ADLXHelper/Windows/Cpp/ADLXHelper.h"
#include "../SDK/Include/I3DSettings.h"
#include "../SDK/Include/I3DSettings1.h"
#include "../SDK/Include/I3DSettings2.h"
#include "../SDK/Include/I3DSettings3.h"
#include "../SDK/Include/IPerformanceMonitoring3.h"
#include "../SDK/Include/IDisplaySettings.h"
#include "../SDK/Include/IDisplays.h"
#include "../SDK/Include/IGPUTuning.h"
#include "../SDK/Include/IGPUPresetTuning.h"
#include "../SDK/Include/IGPUManualGFXTuning.h"
#include "../SDK/Include/IPerformanceMonitoring.h"
#include "../SDK/Include/IDisplays.h"
#include "../SDK/Include/IDesktops.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
using namespace std;

#define ADLX_Wrapper _declspec(dllexport)

extern "C" {
    using namespace adlx;
    static ADLXHelper g_ADLXHelp;

    struct AdlxTelemetryData
    {
        // GPU Usage
        bool gpuUsageSupported = false;
        double gpuUsageValue;

        // GPU Core Frequency
        bool gpuClockSpeedSupported = false;
        double gpuClockSpeedValue;

        // GPU VRAM Frequency
        bool gpuVRAMClockSpeedSupported = false;
        double gpuVRAMClockSpeedValue;

        // GPU Core Temperature
        bool gpuTemperatureSupported = false;
        double gpuTemperatureValue;

        // GPU Hotspot Temperature
        bool gpuHotspotTemperatureSupported = false;
        double gpuHotspotTemperatureValue;

        // GPU Power
        bool gpuPowerSupported = false;
        double gpuPowerValue;

        // Fan Speed
        bool gpuFanSpeedSupported = false;
        double gpuFanSpeedValue;

        // VRAM Usage
        bool gpuVramSupported = false;
        double gpuVramValue;

        // GPU Voltage
        bool gpuVoltageSupported = false;
        double gpuVoltageValue;

        // GPU TBP
        bool gpuTotalBoardPowerSupported = false;
        double gpuTotalBoardPowerValue;

        // GPU Intake Temperature
        bool gpuIntakeTemperatureSupported = false;
        double gpuIntakeTemperatureValue;

        // GPU Memory Temperature
        bool gpuMemoryTemperatureSupported = false;
        double gpuMemoryTemperatureValue;

        // NPU Frequency
        bool npuFrequencySupported = false;
        double npuFrequencyValue;

        // NPU Activity
        bool npuActivitySupported = false;
        double npuActivityValue;

        // GPU Fan Duty
        bool gpuFanDutySupported = false;
        double gpuFanDutyValue;

        // GPU Shared Memory
        bool gpuSharedMemorySupported = false;
        double gpuSharedMemoryValue;

        // Framerate
        long timeStamp = 0;
        int fpsData = 0;
    };

    struct AdlxTelemetryInterfaces
    {
        IADLXGPUMetricsSupportPtr gpuMetricsSupport;
        IADLXGPUMetricsSupport1Ptr gpuMetricsSupport1;
        IADLXGPUMetricsSupport2Ptr gpuMetricsSupport2;
        IADLXGPUMetricsSupport3Ptr gpuMetricsSupport3;
        IADLXGPUMetricsPtr gpuMetrics;
        IADLXGPUMetrics1Ptr gpuMetrics1;
        IADLXGPUMetrics2Ptr gpuMetrics2;
        IADLXGPUMetrics3Ptr gpuMetrics3;
        adlx_uint gpuMetricsSupportVersion = 0;
        adlx_uint gpuMetricsVersion = 0;
    };

    ADLX_Wrapper void ResolveTelemetryInterfaces(IADLXGPUMetricsSupportPtr gpuMetricsSupport, IADLXGPUMetricsPtr gpuMetrics, AdlxTelemetryInterfaces* telemetryInterfaces)
    {
        if (telemetryInterfaces == nullptr)
            return;

        telemetryInterfaces->gpuMetricsSupport = gpuMetricsSupport;
        telemetryInterfaces->gpuMetricsSupport1 = IADLXGPUMetricsSupport1Ptr(gpuMetricsSupport);
        telemetryInterfaces->gpuMetricsSupport2 = IADLXGPUMetricsSupport2Ptr(gpuMetricsSupport);
        telemetryInterfaces->gpuMetricsSupport3 = IADLXGPUMetricsSupport3Ptr(gpuMetricsSupport);
        telemetryInterfaces->gpuMetrics = gpuMetrics;
        telemetryInterfaces->gpuMetrics1 = IADLXGPUMetrics1Ptr(gpuMetrics);
        telemetryInterfaces->gpuMetrics2 = IADLXGPUMetrics2Ptr(gpuMetrics);
        telemetryInterfaces->gpuMetrics3 = IADLXGPUMetrics3Ptr(gpuMetrics);

        if (telemetryInterfaces->gpuMetricsSupport3 != nullptr)
            telemetryInterfaces->gpuMetricsSupportVersion = 3;
        else if (telemetryInterfaces->gpuMetricsSupport2 != nullptr)
            telemetryInterfaces->gpuMetricsSupportVersion = 2;
        else if (telemetryInterfaces->gpuMetricsSupport1 != nullptr)
            telemetryInterfaces->gpuMetricsSupportVersion = 1;

        if (telemetryInterfaces->gpuMetrics3 != nullptr)
            telemetryInterfaces->gpuMetricsVersion = 3;
        else if (telemetryInterfaces->gpuMetrics2 != nullptr)
            telemetryInterfaces->gpuMetricsVersion = 2;
        else if (telemetryInterfaces->gpuMetrics1 != nullptr)
            telemetryInterfaces->gpuMetricsVersion = 1;
    }

    ADLX_Wrapper bool IntializeAdlx(char* adlxVersion, adlx_uint nameLength)
    {
        ADLX_RESULT res = ADLX_FAIL;

        if (adlxVersion == nullptr || nameLength == 0)
            return false;

        try
        {
            // Initialize ADLX
            res = g_ADLXHelp.Initialize();

            // Code to run when the DLL is loaded
            if (ADLX_SUCCEEDED(res))
            {
                const char* dispName = g_ADLXHelp.QueryVersion();

                // Make sure not to overflow the provided buffer
                if (dispName != nullptr)
                {
                    strncpy(adlxVersion, dispName, nameLength);
                    adlxVersion[nameLength - 1] = '\0'; // Ensure null-termination
                }
                else
                {
                    adlxVersion[0] = '\0';
                }
#if DEBUG
                AllocConsole();
                freopen("CONOUT$", "w", stdout); // Redirect stdout to the console
                freopen("CONOUT$", "w", stderr); // Optional: Redirect stderr to the console
                std::ios::sync_with_stdio(); // Sync C++ and C standard streams
#endif
                cout << "ADLX Initialized" << endl;
            }
        }
        catch (const std::exception& e)
        {
            g_ADLXHelp.Terminate();
        }
        catch (...)
        {
            g_ADLXHelp.Terminate();
        }

        return ADLX_SUCCEEDED(res);
    }

    ADLX_Wrapper bool InitializeWithIncompatibleDriver(char* adlxVersion, adlx_uint nameLength)
    {
        ADLX_RESULT res = ADLX_FAIL;

        if (adlxVersion == nullptr || nameLength == 0)
            return false;

        try
        {
            // Initialize ADLX
            res = g_ADLXHelp.InitializeWithIncompatibleDriver();

            // Code to run when the DLL is loaded
            if (ADLX_SUCCEEDED(res))
            {
                const char* dispName = g_ADLXHelp.QueryVersion();

                // Make sure not to overflow the provided buffer
                if (dispName != nullptr)
                {
                    strncpy(adlxVersion, dispName, nameLength);
                    adlxVersion[nameLength - 1] = '\0'; // Ensure null-termination
                }
                else
                {
                    adlxVersion[0] = '\0';
                }
#if DEBUG
                AllocConsole();
                freopen("CONOUT$", "w", stdout); // Redirect stdout to the console
                freopen("CONOUT$", "w", stderr); // Optional: Redirect stderr to the console
                std::ios::sync_with_stdio(); // Sync C++ and C standard streams
#endif
                cout << "ADLX Initialized" << endl;
            }
        }
        catch (const std::exception& e)
        {
            g_ADLXHelp.Terminate();
        }
        catch (...)
        {
            g_ADLXHelp.Terminate();
        }

        return ADLX_SUCCEEDED(res);
    }

    ADLX_Wrapper bool CloseAdlx()
    {
        ADLX_RESULT res = ADLX_FAIL;

        // Terminate ADLX
        res = g_ADLXHelp.Terminate();
        if (ADLX_SUCCEEDED(res))
        {
            cout << "ADLX Terminated" << endl;
        }

        return ADLX_SUCCEEDED(res);
    }

    ADLX_Wrapper ADLX_RESULT GetNumberOfDisplays(adlx_uint* displayNum)
    {
        ADLX_RESULT res = ADLX_FAIL;

        // Get display service
        IADLXDisplayServicesPtr displayService;
        res = g_ADLXHelp.GetSystemServices()->GetDisplaysServices(&displayService);
        if (ADLX_SUCCEEDED(res))
        {
            // Get display list
            IADLXDisplayListPtr displayList;
            res = displayService->GetDisplays(&displayList);
            if (ADLX_SUCCEEDED(res) && !displayList->Empty())
            {
                *displayNum = displayList->Size();

                cout << "======== GetNumberOfDisplays ========" << endl;
                cout << "displayNum: " << displayNum << endl;
            }
        }

        return res;
    }

    ADLX_Wrapper ADLX_RESULT GetDisplayName(adlx_uint index, char* Name, adlx_uint nameLength)
    {
        // Define return code
        ADLX_RESULT res = ADLX_FAIL;

        // Get display service
        IADLXDisplayServicesPtr displayService;
        res = g_ADLXHelp.GetSystemServices()->GetDisplaysServices(&displayService);
        if (ADLX_SUCCEEDED(res))
        {
            // Get display list
            IADLXDisplayListPtr displayList;
            res = displayService->GetDisplays(&displayList);
            if (ADLX_SUCCEEDED(res))
            {
                // Inspect for the display at position index in the list
                IADLXDisplayPtr display;
                res = displayList->At(index, &display);
                if (ADLX_SUCCEEDED(res))
                {
                    cout << "======== GetDisplayName ========" << endl;

                    const char* dispName;
                    res = display->Name(&dispName);
                    if (ADLX_SUCCEEDED(res))
                    {
                        cout << "Display name: " << dispName << endl;

                        // Make sure not to overflow the provided buffer
                        strncpy(Name, dispName, nameLength);
                        Name[nameLength - 1] = '\0'; // Ensure null-termination
                    }

                    adlx_uint manufacturerID;
                    res = display->ManufacturerID(&manufacturerID);
                    if (ADLX_SUCCEEDED(res))
                        cout << "Manufacturer id: " << manufacturerID << endl;

                    const char* edid;
                    res = display->EDID(&edid);
                    if (ADLX_SUCCEEDED(res))
                        std::cout << "EDID: " << edid << std::endl;

                    adlx_size id;
                    res = display->UniqueId(&id);
                    if (ADLX_SUCCEEDED(res))
                        std::cout << "UniqueId: " << id << std::endl;
                }
            }
        }

        return res;
    }

    ADLX_Wrapper ADLX_RESULT GetNumberOfGPU(adlx_uint* gpuNum)
    {
        ADLX_RESULT res = ADLX_FAIL;

        // Get GPUs
        IADLXGPUListPtr gpus;
        res = g_ADLXHelp.GetSystemServices()->GetGPUs(&gpus);
        if (ADLX_SUCCEEDED(res) && !gpus->Empty())
        {
            *gpuNum = gpus->Size();

            cout << "======== GetNumberOfGPU ========" << endl;
            cout << "gpuNum: " << gpuNum << endl;
        }

        return res;
    }

    ADLX_Wrapper ADLX_RESULT GetDisplayGPU(adlx_uint index, adlx_int* uniqueId)
    {
        // Define return code
        ADLX_RESULT res = ADLX_FAIL;

        // Get display service
        IADLXDisplayServicesPtr displayService;
        res = g_ADLXHelp.GetSystemServices()->GetDisplaysServices(&displayService);
        if (ADLX_SUCCEEDED(res))
        {
            // Get display list
            IADLXDisplayListPtr displayList;
            res = displayService->GetDisplays(&displayList);
            if (ADLX_SUCCEEDED(res))
            {
                cout << "======== GetDisplayGPU ========" << endl;

                // Inspect for the display at position index in the list
                IADLXDisplayPtr display;
                res = displayList->At(index, &display);
                if (ADLX_SUCCEEDED(res))
                {
                    IADLXGPUPtr gpu;
                    res = display->GetGPU(&gpu);

                    if (ADLX_SUCCEEDED(res))
                    {
                        res = gpu->UniqueId(uniqueId);
                        if (ADLX_SUCCEEDED(res))
                        {
                            std::cout << "UniqueId: " << uniqueId << std::endl;
                        }
                    }
                }
            }
        }

        return res;
    }

    ADLX_Wrapper ADLX_RESULT GetGPUIndex(adlx_int uniqueId, adlx_uint* index)
    {
        // Define return code
        ADLX_RESULT res = ADLX_FAIL;

        // Get GPUs
        IADLXGPUListPtr gpus;
        res = g_ADLXHelp.GetSystemServices()->GetGPUs(&gpus);
        if (ADLX_SUCCEEDED(res) && !gpus->Empty())
        {
            for (adlx_uint idx = 0; idx < gpus->Size(); idx++)
            {
                // Get GPU interface
                IADLXGPUPtr gpuInfo;
                res = gpus->At(idx, &gpuInfo);
                if (ADLX_SUCCEEDED(res))
                {
                    adlx_int id;
                    res = gpuInfo->UniqueId(&id);

                    if (id == uniqueId)
                    {
                        *index = idx;
                        break;
                    }
                }
            }
        }

        return res;
    }

    ADLX_Wrapper bool HasIntegerScalingSupport(int displayIdx)
    {
        // Define return code
        ADLX_RESULT res = ADLX_FAIL;
        bool result = false;

        // Get display service
        IADLXDisplayServicesPtr displayService;
        res = g_ADLXHelp.GetSystemServices()->GetDisplaysServices(&displayService);
        if (ADLX_SUCCEEDED(res))
        {
            // Get display list
            IADLXDisplayListPtr displayList;
            res = displayService->GetDisplays(&displayList);
            if (ADLX_SUCCEEDED(res))
            {
                // Inspect for the display at position displayIdx in the list
                IADLXDisplayPtr display;
                res = displayList->At(displayIdx, &display);
                if (ADLX_SUCCEEDED(res))
                {
                    IADLXDisplayIntegerScalingPtr displayIntegerScaling;
                    res = displayService->GetIntegerScaling(display, &displayIntegerScaling);
                    if (ADLX_SUCCEEDED(res))
                    {
                        adlx_bool supported = false;
                        displayIntegerScaling->IsSupported(&supported);
                        result = supported;
                    }
                }
            }
        }

        return result;
    }

    ADLX_Wrapper bool GetIntegerScaling(int displayIdx)
    {
        // Define return code
        ADLX_RESULT res = ADLX_FAIL;
        bool result = false;

        // Get display service
        IADLXDisplayServicesPtr displayService;
        res = g_ADLXHelp.GetSystemServices()->GetDisplaysServices(&displayService);
        if (ADLX_SUCCEEDED(res))
        {
            // Get display list
            IADLXDisplayListPtr displayList;
            res = displayService->GetDisplays(&displayList);
            if (ADLX_SUCCEEDED(res))
            {
                // Inspect for the display at position displayIdx in the list
                IADLXDisplayPtr display;
                res = displayList->At(displayIdx, &display);
                if (ADLX_SUCCEEDED(res))
                {
                    IADLXDisplayIntegerScalingPtr displayIntegerScaling;
                    res = displayService->GetIntegerScaling(display, &displayIntegerScaling);
                    if (ADLX_SUCCEEDED(res))
                    {
                        adlx_bool enabled = false;
                        res = displayIntegerScaling->IsEnabled(&enabled);
                        result = enabled;
                    }
                }
            }
        }

        return result;
    }

    ADLX_Wrapper bool SetIntegerScaling(int displayIdx, bool enabled)
    {
        // Define return code
        ADLX_RESULT res = ADLX_FAIL;
        bool result = false;

        // Get display service
        IADLXDisplayServicesPtr displayService;
        res = g_ADLXHelp.GetSystemServices()->GetDisplaysServices(&displayService);
        if (ADLX_SUCCEEDED(res))
        {
            // Get display list
            IADLXDisplayListPtr displayList;
            res = displayService->GetDisplays(&displayList);
            if (ADLX_SUCCEEDED(res))
            {
                // Inspect for the display at position displayIdx in the list
                IADLXDisplayPtr display;
                res = displayList->At(displayIdx, &display);
                if (ADLX_SUCCEEDED(res))
                {
                    IADLXDisplayIntegerScalingPtr displayIntegerScaling;
                    res = displayService->GetIntegerScaling(display, &displayIntegerScaling);
                    if (ADLX_SUCCEEDED(res))
                    {
                        ADLX_RESULT res = displayIntegerScaling->SetEnabled(enabled);
                        result = ADLX_SUCCEEDED(res);
                    }
                }
            }
        }

        return result;
    }

    ADLX_Wrapper bool HasGPUScalingSupport(int displayIdx)
    {
        // Define return code
        ADLX_RESULT res = ADLX_FAIL;
        bool result = false;

        // Get display service
        IADLXDisplayServicesPtr displayService;
        res = g_ADLXHelp.GetSystemServices()->GetDisplaysServices(&displayService);
        if (ADLX_SUCCEEDED(res))
        {
            // Get display list
            IADLXDisplayListPtr displayList;
            res = displayService->GetDisplays(&displayList);
            if (ADLX_SUCCEEDED(res))
            {
                // Inspect for the display at position displayIdx in the list
                IADLXDisplayPtr display;
                res = displayList->At(displayIdx, &display);
                if (ADLX_SUCCEEDED(res))
                {
                    IADLXDisplayGPUScalingPtr gpuScaling;
                    res = displayService->GetGPUScaling(display, &gpuScaling);
                    if (ADLX_SUCCEEDED(res))
                    {
                        adlx_bool supported = false;
                        gpuScaling->IsSupported(&supported);
                        result = supported;
                    }
                }
            }
        }

        return result;
    }

    ADLX_Wrapper bool GetGPUScaling(int displayIdx)
    {
        // Define return code
        ADLX_RESULT res = ADLX_FAIL;
        bool result = false;

        // Get display service
        IADLXDisplayServicesPtr displayService;
        res = g_ADLXHelp.GetSystemServices()->GetDisplaysServices(&displayService);
        if (ADLX_SUCCEEDED(res))
        {
            // Get display list
            IADLXDisplayListPtr displayList;
            res = displayService->GetDisplays(&displayList);
            if (ADLX_SUCCEEDED(res))
            {
                // Inspect for the display at position displayIdx in the list
                IADLXDisplayPtr display;
                res = displayList->At(displayIdx, &display);
                if (ADLX_SUCCEEDED(res))
                {
                    IADLXDisplayGPUScalingPtr gpuScaling;
                    res = displayService->GetGPUScaling(display, &gpuScaling);
                    if (ADLX_SUCCEEDED(res))
                    {
                        adlx_bool enabled = false;
                        res = gpuScaling->IsEnabled(&enabled);
                        result = enabled;
                    }
                }
            }
        }

        return result;
    }

    ADLX_Wrapper bool SetGPUScaling(int displayIdx, bool enabled)
    {
        // Define return code
        ADLX_RESULT res = ADLX_FAIL;
        bool result = false;

        // Get display service
        IADLXDisplayServicesPtr displayService;
        res = g_ADLXHelp.GetSystemServices()->GetDisplaysServices(&displayService);
        if (ADLX_SUCCEEDED(res))
        {
            // Get display list
            IADLXDisplayListPtr displayList;
            res = displayService->GetDisplays(&displayList);
            if (ADLX_SUCCEEDED(res))
            {
                // Inspect for the display at position displayIdx in the list
                IADLXDisplayPtr display;
                res = displayList->At(displayIdx, &display);
                if (ADLX_SUCCEEDED(res))
                {
                    IADLXDisplayGPUScalingPtr gpuScaling;
                    res = displayService->GetGPUScaling(display, &gpuScaling);
                    if (ADLX_SUCCEEDED(res))
                    {
                        ADLX_RESULT res = gpuScaling->SetEnabled(enabled);
                        result = ADLX_SUCCEEDED(res);
                    }
                }
            }
        }

        return result;
    }

    ADLX_Wrapper bool HasScalingModeSupport(int displayIdx)
    {
        // Define return code
        ADLX_RESULT res = ADLX_FAIL;
        bool result = false;

        // Get display service
        IADLXDisplayServicesPtr displayService;
        res = g_ADLXHelp.GetSystemServices()->GetDisplaysServices(&displayService);
        if (ADLX_SUCCEEDED(res))
        {
            // Get display list
            IADLXDisplayListPtr displayList;
            res = displayService->GetDisplays(&displayList);
            if (ADLX_SUCCEEDED(res))
            {
                // Inspect for the display at position displayIdx in the list
                IADLXDisplayPtr display;
                res = displayList->At(displayIdx, &display);
                if (ADLX_SUCCEEDED(res))
                {
                    IADLXDisplayScalingModePtr scalingMode;
                    res = displayService->GetScalingMode(display, &scalingMode);
                    if (ADLX_SUCCEEDED(res))
                    {
                        adlx_bool supported = false;
                        scalingMode->IsSupported(&supported);
                        result = supported;
                    }
                }
            }
        }

        return result;
    }

    ADLX_Wrapper int GetScalingMode(int displayIdx)
    {
        // Define return code
        ADLX_RESULT res = ADLX_FAIL;
        int result = -1;

        // Get display service
        IADLXDisplayServicesPtr displayService;
        res = g_ADLXHelp.GetSystemServices()->GetDisplaysServices(&displayService);
        if (ADLX_SUCCEEDED(res))
        {
            // Get display list
            IADLXDisplayListPtr displayList;
            res = displayService->GetDisplays(&displayList);
            if (ADLX_SUCCEEDED(res))
            {
                // Inspect for the display at position displayIdx in the list
                IADLXDisplayPtr display;
                res = displayList->At(displayIdx, &display);
                if (ADLX_SUCCEEDED(res))
                {
                    IADLXDisplayScalingModePtr scalingMode;
                    res = displayService->GetScalingMode(display, &scalingMode);
                    if (ADLX_SUCCEEDED(res))
                    {
                        ADLX_SCALE_MODE scaleMode;
                        res = scalingMode->GetMode(&scaleMode);
                        result = scaleMode;
                    }
                }
            }
        }

        return result;
    }

    ADLX_Wrapper bool SetScalingMode(int displayIdx, int mode)
    {
        // Define return code
        ADLX_RESULT res = ADLX_FAIL;
        bool result = false;

        // Get display service
        IADLXDisplayServicesPtr displayService;
        res = g_ADLXHelp.GetSystemServices()->GetDisplaysServices(&displayService);
        if (ADLX_SUCCEEDED(res))
        {
            // Get display list
            IADLXDisplayListPtr displayList;
            res = displayService->GetDisplays(&displayList);
            if (ADLX_SUCCEEDED(res))
            {
                // Inspect for the display at position displayIdx in the list
                IADLXDisplayPtr display;
                res = displayList->At(displayIdx, &display);
                if (ADLX_SUCCEEDED(res))
                {
                    IADLXDisplayScalingModePtr scalingMode;
                    res = displayService->GetScalingMode(display, &scalingMode);
                    if (ADLX_SUCCEEDED(res))
                    {
                        ADLX_RESULT res = scalingMode->SetMode((ADLX_SCALE_MODE)mode);
                        result = ADLX_SUCCEEDED(res);
                    }
                }
            }
        }

        return result;
    }

    ADLX_Wrapper bool HasAFMFSupport()
    {
        // Define return code
        ADLX_RESULT res = ADLX_FAIL;
        bool result = false;

        // Get 3DSettings service
        IADLX3DSettingsServicesPtr d3dSettingSrv;
        res = g_ADLXHelp.GetSystemServices()->Get3DSettingsServices(&d3dSettingSrv);
        if (ADLX_SUCCEEDED(res))
        {
            // Get 3DSettings service
            IADLX3DSettingsServices1Ptr d3dSettingSrv1(d3dSettingSrv);

            // Get AFMF interface
            IADLX3DAMDFluidMotionFramesPtr d3dAFMF;
            res = d3dSettingSrv1->GetAMDFluidMotionFrames(&d3dAFMF);
            if (ADLX_SUCCEEDED(res))
            {
                adlx_bool supported = false;
                d3dAFMF->IsSupported(&supported);
                result = supported;
            }
        }

        return result;
    }

    ADLX_Wrapper bool GetAFMF()
    {
        // Define return code
        ADLX_RESULT res = ADLX_FAIL;
        bool result = false;

        // Get 3DSettings service
        IADLX3DSettingsServicesPtr d3dSettingSrv;
        res = g_ADLXHelp.GetSystemServices()->Get3DSettingsServices(&d3dSettingSrv);
        if (ADLX_SUCCEEDED(res))
        {
            // Get 3DSettings service
            IADLX3DSettingsServices1Ptr d3dSettingSrv1(d3dSettingSrv);

            // Get AFMF interface
            IADLX3DAMDFluidMotionFramesPtr d3dAFMF;
            res = d3dSettingSrv1->GetAMDFluidMotionFrames(&d3dAFMF);
            if (ADLX_SUCCEEDED(res))
            {
                adlx_bool supported = false;
                d3dAFMF->IsSupported(&supported);

                if (supported)
                {
                    adlx_bool enabled = false;
                    d3dAFMF->IsEnabled(&enabled);
                    result = enabled;
                }
            }
        }

        return result;
    }

    ADLX_Wrapper bool SetAFMF(bool enable)
    {
        // Define return code
        ADLX_RESULT res = ADLX_FAIL;
        bool result = false;

        // Get 3DSettings service
        IADLX3DSettingsServicesPtr d3dSettingSrv;
        res = g_ADLXHelp.GetSystemServices()->Get3DSettingsServices(&d3dSettingSrv);
        if (ADLX_SUCCEEDED(res))
        {
            // Get 3DSettings service
            IADLX3DSettingsServices1Ptr d3dSettingSrv1(d3dSettingSrv);

            // Get AFMF interface
            IADLX3DAMDFluidMotionFramesPtr d3dAFMF;
            res = d3dSettingSrv1->GetAMDFluidMotionFrames(&d3dAFMF);
            if (ADLX_SUCCEEDED(res))
            {
                adlx_bool supported = false;
                d3dAFMF->IsSupported(&supported);

                if (supported)
                {
                    ADLX_RESULT res = d3dAFMF->SetEnabled(enable);
                    result = ADLX_SUCCEEDED(res);
                }
            }
        }

        return result;
    }

    ADLX_Wrapper bool GetAFMFAlgorithmSupport()
    {
        ADLX_RESULT res = ADLX_FAIL;
        bool result = false;

        IADLX3DSettingsServicesPtr d3dSettingSrv;
        res = g_ADLXHelp.GetSystemServices()->Get3DSettingsServices(&d3dSettingSrv);
        if (ADLX_SUCCEEDED(res))
        {
            IADLX3DSettingsServices1Ptr d3dSettingSrv1(d3dSettingSrv);
            IADLX3DAMDFluidMotionFramesPtr d3dAFMF;
            res = d3dSettingSrv1->GetAMDFluidMotionFrames(&d3dAFMF);
            if (ADLX_SUCCEEDED(res))
            {
                IADLX3DAMDFluidMotionFrames1Ptr d3dAFMF1(d3dAFMF);
                if (d3dAFMF1 != nullptr)
                {
                    adlx_bool supported = false;
                    d3dAFMF1->IsSupportedAlgorithm(&supported);
                    result = supported;
                }
            }
        }

        return result;
    }

    ADLX_Wrapper int GetAFMFAlgorithm()
    {
        ADLX_RESULT res = ADLX_FAIL;
        int result = -1;

        IADLX3DSettingsServicesPtr d3dSettingSrv;
        res = g_ADLXHelp.GetSystemServices()->Get3DSettingsServices(&d3dSettingSrv);
        if (ADLX_SUCCEEDED(res))
        {
            IADLX3DSettingsServices1Ptr d3dSettingSrv1(d3dSettingSrv);
            IADLX3DAMDFluidMotionFramesPtr d3dAFMF;
            res = d3dSettingSrv1->GetAMDFluidMotionFrames(&d3dAFMF);
            if (ADLX_SUCCEEDED(res))
            {
                IADLX3DAMDFluidMotionFrames1Ptr d3dAFMF1(d3dAFMF);
                if (d3dAFMF1 != nullptr)
                {
                    ADLX_AFMF_ALGORITHM algorithm;
                    res = d3dAFMF1->GetAlgorithm(&algorithm);
                    if (ADLX_SUCCEEDED(res))
                        result = algorithm;
                }
            }
        }

        return result;
    }

    ADLX_Wrapper bool SetAFMFAlgorithm(int algorithm)
    {
        ADLX_RESULT res = ADLX_FAIL;
        bool result = false;

        IADLX3DSettingsServicesPtr d3dSettingSrv;
        res = g_ADLXHelp.GetSystemServices()->Get3DSettingsServices(&d3dSettingSrv);
        if (ADLX_SUCCEEDED(res))
        {
            IADLX3DSettingsServices1Ptr d3dSettingSrv1(d3dSettingSrv);
            IADLX3DAMDFluidMotionFramesPtr d3dAFMF;
            res = d3dSettingSrv1->GetAMDFluidMotionFrames(&d3dAFMF);
            if (ADLX_SUCCEEDED(res))
            {
                IADLX3DAMDFluidMotionFrames1Ptr d3dAFMF1(d3dAFMF);
                if (d3dAFMF1 != nullptr)
                {
                    res = d3dAFMF1->SetAlgorithm((ADLX_AFMF_ALGORITHM)algorithm);
                    result = ADLX_SUCCEEDED(res);
                }
            }
        }

        return result;
    }

    ADLX_Wrapper int GetAFMFSearchMode()
    {
        ADLX_RESULT res = ADLX_FAIL;
        int result = -1;

        IADLX3DSettingsServicesPtr d3dSettingSrv;
        res = g_ADLXHelp.GetSystemServices()->Get3DSettingsServices(&d3dSettingSrv);
        if (ADLX_SUCCEEDED(res))
        {
            IADLX3DSettingsServices1Ptr d3dSettingSrv1(d3dSettingSrv);
            IADLX3DAMDFluidMotionFramesPtr d3dAFMF;
            res = d3dSettingSrv1->GetAMDFluidMotionFrames(&d3dAFMF);
            if (ADLX_SUCCEEDED(res))
            {
                IADLX3DAMDFluidMotionFrames1Ptr d3dAFMF1(d3dAFMF);
                if (d3dAFMF1 != nullptr)
                {
                    ADLX_AFMF_SEARCH_MODE_TYPE mode;
                    res = d3dAFMF1->GetSearchMode(&mode);
                    if (ADLX_SUCCEEDED(res))
                        result = mode;
                }
            }
        }

        return result;
    }

    ADLX_Wrapper bool SetAFMFSearchMode(int mode)
    {
        ADLX_RESULT res = ADLX_FAIL;
        bool result = false;

        IADLX3DSettingsServicesPtr d3dSettingSrv;
        res = g_ADLXHelp.GetSystemServices()->Get3DSettingsServices(&d3dSettingSrv);
        if (ADLX_SUCCEEDED(res))
        {
            IADLX3DSettingsServices1Ptr d3dSettingSrv1(d3dSettingSrv);
            IADLX3DAMDFluidMotionFramesPtr d3dAFMF;
            res = d3dSettingSrv1->GetAMDFluidMotionFrames(&d3dAFMF);
            if (ADLX_SUCCEEDED(res))
            {
                IADLX3DAMDFluidMotionFrames1Ptr d3dAFMF1(d3dAFMF);
                if (d3dAFMF1 != nullptr)
                {
                    res = d3dAFMF1->SetSearchMode((ADLX_AFMF_SEARCH_MODE_TYPE)mode);
                    result = ADLX_SUCCEEDED(res);
                }
            }
        }

        return result;
    }

    ADLX_Wrapper int GetAFMFPerformanceMode()
    {
        ADLX_RESULT res = ADLX_FAIL;
        int result = -1;

        IADLX3DSettingsServicesPtr d3dSettingSrv;
        res = g_ADLXHelp.GetSystemServices()->Get3DSettingsServices(&d3dSettingSrv);
        if (ADLX_SUCCEEDED(res))
        {
            IADLX3DSettingsServices1Ptr d3dSettingSrv1(d3dSettingSrv);
            IADLX3DAMDFluidMotionFramesPtr d3dAFMF;
            res = d3dSettingSrv1->GetAMDFluidMotionFrames(&d3dAFMF);
            if (ADLX_SUCCEEDED(res))
            {
                IADLX3DAMDFluidMotionFrames1Ptr d3dAFMF1(d3dAFMF);
                if (d3dAFMF1 != nullptr)
                {
                    ADLX_AFMF_PERFORMANCE_MODE_TYPE perfMode;
                    res = d3dAFMF1->GetPerformanceMode(&perfMode);
                    if (ADLX_SUCCEEDED(res))
                        result = perfMode;
                }
            }
        }

        return result;
    }

    ADLX_Wrapper bool SetAFMFPerformanceMode(int mode)
    {
        ADLX_RESULT res = ADLX_FAIL;
        bool result = false;

        IADLX3DSettingsServicesPtr d3dSettingSrv;
        res = g_ADLXHelp.GetSystemServices()->Get3DSettingsServices(&d3dSettingSrv);
        if (ADLX_SUCCEEDED(res))
        {
            IADLX3DSettingsServices1Ptr d3dSettingSrv1(d3dSettingSrv);
            IADLX3DAMDFluidMotionFramesPtr d3dAFMF;
            res = d3dSettingSrv1->GetAMDFluidMotionFrames(&d3dAFMF);
            if (ADLX_SUCCEEDED(res))
            {
                IADLX3DAMDFluidMotionFrames1Ptr d3dAFMF1(d3dAFMF);
                if (d3dAFMF1 != nullptr)
                {
                    res = d3dAFMF1->SetPerformanceMode((ADLX_AFMF_PERFORMANCE_MODE_TYPE)mode);
                    result = ADLX_SUCCEEDED(res);
                }
            }
        }

        return result;
    }

    ADLX_Wrapper int GetAFMFFastMotionResponse()
    {
        ADLX_RESULT res = ADLX_FAIL;
        int result = -1;

        IADLX3DSettingsServicesPtr d3dSettingSrv;
        res = g_ADLXHelp.GetSystemServices()->Get3DSettingsServices(&d3dSettingSrv);
        if (ADLX_SUCCEEDED(res))
        {
            IADLX3DSettingsServices1Ptr d3dSettingSrv1(d3dSettingSrv);
            IADLX3DAMDFluidMotionFramesPtr d3dAFMF;
            res = d3dSettingSrv1->GetAMDFluidMotionFrames(&d3dAFMF);
            if (ADLX_SUCCEEDED(res))
            {
                IADLX3DAMDFluidMotionFrames1Ptr d3dAFMF1(d3dAFMF);
                if (d3dAFMF1 != nullptr)
                {
                    ADLX_AFMF_FAST_MOTION_RESP response;
                    res = d3dAFMF1->GetFastMotionResponse(&response);
                    if (ADLX_SUCCEEDED(res))
                        result = response;
                }
            }
        }

        return result;
    }

    ADLX_Wrapper bool SetAFMFFastMotionResponse(int response)
    {
        ADLX_RESULT res = ADLX_FAIL;
        bool result = false;

        IADLX3DSettingsServicesPtr d3dSettingSrv;
        res = g_ADLXHelp.GetSystemServices()->Get3DSettingsServices(&d3dSettingSrv);
        if (ADLX_SUCCEEDED(res))
        {
            IADLX3DSettingsServices1Ptr d3dSettingSrv1(d3dSettingSrv);
            IADLX3DAMDFluidMotionFramesPtr d3dAFMF;
            res = d3dSettingSrv1->GetAMDFluidMotionFrames(&d3dAFMF);
            if (ADLX_SUCCEEDED(res))
            {
                IADLX3DAMDFluidMotionFrames1Ptr d3dAFMF1(d3dAFMF);
                if (d3dAFMF1 != nullptr)
                {
                    res = d3dAFMF1->SetFastMotionResponse((ADLX_AFMF_FAST_MOTION_RESP)response);
                    result = ADLX_SUCCEEDED(res);
                }
            }
        }

        return result;
    }

    ADLX_Wrapper bool HasRSRSupport()
    {
        // Define return code
        ADLX_RESULT res = ADLX_FAIL;
        bool result = false;

        // Get 3DSettings service
        IADLX3DSettingsServicesPtr d3dSettingSrv;
        res = g_ADLXHelp.GetSystemServices()->Get3DSettingsServices(&d3dSettingSrv);
        if (ADLX_SUCCEEDED(res))
        {
            // Get RadeonSuperResolution interface
            IADLX3DRadeonSuperResolutionPtr rsr;
            res = d3dSettingSrv->GetRadeonSuperResolution(&rsr);
            if (ADLX_SUCCEEDED(res))
            {
                adlx_bool supported = false;
                rsr->IsSupported(&supported);
                result = supported;
            }
        }

        return result;
    }

    ADLX_Wrapper bool GetRSR()
    {
        // Define return code
        ADLX_RESULT res = ADLX_FAIL;
        bool result = false;

        // Get 3DSettings service
        IADLX3DSettingsServicesPtr d3dSettingSrv;
        res = g_ADLXHelp.GetSystemServices()->Get3DSettingsServices(&d3dSettingSrv);
        if (ADLX_SUCCEEDED(res))
        {
            // Get RadeonSuperResolution interface
            IADLX3DRadeonSuperResolutionPtr rsr;
            res = d3dSettingSrv->GetRadeonSuperResolution(&rsr);
            if (ADLX_SUCCEEDED(res))
            {
                adlx_bool supported = false;
                rsr->IsSupported(&supported);

                if (supported)
                {
                    adlx_bool enabled = false;
                    rsr->IsEnabled(&enabled);
                    result = enabled;
                }
            }
        }

        return result;
    }

    ADLX_Wrapper bool SetRSR(bool enable)
    {
        // Define return code
        ADLX_RESULT res = ADLX_FAIL;
        bool result = false;

        // Get 3DSettings service
        IADLX3DSettingsServicesPtr d3dSettingSrv;
        res = g_ADLXHelp.GetSystemServices()->Get3DSettingsServices(&d3dSettingSrv);
        if (ADLX_SUCCEEDED(res))
        {
            // Get RadeonSuperResolution interface
            IADLX3DRadeonSuperResolutionPtr rsr;
            res = d3dSettingSrv->GetRadeonSuperResolution(&rsr);
            if (ADLX_SUCCEEDED(res))
            {
                adlx_bool supported = false;
                rsr->IsSupported(&supported);

                if (supported)
                {
                    ADLX_RESULT res = rsr->SetEnabled(enable);
                    result = ADLX_SUCCEEDED(res);
                }
            }
        }

        return result;
    }

    ADLX_Wrapper int GetRSRSharpness()
    {
        // Define return code
        ADLX_RESULT res = ADLX_FAIL;
        int result = -1;

        // Get 3DSettings service
        IADLX3DSettingsServicesPtr d3dSettingSrv;
        res = g_ADLXHelp.GetSystemServices()->Get3DSettingsServices(&d3dSettingSrv);
        if (ADLX_SUCCEEDED(res))
        {
            // Get RadeonSuperResolution interface
            IADLX3DRadeonSuperResolutionPtr rsr;
            res = d3dSettingSrv->GetRadeonSuperResolution(&rsr);
            if (ADLX_SUCCEEDED(res))
            {
                adlx_bool supported = false;
                rsr->IsSupported(&supported);

                if (supported)
                {
                    adlx_int currentSharpness;
                    rsr->GetSharpness(&currentSharpness);
                    result = currentSharpness;
                }
            }
        }

        return result;
    }

    ADLX_Wrapper bool SetRSRSharpness(int sharpness)
    {
        // Define return code
        ADLX_RESULT res = ADLX_FAIL;
        bool result = false;

        // Get 3DSettings service
        IADLX3DSettingsServicesPtr d3dSettingSrv;
        res = g_ADLXHelp.GetSystemServices()->Get3DSettingsServices(&d3dSettingSrv);
        if (ADLX_SUCCEEDED(res))
        {
            // Get RadeonSuperResolution interface
            IADLX3DRadeonSuperResolutionPtr rsr;
            res = d3dSettingSrv->GetRadeonSuperResolution(&rsr);
            if (ADLX_SUCCEEDED(res))
            {
                adlx_bool supported = false;
                rsr->IsSupported(&supported);

                if (supported)
                {
                    ADLX_RESULT adlx_result = rsr->SetSharpness(sharpness);
                    result = ADLX_SUCCEEDED(adlx_result);
                }
            }
        }

        return result;
    }

    ADLX_Wrapper bool HasSharpenDesktopSupport(int GPU)
    {
        ADLX_RESULT res = ADLX_FAIL;
        bool result = false;

        IADLXGPUListPtr gpus;
        res = g_ADLXHelp.GetSystemServices()->GetGPUs(&gpus);
        if (ADLX_SUCCEEDED(res) && !gpus->Empty())
        {
            IADLX3DSettingsServicesPtr d3dSettingSrv;
            res = g_ADLXHelp.GetSystemServices()->Get3DSettingsServices(&d3dSettingSrv);
            if (ADLX_SUCCEEDED(res))
            {
                IADLX3DSettingsServices2Ptr d3dSettingSrv2(d3dSettingSrv);
                if (d3dSettingSrv2 != nullptr)
                {
                    IADLXGPUPtr gpuInfo;
                    res = gpus->At(GPU, &gpuInfo);
                    if (ADLX_SUCCEEDED(res))
                    {
                        IADLX3DImageSharpenDesktopPtr sharpenDesktop;
                        res = d3dSettingSrv2->GetImageSharpenDesktop(gpuInfo, &sharpenDesktop);
                        if (ADLX_SUCCEEDED(res))
                        {
                            adlx_bool supported = false;
                            sharpenDesktop->IsSupported(&supported);
                            result = supported;
                        }
                    }
                }
            }
        }

        return result;
    }

    ADLX_Wrapper bool GetSharpenDesktop(int GPU)
    {
        ADLX_RESULT res = ADLX_FAIL;
        bool result = false;

        IADLXGPUListPtr gpus;
        res = g_ADLXHelp.GetSystemServices()->GetGPUs(&gpus);
        if (ADLX_SUCCEEDED(res) && !gpus->Empty())
        {
            IADLX3DSettingsServicesPtr d3dSettingSrv;
            res = g_ADLXHelp.GetSystemServices()->Get3DSettingsServices(&d3dSettingSrv);
            if (ADLX_SUCCEEDED(res))
            {
                IADLX3DSettingsServices2Ptr d3dSettingSrv2(d3dSettingSrv);
                if (d3dSettingSrv2 != nullptr)
                {
                    IADLXGPUPtr gpuInfo;
                    res = gpus->At(GPU, &gpuInfo);
                    if (ADLX_SUCCEEDED(res))
                    {
                        IADLX3DImageSharpenDesktopPtr sharpenDesktop;
                        res = d3dSettingSrv2->GetImageSharpenDesktop(gpuInfo, &sharpenDesktop);
                        if (ADLX_SUCCEEDED(res))
                        {
                            adlx_bool enabled = false;
                            res = sharpenDesktop->IsEnabled(&enabled);
                            result = enabled;
                        }
                    }
                }
            }
        }

        return result;
    }

    ADLX_Wrapper bool SetSharpenDesktop(int GPU, bool enable)
    {
        ADLX_RESULT res = ADLX_FAIL;
        bool result = false;

        IADLXGPUListPtr gpus;
        res = g_ADLXHelp.GetSystemServices()->GetGPUs(&gpus);
        if (ADLX_SUCCEEDED(res) && !gpus->Empty())
        {
            IADLX3DSettingsServicesPtr d3dSettingSrv;
            res = g_ADLXHelp.GetSystemServices()->Get3DSettingsServices(&d3dSettingSrv);
            if (ADLX_SUCCEEDED(res))
            {
                IADLX3DSettingsServices2Ptr d3dSettingSrv2(d3dSettingSrv);
                if (d3dSettingSrv2 != nullptr)
                {
                    IADLXGPUPtr gpuInfo;
                    res = gpus->At(GPU, &gpuInfo);
                    if (ADLX_SUCCEEDED(res))
                    {
                        IADLX3DImageSharpenDesktopPtr sharpenDesktop;
                        res = d3dSettingSrv2->GetImageSharpenDesktop(gpuInfo, &sharpenDesktop);
                        if (ADLX_SUCCEEDED(res))
                        {
                            res = sharpenDesktop->SetEnabled(enable);
                            result = ADLX_SUCCEEDED(res);
                        }
                    }
                }
            }
        }

        return result;
    }

    ADLX_Wrapper bool GetAntiLag(int GPU)
    {
        // Define return code
        ADLX_RESULT res = ADLX_FAIL;
        bool result = false;

        // Get GPUs
        IADLXGPUListPtr gpus;
        res = g_ADLXHelp.GetSystemServices()->GetGPUs(&gpus);
        if (ADLX_SUCCEEDED(res) && !gpus->Empty())
        {
            // Get 3DSettings service
            IADLX3DSettingsServicesPtr d3dSettingSrv;
            res = g_ADLXHelp.GetSystemServices()->Get3DSettingsServices(&d3dSettingSrv);
            if (ADLX_SUCCEEDED(res))
            {
                // Get GPU interface
                IADLXGPUPtr gpuInfo;
                res = gpus->At(GPU, &gpuInfo);
                if (ADLX_SUCCEEDED(res))
                {
                    // Get Anti Lag interface
                    IADLX3DAntiLagPtr anti;
                    res = d3dSettingSrv->GetAntiLag(gpuInfo, &anti);
                    if (ADLX_SUCCEEDED(res))
                    {
                        adlx_bool enabled = false;
                        anti->IsEnabled(&enabled);
                        result = enabled;
                    }
                }
            }
        }

        return result;
    }

    ADLX_Wrapper bool SetAntiLag(int GPU, bool enable)
    {
        // Define return code
        ADLX_RESULT res = ADLX_FAIL;
        bool result = false;

        // Get GPUs
        IADLXGPUListPtr gpus;
        res = g_ADLXHelp.GetSystemServices()->GetGPUs(&gpus);
        if (ADLX_SUCCEEDED(res) && !gpus->Empty())
        {
            // Get 3DSettings service
            IADLX3DSettingsServicesPtr d3dSettingSrv;
            res = g_ADLXHelp.GetSystemServices()->Get3DSettingsServices(&d3dSettingSrv);
            if (ADLX_SUCCEEDED(res))
            {
                // Get GPU interface
                IADLXGPUPtr gpuInfo;
                res = gpus->At(GPU, &gpuInfo);
                if (ADLX_SUCCEEDED(res))
                {
                    // Get Anti Lag interface
                    IADLX3DAntiLagPtr anti;
                    res = d3dSettingSrv->GetAntiLag(gpuInfo, &anti);
                    if (ADLX_SUCCEEDED(res))
                    {
                        ADLX_RESULT res = anti->SetEnabled(enable);
                        result = ADLX_SUCCEEDED(res);
                    }
                }
            }
        }

        return result;
    }

    ADLX_Wrapper bool GetBoost(int GPU)
    {
        // Define return code
        ADLX_RESULT res = ADLX_FAIL;
        bool result = false;

        // Get GPUs
        IADLXGPUListPtr gpus;
        res = g_ADLXHelp.GetSystemServices()->GetGPUs(&gpus);
        if (ADLX_SUCCEEDED(res) && !gpus->Empty())
        {
            // Get 3DSettings service
            IADLX3DSettingsServicesPtr d3dSettingSrv;
            res = g_ADLXHelp.GetSystemServices()->Get3DSettingsServices(&d3dSettingSrv);
            if (ADLX_SUCCEEDED(res))
            {
                // Get GPU interface
                IADLXGPUPtr gpuInfo;
                res = gpus->At(GPU, &gpuInfo);
                if (ADLX_SUCCEEDED(res))
                {
                    // Get Boost interface
                    IADLX3DBoostPtr boost;
                    res = d3dSettingSrv->GetBoost(gpuInfo, &boost);
                    if (ADLX_SUCCEEDED(res))
                    {
                        adlx_bool isEnabled = false;
                        boost->IsEnabled(&isEnabled);
                        result = isEnabled;
                    }
                }
            }
        }

        return result;
    }

    ADLX_Wrapper bool SetBoost(int GPU, bool enable)
    {
        // Define return code
        ADLX_RESULT res = ADLX_FAIL;
        bool result = false;

        // Get GPUs
        IADLXGPUListPtr gpus;
        res = g_ADLXHelp.GetSystemServices()->GetGPUs(&gpus);
        if (ADLX_SUCCEEDED(res) && !gpus->Empty())
        {
            // Get 3DSettings service
            IADLX3DSettingsServicesPtr d3dSettingSrv;
            res = g_ADLXHelp.GetSystemServices()->Get3DSettingsServices(&d3dSettingSrv);
            if (ADLX_SUCCEEDED(res))
            {
                // Get GPU interface
                IADLXGPUPtr gpuInfo;
                res = gpus->At(GPU, &gpuInfo);
                if (ADLX_SUCCEEDED(res))
                {
                    // Get Boost interface
                    IADLX3DBoostPtr boost;
                    res = d3dSettingSrv->GetBoost(gpuInfo, &boost);
                    if (ADLX_SUCCEEDED(res))
                    {
                        ADLX_RESULT res = boost->SetEnabled(enable);
                        result = ADLX_SUCCEEDED(res);
                    }
                }
            }
        }

        return result;
    }

    ADLX_Wrapper int GetBoostResolution(int GPU)
    {
        // Define return code
        ADLX_RESULT res = ADLX_FAIL;
        int result = -1;

        // Get GPUs
        IADLXGPUListPtr gpus;
        res = g_ADLXHelp.GetSystemServices()->GetGPUs(&gpus);
        if (ADLX_SUCCEEDED(res) && !gpus->Empty())
        {
            // Get 3DSettings service
            IADLX3DSettingsServicesPtr d3dSettingSrv;
            res = g_ADLXHelp.GetSystemServices()->Get3DSettingsServices(&d3dSettingSrv);
            if (ADLX_SUCCEEDED(res))
            {
                // Get GPU interface
                IADLXGPUPtr gpuInfo;
                res = gpus->At(GPU, &gpuInfo);
                if (ADLX_SUCCEEDED(res))
                {
                    // Get Boost interface
                    IADLX3DBoostPtr boost;
                    res = d3dSettingSrv->GetBoost(gpuInfo, &boost);
                    if (ADLX_SUCCEEDED(res))
                    {
                        adlx_int currentMinRes;
                        boost->GetResolution(&currentMinRes);
                        result = currentMinRes;
                    }
                }
            }
        }

        return result;
    }

    ADLX_Wrapper bool SetBoostResolution(int GPU, int minRes)
    {
        // Define return code
        ADLX_RESULT res = ADLX_FAIL;
        bool result = false;

        // Get GPUs
        IADLXGPUListPtr gpus;
        res = g_ADLXHelp.GetSystemServices()->GetGPUs(&gpus);
        if (ADLX_SUCCEEDED(res) && !gpus->Empty())
        {
            // Get 3DSettings service
            IADLX3DSettingsServicesPtr d3dSettingSrv;
            res = g_ADLXHelp.GetSystemServices()->Get3DSettingsServices(&d3dSettingSrv);
            if (ADLX_SUCCEEDED(res))
            {
                // Get GPU interface
                IADLXGPUPtr gpuInfo;
                res = gpus->At(GPU, &gpuInfo);
                if (ADLX_SUCCEEDED(res))
                {
                    // Get Boost interface
                    IADLX3DBoostPtr boost;
                    res = d3dSettingSrv->GetBoost(gpuInfo, &boost);
                    if (ADLX_SUCCEEDED(res))
                    {
                        ADLX_RESULT res = boost->SetResolution(minRes);
                        result = ADLX_SUCCEEDED(res);
                    }
                }
            }
        }

        return result;
    }

    ADLX_Wrapper bool GetChill(int GPU)
    {
        // Define return code
        ADLX_RESULT res = ADLX_FAIL;
        bool result = false;

        // Get GPUs
        IADLXGPUListPtr gpus;
        res = g_ADLXHelp.GetSystemServices()->GetGPUs(&gpus);
        if (ADLX_SUCCEEDED(res) && !gpus->Empty())
        {
            // Get 3DSettings service
            IADLX3DSettingsServicesPtr d3dSettingSrv;
            res = g_ADLXHelp.GetSystemServices()->Get3DSettingsServices(&d3dSettingSrv);
            if (ADLX_SUCCEEDED(res))
            {
                // Get GPU interface
                IADLXGPUPtr gpuInfo;
                res = gpus->At(GPU, &gpuInfo);
                if (ADLX_SUCCEEDED(res))
                {
                    // Get Chill interface
                    IADLX3DChillPtr chill;
                    res = d3dSettingSrv->GetChill(gpuInfo, &chill);
                    if (ADLX_SUCCEEDED(res))
                    {
                        adlx_bool isEnabled;
                        chill->IsEnabled(&isEnabled);
                        result = isEnabled;
                    }
                }
            }
        }

        return result;
    }

    ADLX_Wrapper bool SetChill(int GPU, bool enable)
    {
        // Define return code
        ADLX_RESULT res = ADLX_FAIL;
        bool result = false;

        // Get GPUs
        IADLXGPUListPtr gpus;
        res = g_ADLXHelp.GetSystemServices()->GetGPUs(&gpus);
        if (ADLX_SUCCEEDED(res) && !gpus->Empty())
        {
            // Get 3DSettings service
            IADLX3DSettingsServicesPtr d3dSettingSrv;
            res = g_ADLXHelp.GetSystemServices()->Get3DSettingsServices(&d3dSettingSrv);
            if (ADLX_SUCCEEDED(res))
            {
                // Get GPU interface
                IADLXGPUPtr gpuInfo;
                res = gpus->At(GPU, &gpuInfo);
                if (ADLX_SUCCEEDED(res))
                {
                    // Get Chill interface
                    IADLX3DChillPtr chill;
                    res = d3dSettingSrv->GetChill(gpuInfo, &chill);
                    if (ADLX_SUCCEEDED(res))
                    {
                        ADLX_RESULT res = chill->SetEnabled(enable);
                        result = ADLX_SUCCEEDED(res);
                    }
                }
            }
        }

        return result;
    }

    ADLX_Wrapper int GetChillMinFPS(int GPU)
    {
        // Define return code
        ADLX_RESULT res = ADLX_FAIL;
        int result = -1;

        // Get GPUs
        IADLXGPUListPtr gpus;
        res = g_ADLXHelp.GetSystemServices()->GetGPUs(&gpus);
        if (ADLX_SUCCEEDED(res) && !gpus->Empty())
        {
            // Get 3DSettings service
            IADLX3DSettingsServicesPtr d3dSettingSrv;
            res = g_ADLXHelp.GetSystemServices()->Get3DSettingsServices(&d3dSettingSrv);
            if (ADLX_SUCCEEDED(res))
            {
                // Get GPU interface
                IADLXGPUPtr gpuInfo;
                res = gpus->At(GPU, &gpuInfo);
                if (ADLX_SUCCEEDED(res))
                {
                    // Get Chill interface
                    IADLX3DChillPtr chill;
                    res = d3dSettingSrv->GetChill(gpuInfo, &chill);
                    if (ADLX_SUCCEEDED(res))
                    {
                        adlx_int currentMinFPS;
                        chill->GetMinFPS(&currentMinFPS);
                        result = currentMinFPS;
                    }
                }
            }
        }

        return result;
    }

    ADLX_Wrapper bool SetChillMinFPS(int GPU, int minFPS)
    {
        // Define return code
        ADLX_RESULT res = ADLX_FAIL;
        bool result = false;

        // Get GPUs
        IADLXGPUListPtr gpus;
        res = g_ADLXHelp.GetSystemServices()->GetGPUs(&gpus);
        if (ADLX_SUCCEEDED(res) && !gpus->Empty())
        {
            // Get 3DSettings service
            IADLX3DSettingsServicesPtr d3dSettingSrv;
            res = g_ADLXHelp.GetSystemServices()->Get3DSettingsServices(&d3dSettingSrv);
            if (ADLX_SUCCEEDED(res))
            {
                // Get GPU interface
                IADLXGPUPtr gpuInfo;
                res = gpus->At(GPU, &gpuInfo);
                if (ADLX_SUCCEEDED(res))
                {
                    // Get Chill interface
                    IADLX3DChillPtr chill;
                    res = d3dSettingSrv->GetChill(gpuInfo, &chill);
                    if (ADLX_SUCCEEDED(res))
                    {
                        ADLX_RESULT res = chill->SetMinFPS(minFPS);
                        result = ADLX_SUCCEEDED(res);
                    }
                }
            }
        }

        return result;
    }

    ADLX_Wrapper int GetChillMaxFPS(int GPU)
    {
        // Define return code
        ADLX_RESULT res = ADLX_FAIL;
        int result = -1;

        // Get GPUs
        IADLXGPUListPtr gpus;
        res = g_ADLXHelp.GetSystemServices()->GetGPUs(&gpus);
        if (ADLX_SUCCEEDED(res) && !gpus->Empty())
        {
            // Get 3DSettings service
            IADLX3DSettingsServicesPtr d3dSettingSrv;
            res = g_ADLXHelp.GetSystemServices()->Get3DSettingsServices(&d3dSettingSrv);
            if (ADLX_SUCCEEDED(res))
            {
                // Get GPU interface
                IADLXGPUPtr gpuInfo;
                res = gpus->At(GPU, &gpuInfo);
                if (ADLX_SUCCEEDED(res))
                {
                    // Get Chill interface
                    IADLX3DChillPtr chill;
                    res = d3dSettingSrv->GetChill(gpuInfo, &chill);
                    if (ADLX_SUCCEEDED(res))
                    {
                        adlx_int currentMaxFPS;
                        chill->GetMaxFPS(&currentMaxFPS);
                        result = currentMaxFPS;
                    }
                }
            }
        }

        return result;
    }

    ADLX_Wrapper bool SetChillMaxFPS(int GPU, int maxFPS)
    {
        // Define return code
        ADLX_RESULT res = ADLX_FAIL;
        bool result = false;

        // Get GPUs
        IADLXGPUListPtr gpus;
        res = g_ADLXHelp.GetSystemServices()->GetGPUs(&gpus);
        if (ADLX_SUCCEEDED(res) && !gpus->Empty())
        {
            // Get 3DSettings service
            IADLX3DSettingsServicesPtr d3dSettingSrv;
            res = g_ADLXHelp.GetSystemServices()->Get3DSettingsServices(&d3dSettingSrv);
            if (ADLX_SUCCEEDED(res))
            {
                // Get GPU interface
                IADLXGPUPtr gpuInfo;
                res = gpus->At(GPU, &gpuInfo);
                if (ADLX_SUCCEEDED(res))
                {
                    // Get Chill interface
                    IADLX3DChillPtr chill;
                    res = d3dSettingSrv->GetChill(gpuInfo, &chill);
                    if (ADLX_SUCCEEDED(res))
                    {
                        ADLX_RESULT res = chill->SetMaxFPS(maxFPS);
                        result = ADLX_SUCCEEDED(res);
                    }
                }
            }
        }

        return result;
    }

    ADLX_Wrapper bool GetImageSharpening(int GPU)
    {
        // Define return code
        ADLX_RESULT res = ADLX_FAIL;
        bool result = false;

        // Get GPUs
        IADLXGPUListPtr gpus;
        res = g_ADLXHelp.GetSystemServices()->GetGPUs(&gpus);
        if (ADLX_SUCCEEDED(res) && !gpus->Empty())
        {
            // Get 3DSettings service
            IADLX3DSettingsServicesPtr d3dSettingSrv;
            res = g_ADLXHelp.GetSystemServices()->Get3DSettingsServices(&d3dSettingSrv);
            if (ADLX_SUCCEEDED(res))
            {
                // Get 3DSettings service
                IADLX3DSettingsServices1Ptr d3dSettingSrv1(d3dSettingSrv);

                // Get GPU interface
                IADLXGPUPtr gpuInfo;
                res = gpus->At(GPU, &gpuInfo);
                if (ADLX_SUCCEEDED(res))
                {
                    // Get Image Sharpening interface
                    IADLX3DImageSharpeningPtr sharp;
                    res = d3dSettingSrv1->GetImageSharpening(gpuInfo, &sharp);
                    if (ADLX_SUCCEEDED(res))
                    {
                        adlx_bool isEnabled;
                        sharp->IsEnabled(&isEnabled);
                        result = isEnabled;
                    }
                }
            }
        }

        return result;
    }

    ADLX_Wrapper bool SetImageSharpening(int GPU, bool enable)
    {
        // Define return code
        ADLX_RESULT res = ADLX_FAIL;
        bool result = false;

        // Get GPUs
        IADLXGPUListPtr gpus;
        res = g_ADLXHelp.GetSystemServices()->GetGPUs(&gpus);
        if (ADLX_SUCCEEDED(res) && !gpus->Empty())
        {
            // Get 3DSettings service
            IADLX3DSettingsServicesPtr d3dSettingSrv;
            res = g_ADLXHelp.GetSystemServices()->Get3DSettingsServices(&d3dSettingSrv);
            if (ADLX_SUCCEEDED(res))
            {
                // Get 3DSettings service
                IADLX3DSettingsServices1Ptr d3dSettingSrv1(d3dSettingSrv);

                // Get GPU interface
                IADLXGPUPtr gpuInfo;
                res = gpus->At(GPU, &gpuInfo);
                if (ADLX_SUCCEEDED(res))
                {
                    // Get Image Sharpening interface
                    IADLX3DImageSharpeningPtr sharp;
                    res = d3dSettingSrv1->GetImageSharpening(gpuInfo, &sharp);
                    if (ADLX_SUCCEEDED(res))
                    {
                        ADLX_RESULT res = sharp->SetEnabled(enable);
                        result = ADLX_SUCCEEDED(res);
                    }
                }
            }
        }

        return result;
    }

    ADLX_Wrapper int GetImageSharpeningSharpness(int GPU)
    {
        // Define return code
        ADLX_RESULT res = ADLX_FAIL;
        int result = -1;

        // Get GPUs
        IADLXGPUListPtr gpus;
        res = g_ADLXHelp.GetSystemServices()->GetGPUs(&gpus);
        if (ADLX_SUCCEEDED(res) && !gpus->Empty())
        {
            // Get 3DSettings service
            IADLX3DSettingsServicesPtr d3dSettingSrv;
            res = g_ADLXHelp.GetSystemServices()->Get3DSettingsServices(&d3dSettingSrv);
            if (ADLX_SUCCEEDED(res))
            {
                // Get 3DSettings service
                IADLX3DSettingsServices1Ptr d3dSettingSrv1(d3dSettingSrv);

                // Get GPU interface
                IADLXGPUPtr gpuInfo;
                res = gpus->At(GPU, &gpuInfo);
                if (ADLX_SUCCEEDED(res))
                {
                    // Get Image Sharpening interface
                    IADLX3DImageSharpeningPtr sharp;
                    res = d3dSettingSrv1->GetImageSharpening(gpuInfo, &sharp);
                    if (ADLX_SUCCEEDED(res))
                    {
                        adlx_int currentMaxFPS;
                        sharp->GetSharpness(&currentMaxFPS);
                        result = currentMaxFPS;
                    }
                }
            }
        }

        return result;
    }

    ADLX_Wrapper bool SetImageSharpeningSharpness(int GPU, int sharpness)
    {
        // Define return code
        ADLX_RESULT res = ADLX_FAIL;
        int result = -1;

        // Get GPUs
        IADLXGPUListPtr gpus;
        res = g_ADLXHelp.GetSystemServices()->GetGPUs(&gpus);
        if (ADLX_SUCCEEDED(res) && !gpus->Empty())
        {
            // Get 3DSettings service
            IADLX3DSettingsServicesPtr d3dSettingSrv;
            res = g_ADLXHelp.GetSystemServices()->Get3DSettingsServices(&d3dSettingSrv);
            if (ADLX_SUCCEEDED(res))
            {
                // Get 3DSettings service
                IADLX3DSettingsServices1Ptr d3dSettingSrv1(d3dSettingSrv);

                // Get GPU interface
                IADLXGPUPtr gpuInfo;
                res = gpus->At(GPU, &gpuInfo);
                if (ADLX_SUCCEEDED(res))
                {
                    // Get Image Sharpening interface
                    IADLX3DImageSharpeningPtr sharp;
                    res = d3dSettingSrv1->GetImageSharpening(gpuInfo, &sharp);
                    if (ADLX_SUCCEEDED(res))
                    {
                        ADLX_RESULT res = sharp->SetSharpness(sharpness);
                        result = ADLX_SUCCEEDED(res);
                    }
                }
            }
        }

        return result;
    }

    ADLX_Wrapper bool GetTimeStamp(const AdlxTelemetryInterfaces& telemetryInterfaces)
    {
        ADLX_RESULT res = ADLX_FAIL;
        adlx_int64 timeStamp = 0;

        if (telemetryInterfaces.gpuMetrics == nullptr)
            return false;

        res = telemetryInterfaces.gpuMetrics->TimeStamp(&timeStamp);
        return ADLX_SUCCEEDED(res);
    }
    
    // Set GPU usage (in %)
    ADLX_Wrapper void GetGPUUsage(const AdlxTelemetryInterfaces& telemetryInterfaces, AdlxTelemetryData* telemetryData)
    {
        if (telemetryInterfaces.gpuMetricsSupport == nullptr || telemetryInterfaces.gpuMetrics == nullptr)
            return;

        adlx_bool supported = false;

        // Display GPU usage support status
        ADLX_RESULT res = telemetryInterfaces.gpuMetricsSupport->IsSupportedGPUUsage(&supported);
        if (ADLX_SUCCEEDED(res))
        {
            telemetryData->gpuUsageSupported = supported;
            if (supported)
            {
                adlx_double usage = 0;
                res = telemetryInterfaces.gpuMetrics->GPUUsage(&usage);
                if (ADLX_SUCCEEDED(res))
                    telemetryData->gpuUsageValue = usage;
            }
        }
    }

    ADLX_Wrapper bool HasPresetTuningSupport(int GPU)
    {
        // Define return code
        ADLX_RESULT res = ADLX_FAIL;
        bool result = false;

        IADLXGPUTuningServicesPtr gpuTuningService;
        res = g_ADLXHelp.GetSystemServices()->GetGPUTuningServices(&gpuTuningService);
        if (ADLX_SUCCEEDED(res))
        {
            IADLXGPUListPtr gpus;
            res = g_ADLXHelp.GetSystemServices()->GetGPUs(&gpus);
            if (ADLX_SUCCEEDED(res))
            {
                IADLXGPUPtr oneGPU;
                res = gpus->At(GPU, &oneGPU);
                if (ADLX_SUCCEEDED(res) && oneGPU != nullptr)
                {
                    adlx_bool supported = false;
                    gpuTuningService->IsSupportedPresetTuning(oneGPU, &supported);
                    result = supported;
                }
            }
        }

        return result;
    }

    ADLX_Wrapper ADLX_RESULT GetGPUFrequency(int GPU, int* minFreq, int* maxFreq)
    {
        // Define return code
        ADLX_RESULT res = ADLX_FAIL;

        IADLXGPUTuningServicesPtr gpuTuningService;
        res = g_ADLXHelp.GetSystemServices()->GetGPUTuningServices(&gpuTuningService);
        if (ADLX_SUCCEEDED(res))
        {
            IADLXGPUListPtr gpus;
            res = g_ADLXHelp.GetSystemServices()->GetGPUs(&gpus);
            if (ADLX_SUCCEEDED(res))
            {
                IADLXGPUPtr oneGPU;
                res = gpus->At(GPU, &oneGPU);
                if (ADLX_SUCCEEDED(res) && oneGPU != nullptr)
                {
                    adlx_bool supported = false;
                    res = gpuTuningService->IsSupportedPresetTuning(oneGPU, &supported);
                    if (ADLX_SUCCEEDED(res) && supported)
                    {
                        IADLXInterfacePtr gpuPresetTuningIfc;
                        res = gpuTuningService->GetPresetTuning(oneGPU, &gpuPresetTuningIfc);
                        if (ADLX_SUCCEEDED(res) && gpuPresetTuningIfc != nullptr)
                        {
                            IADLXGPUPresetTuningPtr gpuPresetTuning(gpuPresetTuningIfc);
                            if (gpuPresetTuning != nullptr)
                            {
                                IADLXInterfacePtr manualGFXTuningIfc;
                                res = gpuTuningService->GetManualGFXTuning(oneGPU, &manualGFXTuningIfc);

                                // Post-Navi ASIC
                                IADLXManualGraphicsTuning2Ptr manualGFXTuning2(manualGFXTuningIfc);
                                if (manualGFXTuning2 != nullptr)
                                {
                                    res = manualGFXTuning2->GetGPUMinFrequency(minFreq);
                                    res = manualGFXTuning2->GetGPUMaxFrequency(maxFreq);
                                }
                            }
                        }
                    }
                }
            }
        }

        return res;
    }

    ADLX_Wrapper ADLX_RESULT SetGPUFrequency(int GPU, int minFreq, int maxFreq)
    {
        // Define return code
        ADLX_RESULT res = ADLX_FAIL;

        IADLXGPUTuningServicesPtr gpuTuningService;
        res = g_ADLXHelp.GetSystemServices()->GetGPUTuningServices(&gpuTuningService);
        if (ADLX_SUCCEEDED(res))
        {
            IADLXGPUListPtr gpus;
            res = g_ADLXHelp.GetSystemServices()->GetGPUs(&gpus);
            if (ADLX_SUCCEEDED(res))
            {
                IADLXGPUPtr oneGPU;
                res = gpus->At(GPU, &oneGPU);
                if (ADLX_SUCCEEDED(res) && oneGPU != nullptr)
                {
                    adlx_bool supported = false;
                    res = gpuTuningService->IsSupportedPresetTuning(oneGPU, &supported);
                    if (ADLX_SUCCEEDED(res) && supported)
                    {
                        IADLXInterfacePtr gpuPresetTuningIfc;
                        res = gpuTuningService->GetPresetTuning(oneGPU, &gpuPresetTuningIfc);
                        if (ADLX_SUCCEEDED(res) && gpuPresetTuningIfc != nullptr)
                        {
                            IADLXGPUPresetTuningPtr gpuPresetTuning(gpuPresetTuningIfc);
                            if (gpuPresetTuning != nullptr)
                            {
                                IADLXInterfacePtr manualGFXTuningIfc;
                                res = gpuTuningService->GetManualGFXTuning(oneGPU, &manualGFXTuningIfc);

                                // Post-Navi ASIC
                                IADLXManualGraphicsTuning2Ptr manualGFXTuning2(manualGFXTuningIfc);
                                if (manualGFXTuning2 != nullptr)
                                {
                                    res = manualGFXTuning2->SetGPUMinFrequency(minFreq);
                                    res = manualGFXTuning2->SetGPUMaxFrequency(maxFreq);
                                }
                            }
                        }
                    }
                }
            }
        }

        return res;
    }

    // Set GPU clock speed (in MHz)
    ADLX_Wrapper void GetGPUClockSpeed(const AdlxTelemetryInterfaces& telemetryInterfaces, AdlxTelemetryData* telemetryData)
    {
        if (telemetryInterfaces.gpuMetricsSupport == nullptr || telemetryInterfaces.gpuMetrics == nullptr)
            return;

        adlx_bool supported = false;

        // Display GPU clock speed support status
        ADLX_RESULT res = telemetryInterfaces.gpuMetricsSupport->IsSupportedGPUClockSpeed(&supported);
        if (ADLX_SUCCEEDED(res))
        {
            telemetryData->gpuClockSpeedSupported = supported;
            if (supported)
            {
                adlx_int gpuClock = 0;
                res = telemetryInterfaces.gpuMetrics->GPUClockSpeed(&gpuClock);
                if (ADLX_SUCCEEDED(res))
                    telemetryData->gpuClockSpeedValue = gpuClock;
            }
        }
    }

    // Set GPU VRAM clock speed (in MHz)
    ADLX_Wrapper void GetGPUVRAMClockSpeed(const AdlxTelemetryInterfaces& telemetryInterfaces, AdlxTelemetryData* telemetryData)
    {
        if (telemetryInterfaces.gpuMetricsSupport == nullptr || telemetryInterfaces.gpuMetrics == nullptr)
            return;

        adlx_bool supported = false;

        // Display the GPU VRAM clock speed support status
        ADLX_RESULT res = telemetryInterfaces.gpuMetricsSupport->IsSupportedGPUVRAMClockSpeed(&supported);
        if (ADLX_SUCCEEDED(res))
        {
            telemetryData->gpuVRAMClockSpeedSupported = supported;
            if (supported)
            {
                adlx_int memoryClock = 0;
                res = telemetryInterfaces.gpuMetrics->GPUVRAMClockSpeed(&memoryClock);
                if (ADLX_SUCCEEDED(res))
                    telemetryData->gpuVRAMClockSpeedValue = memoryClock;
            }
        }
    }

    // Set GPU temperature(in °C)
    ADLX_Wrapper void GetGPUTemperature(const AdlxTelemetryInterfaces& telemetryInterfaces, AdlxTelemetryData* telemetryData)
    {
        if (telemetryInterfaces.gpuMetricsSupport == nullptr || telemetryInterfaces.gpuMetrics == nullptr)
            return;

        adlx_bool supported = false;

        // Display the GPU temperature support status
        ADLX_RESULT res = telemetryInterfaces.gpuMetricsSupport->IsSupportedGPUTemperature(&supported);
        if (ADLX_SUCCEEDED(res))
        {
            telemetryData->gpuTemperatureSupported = supported;
            if (supported)
            {
                adlx_double temperature = 0;
                res = telemetryInterfaces.gpuMetrics->GPUTemperature(&temperature);
                if (ADLX_SUCCEEDED(res))
                    telemetryData->gpuTemperatureValue = temperature;
            }
        }
    }

    // Set GPU hotspot temperature(in °C)
    ADLX_Wrapper void GetGPUHotspotTemperature(const AdlxTelemetryInterfaces& telemetryInterfaces, AdlxTelemetryData* telemetryData)
    {
        if (telemetryInterfaces.gpuMetricsSupport == nullptr || telemetryInterfaces.gpuMetrics == nullptr)
            return;

        adlx_bool supported = false;

        // Display GPU hotspot temperature support status
        ADLX_RESULT res = telemetryInterfaces.gpuMetricsSupport->IsSupportedGPUHotspotTemperature(&supported);
        if (ADLX_SUCCEEDED(res))
        {
            telemetryData->gpuHotspotTemperatureSupported = supported;
            if (supported)
            {
                adlx_double hotspotTemperature = 0;
                res = telemetryInterfaces.gpuMetrics->GPUHotspotTemperature(&hotspotTemperature);
                if (ADLX_SUCCEEDED(res))
                    telemetryData->gpuHotspotTemperatureValue = hotspotTemperature;
            }
        }
    }

    // Set GPU power(in W)
    ADLX_Wrapper void GetGPUPower(const AdlxTelemetryInterfaces& telemetryInterfaces, AdlxTelemetryData* telemetryData)
    {
        if (telemetryInterfaces.gpuMetricsSupport == nullptr || telemetryInterfaces.gpuMetrics == nullptr)
            return;

        adlx_bool supported = false;

        // Display GPU power support status
        ADLX_RESULT res = telemetryInterfaces.gpuMetricsSupport->IsSupportedGPUPower(&supported);
        if (ADLX_SUCCEEDED(res))
        {
            telemetryData->gpuPowerSupported = supported;
            if (supported)
            {
                adlx_double power = 0;
                res = telemetryInterfaces.gpuMetrics->GPUPower(&power);
                if (ADLX_SUCCEEDED(res))
                    telemetryData->gpuPowerValue = power;
            }
        }
    }

    // Set GPU total board power(in W)
    ADLX_Wrapper void GetGPUTotalBoardPower(const AdlxTelemetryInterfaces& telemetryInterfaces, AdlxTelemetryData* telemetryData)
    {
        if (telemetryInterfaces.gpuMetricsSupport == nullptr || telemetryInterfaces.gpuMetrics == nullptr)
            return;

        adlx_bool supported = false;

        // Display GPU total board power support status
        ADLX_RESULT res = telemetryInterfaces.gpuMetricsSupport->IsSupportedGPUTotalBoardPower(&supported);
        if (ADLX_SUCCEEDED(res))
        {
            telemetryData->gpuTotalBoardPowerSupported = supported;
            if (supported)
            {
                adlx_double power = 0;
                res = telemetryInterfaces.gpuMetrics->GPUTotalBoardPower(&power);
                if (ADLX_SUCCEEDED(res))
                    telemetryData->gpuTotalBoardPowerValue = power;
            }
        }
    }

    // Set GPU intake temperature(in ?C)
    ADLX_Wrapper void GetGPUIntakeTemperature(const AdlxTelemetryInterfaces& telemetryInterfaces, AdlxTelemetryData* telemetryData)
    {
        if (telemetryInterfaces.gpuMetricsSupport == nullptr || telemetryInterfaces.gpuMetrics == nullptr)
            return;

        adlx_bool supported = false;

        ADLX_RESULT res = telemetryInterfaces.gpuMetricsSupport->IsSupportedGPUIntakeTemperature(&supported);
        if (ADLX_SUCCEEDED(res))
        {
            telemetryData->gpuIntakeTemperatureSupported = supported;
            if (supported)
            {
                adlx_double intakeTemperature = 0;
                res = telemetryInterfaces.gpuMetrics->GPUIntakeTemperature(&intakeTemperature);
                if (ADLX_SUCCEEDED(res))
                    telemetryData->gpuIntakeTemperatureValue = intakeTemperature;
            }
        }
    }

    // Set GPU fan speed (in RPM)
    ADLX_Wrapper void GetGPUFanSpeed(const AdlxTelemetryInterfaces& telemetryInterfaces, AdlxTelemetryData* telemetryData)
    {
        if (telemetryInterfaces.gpuMetricsSupport == nullptr || telemetryInterfaces.gpuMetrics == nullptr)
            return;

        adlx_bool supported = false;

        // Display GPU fan speed support status
        ADLX_RESULT res = telemetryInterfaces.gpuMetricsSupport->IsSupportedGPUFanSpeed(&supported);
        if (ADLX_SUCCEEDED(res))
        {
            telemetryData->gpuFanSpeedSupported = supported;
            if (supported)
            {
                adlx_int fanSpeed = 0;
                res = telemetryInterfaces.gpuMetrics->GPUFanSpeed(&fanSpeed);
                if (ADLX_SUCCEEDED(res))
                    telemetryData->gpuFanSpeedValue = fanSpeed;
            }
        }
    }

    // Set GPU VRAM (in MB)
    ADLX_Wrapper void GetGPUVRAM(const AdlxTelemetryInterfaces& telemetryInterfaces, AdlxTelemetryData* telemetryData)
    {
        if (telemetryInterfaces.gpuMetricsSupport == nullptr || telemetryInterfaces.gpuMetrics == nullptr)
            return;

        adlx_bool supported = false;

        // Display GPU VRAM support status
        ADLX_RESULT res = telemetryInterfaces.gpuMetricsSupport->IsSupportedGPUVRAM(&supported);
        if (ADLX_SUCCEEDED(res))
        {
            telemetryData->gpuVramSupported = supported;
            if (supported)
            {
                adlx_int VRAM = 0;
                res = telemetryInterfaces.gpuMetrics->GPUVRAM(&VRAM);
                if (ADLX_SUCCEEDED(res))
                    telemetryData->gpuVramValue = VRAM;
            }
        }
    }

    // Set GPU Voltage (in mV)
    ADLX_Wrapper void GetGPUVoltage(const AdlxTelemetryInterfaces& telemetryInterfaces, AdlxTelemetryData* telemetryData)
    {
        if (telemetryInterfaces.gpuMetricsSupport == nullptr || telemetryInterfaces.gpuMetrics == nullptr)
            return;

        adlx_bool supported = false;

        // Display GPU voltage support status
        ADLX_RESULT res = telemetryInterfaces.gpuMetricsSupport->IsSupportedGPUVoltage(&supported);
        if (ADLX_SUCCEEDED(res))
        {
            telemetryData->gpuVoltageSupported = supported;
            if (supported)
            {
                adlx_int voltage = 0;
                res = telemetryInterfaces.gpuMetrics->GPUVoltage(&voltage);
                if (ADLX_SUCCEEDED(res))
                    telemetryData->gpuVoltageValue = voltage;
            }
        }
    }

    // GPU memory temperature (in ?C)
    ADLX_Wrapper void GetGPUMemoryTemperature(const AdlxTelemetryInterfaces& telemetryInterfaces, AdlxTelemetryData* telemetryData)
    {
        if (telemetryInterfaces.gpuMetricsSupport1 == nullptr || telemetryInterfaces.gpuMetrics1 == nullptr)
            return;

        adlx_bool supported = false;
        ADLX_RESULT res = telemetryInterfaces.gpuMetricsSupport1->IsSupportedGPUMemoryTemperature(&supported);
        if (ADLX_SUCCEEDED(res))
        {
            telemetryData->gpuMemoryTemperatureSupported = supported;
            if (supported)
            {
                adlx_double memoryTemperature = 0;
                res = telemetryInterfaces.gpuMetrics1->GPUMemoryTemperature(&memoryTemperature);
                if (ADLX_SUCCEEDED(res))
                    telemetryData->gpuMemoryTemperatureValue = memoryTemperature;
            }
        }
    }

    // NPU frequency (in MHz)
    ADLX_Wrapper void GetNPUFrequency(const AdlxTelemetryInterfaces& telemetryInterfaces, AdlxTelemetryData* telemetryData)
    {
        if (telemetryInterfaces.gpuMetricsSupport1 == nullptr || telemetryInterfaces.gpuMetrics1 == nullptr)
            return;

        adlx_bool supported = false;
        ADLX_RESULT res = telemetryInterfaces.gpuMetricsSupport1->IsSupportedNPUFrequency(&supported);
        if (ADLX_SUCCEEDED(res))
        {
            telemetryData->npuFrequencySupported = supported;
            if (supported)
            {
                adlx_int npuFrequency = 0;
                res = telemetryInterfaces.gpuMetrics1->NPUFrequency(&npuFrequency);
                if (ADLX_SUCCEEDED(res))
                    telemetryData->npuFrequencyValue = npuFrequency;
            }
        }
    }

    // NPU activity (in %)
    ADLX_Wrapper void GetNPUActivity(const AdlxTelemetryInterfaces& telemetryInterfaces, AdlxTelemetryData* telemetryData)
    {
        if (telemetryInterfaces.gpuMetricsSupport1 == nullptr || telemetryInterfaces.gpuMetrics1 == nullptr)
            return;

        adlx_bool supported = false;
        ADLX_RESULT res = telemetryInterfaces.gpuMetricsSupport1->IsSupportedNPUActivityLevel(&supported);
        if (ADLX_SUCCEEDED(res))
        {
            telemetryData->npuActivitySupported = supported;
            if (supported)
            {
                adlx_int npuActivity = 0;
                res = telemetryInterfaces.gpuMetrics1->NPUActivityLevel(&npuActivity);
                if (ADLX_SUCCEEDED(res))
                    telemetryData->npuActivityValue = npuActivity;
            }
        }
    }

    // GPU Fan Duty (in %)
    ADLX_Wrapper void GetGPUFanDuty(const AdlxTelemetryInterfaces& telemetryInterfaces, AdlxTelemetryData* telemetryData)
    {
        if (telemetryInterfaces.gpuMetricsSupport3 == nullptr || telemetryInterfaces.gpuMetrics3 == nullptr)
            return;

        adlx_bool supported = false;
        ADLX_RESULT res = telemetryInterfaces.gpuMetricsSupport3->IsSupportedGPUFanDuty(&supported);
        if (ADLX_SUCCEEDED(res))
        {
            telemetryData->gpuFanDutySupported = supported;
            if (supported)
            {
                adlx_int fanDuty = 0;
                res = telemetryInterfaces.gpuMetrics3->GPUFanDuty(&fanDuty);
                if (ADLX_SUCCEEDED(res))
                    telemetryData->gpuFanDutyValue = fanDuty;
            }
        }
    }

    // GPU Shared Memory (in MB)
    ADLX_Wrapper void GetGPUSharedMemory(const AdlxTelemetryInterfaces& telemetryInterfaces, AdlxTelemetryData* telemetryData)
    {
        if (telemetryInterfaces.gpuMetricsSupport2 == nullptr || telemetryInterfaces.gpuMetrics2 == nullptr)
            return;

        adlx_bool supported = false;
        ADLX_RESULT res = telemetryInterfaces.gpuMetricsSupport2->IsSupportedGPUSharedMemory(&supported);
        if (ADLX_SUCCEEDED(res))
        {
            telemetryData->gpuSharedMemorySupported = supported;
            if (supported)
            {
                adlx_int sharedMemory = 0;
                res = telemetryInterfaces.gpuMetrics2->GPUSharedMemory(&sharedMemory);
                if (ADLX_SUCCEEDED(res))
                    telemetryData->gpuSharedMemoryValue = sharedMemory;
            }
        }
    }

    ADLX_Wrapper void GetGPUFramerate(IADLXPerformanceMonitoringServicesPtr perfMonitoringServices, AdlxTelemetryData* telemetryData)
    {
        IADLXFPSPtr oneFPS;

        // Get current FPS metrics
        ADLX_RESULT res = perfMonitoringServices->GetCurrentFPS(&oneFPS);
        if (ADLX_SUCCEEDED(res))
        {
            adlx_int64 timeStamp = 0;
            res = oneFPS->TimeStamp(&timeStamp);
            if (ADLX_SUCCEEDED(res))
            {
                telemetryData->timeStamp = timeStamp;
                std::cout << "The current metric time stamp is: " << timeStamp << "ms\n";
            }

            adlx_int fpsData = 0;
            res = oneFPS->FPS(&fpsData);
            if (ADLX_SUCCEEDED(res))
            {
                telemetryData->fpsData = fpsData;
                std::cout << "The current metric FPS is: " << fpsData << std::endl;
            }
            else if (res == ADLX_NOT_SUPPORTED)
                std::cout << "Don't support FPS" << std::endl;
        }
    }
    
    ADLX_Wrapper bool GetAdlxTelemetry(int GPU, AdlxTelemetryData* adlxTelemetryData)
    {
        ADLX_RESULT res = ADLX_FAIL;
        bool check = false;

        // Get GPU metrics support
        IADLXGPUMetricsSupportPtr gpuMetricsSupport;
        IADLXGPUMetricsPtr gpuMetrics;
        IADLXPerformanceMonitoringServicesPtr perfMonitoringService;

        // Get Performance Monitoring services
        res = g_ADLXHelp.GetSystemServices()->GetPerformanceMonitoringServices(&perfMonitoringService);
        if (ADLX_SUCCEEDED(res))
        {
            // Get GPUs
            IADLXGPUListPtr gpus;
            res = g_ADLXHelp.GetSystemServices()->GetGPUs(&gpus);
            if (ADLX_SUCCEEDED(res) && !gpus->Empty())
            {
                // Get GPU interface
                IADLXGPUPtr gpuInfo;
                res = gpus->At(GPU, &gpuInfo);
                if (ADLX_SUCCEEDED(res))
                {
                    ADLX_RESULT res1 = perfMonitoringService->GetSupportedGPUMetrics(gpuInfo, &gpuMetricsSupport);
                    ADLX_RESULT res2 = perfMonitoringService->GetCurrentGPUMetrics(gpuInfo, &gpuMetrics);

                    if (ADLX_SUCCEEDED(res1) && ADLX_SUCCEEDED(res2))
                    {
                        AdlxTelemetryInterfaces telemetryInterfaces{};
                        ResolveTelemetryInterfaces(gpuMetricsSupport, gpuMetrics, &telemetryInterfaces);

                        // Acquire the interface
                        gpuMetrics->Acquire();

                        GetTimeStamp(telemetryInterfaces);
                        GetGPUUsage(telemetryInterfaces, adlxTelemetryData);
                        GetGPUClockSpeed(telemetryInterfaces, adlxTelemetryData);
                        GetGPUVRAMClockSpeed(telemetryInterfaces, adlxTelemetryData);
                        GetGPUTemperature(telemetryInterfaces, adlxTelemetryData);
                        GetGPUHotspotTemperature(telemetryInterfaces, adlxTelemetryData);
                        GetGPUPower(telemetryInterfaces, adlxTelemetryData);
                        GetGPUFanSpeed(telemetryInterfaces, adlxTelemetryData);
                        GetGPUVRAM(telemetryInterfaces, adlxTelemetryData);
                        GetGPUVoltage(telemetryInterfaces, adlxTelemetryData);
                        GetGPUTotalBoardPower(telemetryInterfaces, adlxTelemetryData);
                        GetGPUIntakeTemperature(telemetryInterfaces, adlxTelemetryData);
                        GetGPUMemoryTemperature(telemetryInterfaces, adlxTelemetryData);
                        GetNPUFrequency(telemetryInterfaces, adlxTelemetryData);
                        GetNPUActivity(telemetryInterfaces, adlxTelemetryData);
                        GetGPUFanDuty(telemetryInterfaces, adlxTelemetryData);
                        GetGPUSharedMemory(telemetryInterfaces, adlxTelemetryData);
                        GetGPUFramerate(perfMonitoringService, adlxTelemetryData);

                        // Release the interface
                        gpuMetrics->Release();

                        check = true;
                    }
                }
            }
        }

        return check;
    }

    ADLX_Wrapper bool HasFSRSupport(int GPU)
    {
        ADLX_RESULT res = ADLX_FAIL;
        bool result = false;

        IADLXGPUListPtr gpus;
        res = g_ADLXHelp.GetSystemServices()->GetGPUs(&gpus);
        if (ADLX_SUCCEEDED(res) && !gpus->Empty())
        {
            IADLX3DSettingsServicesPtr d3dSettingSrv;
            res = g_ADLXHelp.GetSystemServices()->Get3DSettingsServices(&d3dSettingSrv);
            if (ADLX_SUCCEEDED(res))
            {
                IADLX3DSettingsServices3Ptr d3dSettingSrv3(d3dSettingSrv);
                if (d3dSettingSrv3 != nullptr)
                {
                    IADLXGPUPtr gpuInfo;
                    res = gpus->At(GPU, &gpuInfo);
                    if (ADLX_SUCCEEDED(res))
                    {
                        IADLX3DFidelityFXSuperResolutionPtr fsr;
                        res = d3dSettingSrv3->GetFidelityFXSuperResolution(gpuInfo, &fsr);
                        if (ADLX_SUCCEEDED(res))
                        {
                            adlx_bool supported = false;
                            fsr->IsSupported(&supported);
                            result = supported;
                        }
                    }
                }
            }
        }

        return result;
    }

    ADLX_Wrapper bool GetFSR(int GPU)
    {
        ADLX_RESULT res = ADLX_FAIL;
        bool result = false;

        IADLXGPUListPtr gpus;
        res = g_ADLXHelp.GetSystemServices()->GetGPUs(&gpus);
        if (ADLX_SUCCEEDED(res) && !gpus->Empty())
        {
            IADLX3DSettingsServicesPtr d3dSettingSrv;
            res = g_ADLXHelp.GetSystemServices()->Get3DSettingsServices(&d3dSettingSrv);
            if (ADLX_SUCCEEDED(res))
            {
                IADLX3DSettingsServices3Ptr d3dSettingSrv3(d3dSettingSrv);
                if (d3dSettingSrv3 != nullptr)
                {
                    IADLXGPUPtr gpuInfo;
                    res = gpus->At(GPU, &gpuInfo);
                    if (ADLX_SUCCEEDED(res))
                    {
                        IADLX3DFidelityFXSuperResolutionPtr fsr;
                        res = d3dSettingSrv3->GetFidelityFXSuperResolution(gpuInfo, &fsr);
                        if (ADLX_SUCCEEDED(res))
                        {
                            adlx_bool supported = false;
                            fsr->IsSupported(&supported);
                            if (supported)
                            {
                                adlx_bool enabled = false;
                                fsr->IsEnabled(&enabled);
                                result = enabled;
                            }
                        }
                    }
                }
            }
        }

        return result;
    }

    ADLX_Wrapper bool SetFSR(int GPU, bool enable)
    {
        ADLX_RESULT res = ADLX_FAIL;
        bool result = false;

        IADLXGPUListPtr gpus;
        res = g_ADLXHelp.GetSystemServices()->GetGPUs(&gpus);
        if (ADLX_SUCCEEDED(res) && !gpus->Empty())
        {
            IADLX3DSettingsServicesPtr d3dSettingSrv;
            res = g_ADLXHelp.GetSystemServices()->Get3DSettingsServices(&d3dSettingSrv);
            if (ADLX_SUCCEEDED(res))
            {
                IADLX3DSettingsServices3Ptr d3dSettingSrv3(d3dSettingSrv);
                if (d3dSettingSrv3 != nullptr)
                {
                    IADLXGPUPtr gpuInfo;
                    res = gpus->At(GPU, &gpuInfo);
                    if (ADLX_SUCCEEDED(res))
                    {
                        IADLX3DFidelityFXSuperResolutionPtr fsr;
                        res = d3dSettingSrv3->GetFidelityFXSuperResolution(gpuInfo, &fsr);
                        if (ADLX_SUCCEEDED(res))
                        {
                            adlx_bool supported = false;
                            fsr->IsSupported(&supported);
                            if (supported)
                            {
                                res = fsr->SetEnabled(enable);
                                result = ADLX_SUCCEEDED(res);
                            }
                        }
                    }
                }
            }
        }

        return result;
    }

    ADLX_Wrapper bool HasFFXFrameGenSupport(int GPU)
    {
        ADLX_RESULT res = ADLX_FAIL;
        bool result = false;

        IADLXGPUListPtr gpus;
        res = g_ADLXHelp.GetSystemServices()->GetGPUs(&gpus);
        if (ADLX_SUCCEEDED(res) && !gpus->Empty())
        {
            IADLX3DSettingsServicesPtr d3dSettingSrv;
            res = g_ADLXHelp.GetSystemServices()->Get3DSettingsServices(&d3dSettingSrv);
            if (ADLX_SUCCEEDED(res))
            {
                IADLX3DSettingsServices3Ptr d3dSettingSrv3(d3dSettingSrv);
                if (d3dSettingSrv3 != nullptr)
                {
                    IADLXGPUPtr gpuInfo;
                    res = gpus->At(GPU, &gpuInfo);
                    if (ADLX_SUCCEEDED(res))
                    {
                        IADLX3DFidelityFXFrameGenUpgradePtr ffxFG;
                        res = d3dSettingSrv3->GetFidelityFXFrameGenUpgrade(gpuInfo, &ffxFG);
                        if (ADLX_SUCCEEDED(res))
                        {
                            adlx_bool supported = false;
                            ffxFG->IsSupported(&supported);
                            result = supported;
                        }
                    }
                }
            }
        }

        return result;
    }

    ADLX_Wrapper bool GetFFXFrameGen(int GPU)
    {
        ADLX_RESULT res = ADLX_FAIL;
        bool result = false;

        IADLXGPUListPtr gpus;
        res = g_ADLXHelp.GetSystemServices()->GetGPUs(&gpus);
        if (ADLX_SUCCEEDED(res) && !gpus->Empty())
        {
            IADLX3DSettingsServicesPtr d3dSettingSrv;
            res = g_ADLXHelp.GetSystemServices()->Get3DSettingsServices(&d3dSettingSrv);
            if (ADLX_SUCCEEDED(res))
            {
                IADLX3DSettingsServices3Ptr d3dSettingSrv3(d3dSettingSrv);
                if (d3dSettingSrv3 != nullptr)
                {
                    IADLXGPUPtr gpuInfo;
                    res = gpus->At(GPU, &gpuInfo);
                    if (ADLX_SUCCEEDED(res))
                    {
                        IADLX3DFidelityFXFrameGenUpgradePtr ffxFG;
                        res = d3dSettingSrv3->GetFidelityFXFrameGenUpgrade(gpuInfo, &ffxFG);
                        if (ADLX_SUCCEEDED(res))
                        {
                            adlx_bool supported = false;
                            ffxFG->IsSupported(&supported);
                            if (supported)
                            {
                                adlx_bool enabled = false;
                                ffxFG->IsEnabled(&enabled);
                                result = enabled;
                            }
                        }
                    }
                }
            }
        }

        return result;
    }

    ADLX_Wrapper bool SetFFXFrameGen(int GPU, bool enable)
    {
        ADLX_RESULT res = ADLX_FAIL;
        bool result = false;

        IADLXGPUListPtr gpus;
        res = g_ADLXHelp.GetSystemServices()->GetGPUs(&gpus);
        if (ADLX_SUCCEEDED(res) && !gpus->Empty())
        {
            IADLX3DSettingsServicesPtr d3dSettingSrv;
            res = g_ADLXHelp.GetSystemServices()->Get3DSettingsServices(&d3dSettingSrv);
            if (ADLX_SUCCEEDED(res))
            {
                IADLX3DSettingsServices3Ptr d3dSettingSrv3(d3dSettingSrv);
                if (d3dSettingSrv3 != nullptr)
                {
                    IADLXGPUPtr gpuInfo;
                    res = gpus->At(GPU, &gpuInfo);
                    if (ADLX_SUCCEEDED(res))
                    {
                        IADLX3DFidelityFXFrameGenUpgradePtr ffxFG;
                        res = d3dSettingSrv3->GetFidelityFXFrameGenUpgrade(gpuInfo, &ffxFG);
                        if (ADLX_SUCCEEDED(res))
                        {
                            adlx_bool supported = false;
                            ffxFG->IsSupported(&supported);
                            if (supported)
                            {
                                res = ffxFG->SetEnabled(enable);
                                result = ADLX_SUCCEEDED(res);
                            }
                        }
                    }
                }
            }
        }

        return result;
    }

    ADLX_Wrapper int GetFFXFrameGenRatio(int GPU)
    {
        ADLX_RESULT res = ADLX_FAIL;
        int result = -1;

        IADLXGPUListPtr gpus;
        res = g_ADLXHelp.GetSystemServices()->GetGPUs(&gpus);
        if (ADLX_SUCCEEDED(res) && !gpus->Empty())
        {
            IADLX3DSettingsServicesPtr d3dSettingSrv;
            res = g_ADLXHelp.GetSystemServices()->Get3DSettingsServices(&d3dSettingSrv);
            if (ADLX_SUCCEEDED(res))
            {
                IADLX3DSettingsServices3Ptr d3dSettingSrv3(d3dSettingSrv);
                if (d3dSettingSrv3 != nullptr)
                {
                    IADLXGPUPtr gpuInfo;
                    res = gpus->At(GPU, &gpuInfo);
                    if (ADLX_SUCCEEDED(res))
                    {
                        IADLX3DFidelityFXFrameGenUpgradePtr ffxFG;
                        res = d3dSettingSrv3->GetFidelityFXFrameGenUpgrade(gpuInfo, &ffxFG);
                        if (ADLX_SUCCEEDED(res))
                        {
                            ADLX_FFX_FRAME_GEN_RATIO ratio;
                            res = ffxFG->GetRatio(&ratio);
                            if (ADLX_SUCCEEDED(res))
                                result = ratio;
                        }
                    }
                }
            }
        }

        return result;
    }

    ADLX_Wrapper bool SetFFXFrameGenRatio(int GPU, int ratio)
    {
        ADLX_RESULT res = ADLX_FAIL;
        bool result = false;

        IADLXGPUListPtr gpus;
        res = g_ADLXHelp.GetSystemServices()->GetGPUs(&gpus);
        if (ADLX_SUCCEEDED(res) && !gpus->Empty())
        {
            IADLX3DSettingsServicesPtr d3dSettingSrv;
            res = g_ADLXHelp.GetSystemServices()->Get3DSettingsServices(&d3dSettingSrv);
            if (ADLX_SUCCEEDED(res))
            {
                IADLX3DSettingsServices3Ptr d3dSettingSrv3(d3dSettingSrv);
                if (d3dSettingSrv3 != nullptr)
                {
                    IADLXGPUPtr gpuInfo;
                    res = gpus->At(GPU, &gpuInfo);
                    if (ADLX_SUCCEEDED(res))
                    {
                        IADLX3DFidelityFXFrameGenUpgradePtr ffxFG;
                        res = d3dSettingSrv3->GetFidelityFXFrameGenUpgrade(gpuInfo, &ffxFG);
                        if (ADLX_SUCCEEDED(res))
                        {
                            adlx_bool supported = false;
                            ffxFG->IsSupported(&supported);
                            if (supported)
                            {
                                res = ffxFG->SetRatio((ADLX_FFX_FRAME_GEN_RATIO)ratio);
                                result = ADLX_SUCCEEDED(res);
                            }
                        }
                    }
                }
            }
        }

        return result;
    }
}

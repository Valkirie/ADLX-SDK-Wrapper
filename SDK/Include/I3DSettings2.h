//
// Copyright (c) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved.
//
//-------------------------------------------------------------------------------------------------

#ifndef ADLX_I3DSETTINGS2_H
#define ADLX_I3DSETTINGS2_H
#pragma once

#include "ADLXStructures.h"
#include "I3DSettings1.h"


//-------------------------------------------------------------------------------------------------
//I3DSetting.2h - Interfaces for ADLX GPU 3DSetting functionality

//3DSharpenDesktop interface
#pragma region IADLX3DImageSharpenDesktop 
#if defined (__cplusplus)
namespace adlx
{
    class ADLX_NO_VTABLE IADLX3DImageSharpenDesktop : public IADLXInterface
    {
    public:
        ADLX_DECLARE_IID(L"IADLX3DImageSharpenDesktop")

        /**
        *@page DOX_IADLX3DImageSharpenDesktop_IsSupported IsSupported
        *@ENG_START_DOX @brief Checks if Sharpen Desktop is supported on a GPU. @ENG_END_DOX
        *
        *@syntax
        *@codeStart
        * @ref ADLX_RESULT    IsSupported (adlx_bool* supported)
        *@codeEnd
        *
        *@params
        * @paramrow{1.,[out],supported,adlx_bool*,@ENG_START_DOX The pointer to a variable where the state of Sharpen Desktop is returned. The variable is __true__ if Sharpen Desktop is supported. The variable is __false__ if Sharpen Desktop is not supported. @ENG_END_DOX}
        *
        *@retvalues
        *@ENG_START_DOX  If the state of Sharpen Desktop is successfully returned, __ADLX_OK__ is returned.<br>
        * If the state of Sharpen Desktop is not successfully returned, an error code is returned.<br>
        * Refer to @ref ADLX_RESULT for success codes and error codes.<br> @ENG_END_DOX
        *
        * @addinfo
        * @ENG_START_DOX
        * Sharpen Desktop restores clarity softened by other effects to all the desktop visuals, for both windowed applications and for applications that run in fullscreen exclusive mode. <br>
        * @ENG_END_DOX
        * 
        *
        *@copydoc IADLX3DImageSharpenDesktop_REQ_TABLE
        *
        */
        virtual ADLX_RESULT     ADLX_STD_CALL IsSupported(adlx_bool* supported) = 0;

        /**
        *@page DOX_IADLX3DImageSharpenDesktop_IsEnabled IsEnabled
        *@ENG_START_DOX @brief Checks if Sharpen Desktop is enabled on a GPU. @ENG_END_DOX
        *
        *@syntax
        *@codeStart
        * @ref ADLX_RESULT    IsEnabled (adlx_bool* enabled)
        *@codeEnd
        *
        *@params
        * @paramrow{1.,[out],enabled,adlx_bool*,@ENG_START_DOX The pointer to a variable where the state of Sharpen Desktop is returned. The variable is __true__ if Sharpen Desktop is enabled. The variable is __false__ if Sharpen Desktop is not enabled. @ENG_END_DOX}
        *
        *@retvalues
        *@ENG_START_DOX  If the state of Sharpen Desktop is successfully returned, __ADLX_OK__ is returned.<br>
        * If the state of Sharpen Desktop is not successfully returned, an error code is returned.<br>
        * Refer to @ref ADLX_RESULT for success codes and error codes.<br> @ENG_END_DOX
        *
        * @addinfo
        * @ENG_START_DOX
        * Sharpen Desktop restores clarity softened by other effects to all the desktop visuals, for both windowed applications and for applications that run in fullscreen exclusive mode. <br>
        * @ENG_END_DOX
        * 
        * @depifc
        * 
        * Sharpen Desktop requires @ref DOX_IADLX3DImageSharpening "AMD Radeon™ Image Sharpening".<br>

        * When AMD Radeon Image sharpening is enabled and Sharpen Desktop is enabled, the sharpening is applied for both windowed applications and for applications that run in fullscreen exclusive mode.<br>

        * When AMD Radeon Image sharpening is enabled and Sharpen Desktop is disabled, the sharpening is only applied to applications that run in fullscreen exclusive mode.<br>

        * When AMD Radeon™ Image sharpening is disabled and Sharpen Desktop is enabled, Sharpen Desktop remains in enabled state, but the value is ignored.<br>

        * @ENG_END_DOX
        *
        *@copydoc IADLX3DImageSharpenDesktop_REQ_TABLE
        *
        */
        virtual ADLX_RESULT     ADLX_STD_CALL IsEnabled(adlx_bool* enabled) = 0;

        /**
        *@page DOX_IADLX3DImageSharpenDesktop_SetEnabled SetEnabled
        *@ENG_START_DOX @brief Sets Sharpen Desktop to enabled or disabled on a GPU. @ENG_END_DOX
        *
        *@syntax
        *@codeStart
        * @ref ADLX_RESULT    SetEnabled (adlx_bool enable)
        *@codeEnd
        *
        *@params
        * @paramrow{1.,[in],enable,adlx_bool,@ENG_START_DOX The new Sharpen Desktop state. Set __true__ to enable Sharpen Desktop. Set __false__ to disable Sharpen Desktop. @ENG_END_DOX}
        *
        *@retvalues
        *@ENG_START_DOX  If the state of Sharpen Desktop is successfully set, __ADLX_OK__ is returned.<br>
        * If the state of Sharpen Desktop is not successfully set, an error code is returned.<br>
        * Refer to @ref ADLX_RESULT for success codes and error codes.<br> @ENG_END_DOX
        *
        * @addinfo
        * @ENG_START_DOX
        * Sharpen Desktop restores clarity softened by other effects to all the desktop visuals, for both windowed applications and for applications that run in fullscreen exclusive mode. <br>
        * @ENG_END_DOX
        * 
        * @depifc
        * 
        * Sharpen Desktop requires @ref DOX_IADLX3DImageSharpening "AMD Radeon™ Image Sharpening".<br>

        * When AMD Radeon Image sharpening is enabled and Sharpen Desktop is enabled, the sharpening is applied for both windowed applications and for applications that run in fullscreen exclusive mode.<br>

        * When AMD Radeon Image sharpening is enabled and Sharpen Desktop is disabled, the sharpening is only applied to applications that run in fullscreen exclusive mode.<br>

        * When AMD Radeon™ Image sharpening is disabled and Sharpen Desktop is enabled, Sharpen Desktop remains in enabled state, but the value is ignored.<br>
        * 
        * @ENG_END_DOX
        *
        *@copydoc IADLX3DImageSharpenDesktop_REQ_TABLE
        *
        */
        virtual ADLX_RESULT     ADLX_STD_CALL SetEnabled(adlx_bool enable) = 0;
    };  //IADLX3DImageSharpenDesktop
    //----------------------------------------------------------------------------------------------
    typedef IADLXInterfacePtr_T<IADLX3DImageSharpenDesktop> IADLX3DImageSharpenDesktopPtr;
} //namespace adlx
#else //__cplusplus
ADLX_DECLARE_IID(IADLX3DImageSharpenDesktop, L"IADLX3DImageSharpenDesktop")

typedef struct IADLX3DImageSharpenDesktop IADLX3DImageSharpenDesktop;

typedef struct IADLX3DImageSharpenDesktopVtbl
{
    //IADLXInterface
    adlx_long(ADLX_STD_CALL* Acquire)(IADLX3DImageSharpenDesktop* pThis);
    adlx_long(ADLX_STD_CALL* Release)(IADLX3DImageSharpenDesktop* pThis);
    ADLX_RESULT(ADLX_STD_CALL* QueryInterface)(IADLX3DImageSharpenDesktop* pThis, const wchar_t* interfaceId, void** ppInterface);

    //IADLX3DImageSharpenDesktop
    ADLX_RESULT(ADLX_STD_CALL* IsSupported) (IADLX3DImageSharpenDesktop* pThis, adlx_bool* supported);
    ADLX_RESULT(ADLX_STD_CALL* IsEnabled) (IADLX3DImageSharpenDesktop* pThis, adlx_bool* enabled);
    ADLX_RESULT(ADLX_STD_CALL* SetEnabled) (IADLX3DImageSharpenDesktop* pThis, adlx_bool enable);
}IADLX3DImageSharpenDesktopVtbl;

struct IADLX3DImageSharpenDesktop { const IADLX3DImageSharpenDesktopVtbl* pVtbl; };

#endif //__cplusplus
#pragma endregion IADLX3DImageSharpenDesktop

//3DSetting Services interface
#pragma region IADLX3DSettingsServices2
#if defined (__cplusplus)
namespace adlx
{
    class ADLX_NO_VTABLE IADLX3DSettingsServices2 : public IADLX3DSettingsServices1
    {
    public:
        ADLX_DECLARE_IID(L"IADLX3DSettingsServices2")

        /**
        *@page DOX_IADLX3DSettingsServices2_GetImageSharpenDesktop GetImageSharpenDesktop
        *@ENG_START_DOX @brief Gets the reference-counted Sharpen Desktop interface of a GPU. @ENG_END_DOX
        *
        *@syntax
        *@codeStart
        * @ref ADLX_RESULT    GetImageSharpenDesktop (@ref DOX_IADLXGPU* pGPU, @ref DOX_IADLX3DImageSharpenDesktop** pp3DImageSharpenDesktop)

        *@codeEnd
        *
        *@params
        *@paramrow{1.,[in] ,pGPU,@ref DOX_IADLXGPU* ,@ENG_START_DOX The pointer to the GPU interface. @ENG_END_DOX}
        *@paramrow{2.,[out] ,pp3DImageSharpenDesktop,@ref DOX_IADLX3DImageSharpenDesktop** ,@ENG_START_DOX The address of a pointer to the returned interface. If the interface is not successfully returned\, the method sets the dereferenced address __*pp3DImageSharpenDesktop__ to __nullptr__. @ENG_END_DOX}
        *
        *@retvalues
        *@ENG_START_DOX  If the interface is successfully returned, __ADLX_OK__ is returned.<br>
        * If the interface is not successfully returned, an error code is returned.<br>
        * Refer to @ref ADLX_RESULT for success codes and error codes.<br> @ENG_END_DOX
        *
        *@detaileddesc
        *@ENG_START_DOX @details The returned interface must be discarded with @ref DOX_IADLXInterface_Release when it is no longer needed. @ENG_END_DOX
        *
        *@addinfo
        *@ENG_START_DOX  In C++, when using ADLX interfaces as smart pointers, there is no need to call @ref DOX_IADLXInterface_Release because smart pointers call it in their internal implementation. @ENG_END_DOX
        *
        *@copydoc IADLX3DSettingsServices2_REQ_TABLE
        *
        */
        virtual ADLX_RESULT         ADLX_STD_CALL GetImageSharpenDesktop(IADLXGPU* pGPU,IADLX3DImageSharpenDesktop** pp3DImageSharpenDesktop) = 0;

    };  //IADLX3DSettingsServices1
    //----------------------------------------------------------------------------------------------
    typedef IADLXInterfacePtr_T<IADLX3DSettingsServices2> IADLX3DSettingsServices2Ptr;
} //namespace adlx
#else //__cplusplus
ADLX_DECLARE_IID (IADLX3DSettingsServices2, L"IADLX3DSettingsServices2")
typedef struct IADLX3DSettingsServices2 IADLX3DSettingsServices2;

typedef struct IADLX3DSettingsServices2Vtbl
{
    //IADLXInterface
    adlx_long (ADLX_STD_CALL *Acquire)(IADLX3DSettingsServices2* pThis);
    adlx_long (ADLX_STD_CALL *Release)(IADLX3DSettingsServices2* pThis);
    ADLX_RESULT (ADLX_STD_CALL *QueryInterface)(IADLX3DSettingsServices2* pThis, const wchar_t* interfaceId, void** ppInterface);

    //IADLX3DSettingsServices
    ADLX_RESULT (ADLX_STD_CALL *GetAntiLag)(IADLX3DSettingsServices2* pThis, IADLXGPU* pGPU, IADLX3DAntiLag** pp3DAntiLag);
    ADLX_RESULT (ADLX_STD_CALL *GetChill)(IADLX3DSettingsServices2* pThis, IADLXGPU* pGPU, IADLX3DChill** pp3DChill);
    ADLX_RESULT (ADLX_STD_CALL *GetBoost)(IADLX3DSettingsServices2* pThis, IADLXGPU* pGPU, IADLX3DBoost** pp3DBoost);
    ADLX_RESULT (ADLX_STD_CALL *GetImageSharpening)(IADLX3DSettingsServices2* pThis, IADLXGPU* pGPU, IADLX3DImageSharpening** pp3DImageSharpening);
    ADLX_RESULT (ADLX_STD_CALL *GetEnhancedSync)(IADLX3DSettingsServices2* pThis, IADLXGPU* pGPU, IADLX3DEnhancedSync** pp3DEnhancedSync);
    ADLX_RESULT (ADLX_STD_CALL *GetWaitForVerticalRefresh)(IADLX3DSettingsServices2* pThis, IADLXGPU* pGPU, IADLX3DWaitForVerticalRefresh** pp3DWaitForVerticalRefresh);
    ADLX_RESULT (ADLX_STD_CALL *GetFrameRateTargetControl)(IADLX3DSettingsServices2* pThis, IADLXGPU* pGPU, IADLX3DFrameRateTargetControl** pp3DFrameRateTargetControl);
    ADLX_RESULT (ADLX_STD_CALL *GetAntiAliasing)(IADLX3DSettingsServices2* pThis, IADLXGPU* pGPU, IADLX3DAntiAliasing** pp3DAntiAliasing);
    ADLX_RESULT (ADLX_STD_CALL *GetMorphologicalAntiAliasing)(IADLX3DSettingsServices2* pThis, IADLXGPU* pGPU, IADLX3DMorphologicalAntiAliasing** pp3DMorphologicalAntiAliasing);
    ADLX_RESULT (ADLX_STD_CALL *GetAnisotropicFiltering)(IADLX3DSettingsServices2* pThis, IADLXGPU* pGPU, IADLX3DAnisotropicFiltering** pp3DAnisotropicFiltering);
    ADLX_RESULT (ADLX_STD_CALL *GetTessellation)(IADLX3DSettingsServices2* pThis, IADLXGPU* pGPU, IADLX3DTessellation** pp3DTessellation);
    ADLX_RESULT (ADLX_STD_CALL *GetRadeonSuperResolution) (IADLX3DSettingsServices2* pThis, IADLX3DRadeonSuperResolution** pp3DRadeonSuperResolution);
    ADLX_RESULT (ADLX_STD_CALL *GetResetShaderCache)(IADLX3DSettingsServices2* pThis, IADLXGPU* pGPU, IADLX3DResetShaderCache** pp3DResetShaderCache);
    ADLX_RESULT (ADLX_STD_CALL *Get3DSettingsChangedHandling)(IADLX3DSettingsServices2* pThis, IADLX3DSettingsChangedHandling** pp3DSettingsChangedHandling);
    ADLX_RESULT(ADLX_STD_CALL* GetAMDFluidMotionFrames) (IADLX3DSettingsServices2* pThis, IADLX3DAMDFluidMotionFrames** pp3DGetAMDFluidMotionFrames);
    ADLX_RESULT(ADLX_STD_CALL* GetImageSharpenDesktop) (IADLX3DSettingsServices2* pThis, IADLXGPU* pGPU, IADLX3DImageSharpenDesktop** pp3DImageSharpenDesktop);

}IADLX3DSettingsServices2Vtbl;

struct IADLX3DSettingsServices2 { const IADLX3DSettingsServices2Vtbl *pVtbl; };
#endif //__cplusplus
#pragma endregion IADLX3DSettingsServices2

//Interface with information on 3D setting changes on a GPU. ADLX passes this to application that registered for 3D setting changed event in the IADLX3DSettingsChangedListener::On3DSettingsChanged()
#pragma region IADLX3DSettingsChangedEvent2
#if defined (__cplusplus)
namespace adlx
{
    class ADLX_NO_VTABLE IADLXGPU;
    class ADLX_NO_VTABLE IADLX3DSettingsChangedEvent2 : public IADLX3DSettingsChangedEvent1
    {
    public:
        ADLX_DECLARE_IID(L"IADLX3DSettingsChangedEvent2")

            /**
            *@page DOX_IADLX3DSettingsChangedEvent2_IsImageSharpenDesktopChanged IsImageSharpenDesktopChanged
            *@ENG_START_DOX @brief Checks for changes to the Sharpen Desktop settings. @ENG_END_DOX
            *
            *@syntax
            *@codeStart
            * adlx_bool    IsImageSharpenDesktopChanged ()
            *@codeEnd
            *
            *@params
            *N/A
            *
            *@retvalues
            *@ENG_START_DOX  If there are any changes to the Sharpen Desktop settings, __true__ is returned.<br>
            * If there are on changes to the Sharpen Desktop settings, __false__ is returned.<br> @ENG_END_DOX
            *
            *@addinfo
            *@ENG_START_DOX
            * __Note:__ To obtain the GPU, use @ref DOX_IADLX3DSettingsChangedEvent_GetGPU.
            * @ENG_END_DOX
            *
            *@copydoc IADLX3DSettingsChangedEvent2_REQ_TABLE
            *
            */
            virtual adlx_bool ADLX_STD_CALL IsImageSharpenDesktopChanged() = 0;

    }; //IADLX3DSettingsChangedEvent
    //----------------------------------------------------------------------------------------------
    typedef IADLXInterfacePtr_T<IADLX3DSettingsChangedEvent2> IADLX3DSettingsChangedEvent2Ptr;
} //namespace adlx
#else //__cplusplus
ADLX_DECLARE_IID(IADLX3DSettingsChangedEvent2, L"IADLX3DSettingsChangedEvent2")
typedef struct IADLX3DSettingsChangedEvent2 IADLX3DSettingsChangedEvent2;

typedef struct IADLX3DSettingsChangedEvent2Vtbl
{
    //IADLXInterface
    adlx_long(ADLX_STD_CALL* Acquire)(IADLX3DSettingsChangedEvent2* pThis);
    adlx_long(ADLX_STD_CALL* Release)(IADLX3DSettingsChangedEvent2* pThis);
    ADLX_RESULT(ADLX_STD_CALL* QueryInterface)(IADLX3DSettingsChangedEvent2* pThis, const wchar_t* interfaceId, void** ppInterface);

    //IADLXChangedEvent
    ADLX_SYNC_ORIGIN(ADLX_STD_CALL* GetOrigin)(IADLX3DSettingsChangedEvent2* pThis);

    // IADLX3DSettingsChangedEvent interface
    ADLX_RESULT(ADLX_STD_CALL* GetGPU)(IADLX3DSettingsChangedEvent2* pThis, IADLXGPU** ppGPU);
    adlx_bool(ADLX_STD_CALL* IsAntiLagChanged)(IADLX3DSettingsChangedEvent2* pThis);
    adlx_bool(ADLX_STD_CALL* IsChillChanged)(IADLX3DSettingsChangedEvent2* pThis);
    adlx_bool(ADLX_STD_CALL* IsBoostChanged)(IADLX3DSettingsChangedEvent2* pThis);
    adlx_bool(ADLX_STD_CALL* IsImageSharpeningChanged)(IADLX3DSettingsChangedEvent2* pThis);
    adlx_bool(ADLX_STD_CALL* IsEnhancedSyncChanged)(IADLX3DSettingsChangedEvent2* pThis);
    adlx_bool(ADLX_STD_CALL* IsWaitForVerticalRefreshChanged)(IADLX3DSettingsChangedEvent2* pThis);
    adlx_bool(ADLX_STD_CALL* IsFrameRateTargetControlChanged)(IADLX3DSettingsChangedEvent2* pThis);
    adlx_bool(ADLX_STD_CALL* IsAntiAliasingChanged)(IADLX3DSettingsChangedEvent2* pThis);
    adlx_bool(ADLX_STD_CALL* IsMorphologicalAntiAliasingChanged)(IADLX3DSettingsChangedEvent2* pThis);
    adlx_bool(ADLX_STD_CALL* IsAnisotropicFilteringChanged)(IADLX3DSettingsChangedEvent2* pThis);
    adlx_bool(ADLX_STD_CALL* IsTessellationModeChanged)(IADLX3DSettingsChangedEvent2* pThis);
    adlx_bool(ADLX_STD_CALL* IsRadeonSuperResolutionChanged)(IADLX3DSettingsChangedEvent2* pThis);
    adlx_bool(ADLX_STD_CALL* IsResetShaderCache)(IADLX3DSettingsChangedEvent2* pThis);
    adlx_bool(ADLX_STD_CALL* IsAMDFluidMotionFramesChanged)(IADLX3DSettingsChangedEvent2* pThis);
    adlx_bool(ADLX_STD_CALL* IsImageSharpenDesktopChanged)(IADLX3DSettingsChangedEvent2* pThis);

} IADLX3DSettingsChangedEvent2Vtbl;

struct IADLX3DSettingsChangedEvent2 { const IADLX3DSettingsChangedEvent2Vtbl* pVtbl; };
#endif //__cplusplus
#pragma endregion IADLX3DSettingsChangedEvent2

#endif //ADLX_I3DSETTINGS2_H

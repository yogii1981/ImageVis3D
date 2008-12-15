/*
   For more information, please see: http://software.sci.utah.edu

   The MIT License

   Copyright (c) 2008 Scientific Computing and Imaging Institute,
   University of Utah.

   
   Permission is hereby granted, free of charge, to any person obtaining a
   copy of this software and associated documentation files (the "Software"),
   to deal in the Software without restriction, including without limitation
   the rights to use, copy, modify, merge, publish, distribute, sublicense,
   and/or sell copies of the Software, and to permit persons to whom the
   Software is furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included
   in all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
   OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
   THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
   DEALINGS IN THE SOFTWARE.
*/


//!    File   : ImageVis3D.h
//!    Author : Jens Krueger
//!             SCI Institute
//!             University of Utah
//!    Date   : September 2008
//
//!    Copyright (C) 2008 SCI Institute

#pragma once

#ifndef IMAGEVIS3D_H
#define IMAGEVIS3D_H

#include <StdDefines.h>

#include "Controller/MasterController.h"

#include "AutoGen/ui_ImageVis3D.h"
#include "RenderWindow.h"
#include "Q1DTransferFunction.h"
#include "Q2DTransferFunction.h"
#include "DebugOut/QTOut.h"
#include <UI/SettingsDlg.h>


class MainWindow : public QMainWindow, protected Ui_MainWindow
{
  Q_OBJECT

  public:
    MainWindow(MasterController& masterController,
	       QWidget* parent = 0,
	       Qt::WindowFlags flags = 0);

    virtual ~MainWindow();
    
    QTOut* GetDebugOut() {return m_DebugOut;}

  public slots:
    void SetRenderProgress(unsigned int iLODCount, unsigned int iCurrentCount, unsigned int iBrickCount, unsigned int iWorkingBrick);

  protected slots:
    void SetCaptureFilename();
    void CaptureFrame();
    void CaptureSequence();
    void CaptureRotation();
    void LoadDataset();
    void LoadDirectory();
    void CloneCurrentView();

    void ToggleRenderWindowView2x2();
    void ToggleRenderWindowViewSingle();

    void Use1DTrans();
    void Use2DTrans();
    void UseIso();
    void DisableAllTrans();

    void Transfer1DSetColors();
    void Transfer1DSetGroups();
    void Transfer1DLoad();
    void Transfer1DSave();
    void Transfer1DCopyTo2DTrans();

    void Transfer2DAddGradient();
    void Transfer2DDeleteGradient();
    void Transfer2DChooseGradientColor();
    void Transfer2DChooseGradientOpacity();
    void Transfer2DLoad();
    void Transfer2DSave();

    void SetUpdateMode();
    void ApplyUpdate();

    void Transfer2DSwatchesChanged();
    void Transfer2DUpdateSwatchButtons();
    void Transfer2DUpdateGradientBox();
    void Transfer2DUpdateGradientButtons();

    void FilterImage();

    bool LoadWorkspace();
    bool SaveWorkspace();
    bool ApplyWorkspace();

    bool LoadGeometry();
    bool SaveGeometry();

    void OpenRecentFile();
    void ClearMRUList();

    void UpdateMenus();
    void SaveDataset();

    void RenderWindowActive(RenderWindow* sender);
    void RenderWindowClosing(RenderWindow* sender);

    void ClearDebugWin();
    void ParseAndExecuteDebugCommand();
    void SetDebugViewMask();

    void CheckForRedraw();
    bool ShowSettings();
    void SetLighting(bool bLighting);

    void Collapse2DWidgets();
    void Expand2DWidgets();
    void SetSampleRate(int iValue);
    void SetIsoValue(int iValue);
    void ToggleGlobalBBox(bool bRenderBBox);
    void ToggleLocalBBox(bool bRenderBBox);

    void SetRescaleFactors();
    virtual void closeEvent(QCloseEvent *event);

    void ShowAbout();

    void ChooseIsoColor();

    void ToggleClearView();
    void SetFocusIsoValue(int iValue);
    void ChooseFocusColor();
    void SetFocusSize(int iValue);
    void SetContextScale(int iValue);
    void SetBorderSize(int iValue);

    void UpdateLockView();
    void LockModalityChange();
    void ChangeLocks();

  private :
    MasterController&                         m_MasterController;
    QString                                   m_strCurrentWorkspaceFilename;
    Q1DTransferFunction*                      m_1DTransferFunction;
    Q2DTransferFunction*                      m_2DTransferFunction;
    QGLWidget*                                m_glShareWidget;
    QTOut*                                    m_DebugOut;
    RenderWindow*                             m_ActiveRenderWin;
    static const unsigned int                 ms_iMaxRecentFiles = 5;
    QAction*                                  m_recentFileActs[ms_iMaxRecentFiles];
    FLOATVECTOR3                              m_vBackgroundColors[2];
    FLOATVECTOR4                              m_vTextColor;
    bool                                      m_bQuickopen;
    unsigned int                              m_iMinFramerate;
    unsigned int                              m_iLODDelay;
    unsigned int                              m_iActiveTS;
    unsigned int                              m_iInactiveTS;

    unsigned int                              m_iBlendPrecisionMode;
    bool                                      m_bPowerOfTwo;
    bool                                      m_bAvoidCompositing;
    bool                                      m_bAutoSaveGEO;
    bool                                      m_bAutoSaveWSP;
    MasterController::EVolumeRendererType     m_eVolumeRendererType;
    bool                                      m_bUpdatingLockView;
    QString                                   m_strLogoFilename;
    int                                       m_iLogoPos;
    bool                                      m_bAutoLockClonedWindow;
    bool                                      m_bAbsoluteViewLocks;

    RenderWindow* CreateNewRenderWindow(QString dataset);
    bool CheckRenderwindowFitness(RenderWindow *renderWin, bool bIfNotOkShowMessageAndCloseWindow=true);
    RenderWindow* GetActiveRenderWindow();

    void SetupWorkspaceMenu();
    bool LoadWorkspace(QString strFilename,
		       bool bSilentFail = false,
		       bool bRetryResource = true);

    bool SaveWorkspace(QString strFilename);

    bool LoadGeometry(QString strFilename,
		      bool bSilentFail = false,
		      bool bRetryResource = true);

    bool SaveGeometry(QString strFilename);


    void setupUi(QMainWindow *MainWindow);

    void UpdateMRUActions();
    void AddFileToMRUList(const QString &fileName);

    void GetDebugViewMask();
      
    void LoadDataset(QString fileName);

    QString GetConvFilename();

    void InitAllWorkspaces();

    bool ParseCommand(std::string strCommand, std::string strParam);
    void CheckSettings();
    void ApplySettings();
    void ApplySettings(RenderWindow* renderWin);
    void SetSampleRateSlider(int iValue);
    void UpdateSampleRateLabel(int iValue);
    void SetIsoValueSlider(int iValue, int iMaxValue);
    void UpdateIsoValLabel(int iValue, int iMaxValue);
    void SetToggleGlobalBBoxLabel(bool bRenderBBox);
    void SetToggleLocalBBoxLabel(bool bRenderBBox);
    void ClearProgressView();

    void ToggleClearViewControls(int iRange);
    void SetFocusIsoValueSlider(int iValue, int iMaxValue);
    void UpdateFocusIsoValLabel(int iValue, int iMaxValue);
    void SetFocusSizeValueSlider(int iValue);
    void SetContextScaleValueSlider(int iValue);
    void SetBorderSizeValueSlider(int iValue);

    void RemoveAllLocks(RenderWindow* sender);
    void RemoveAllLocks(RenderWindow* sender, size_t iLockType);
    bool SetLock(size_t iLockType, RenderWindow* winA, RenderWindow* winB);
    bool IsLockedWith(size_t iLockType, RenderWindow* winA, RenderWindow* winB);

};

#endif // IMAGEVIS3D_H
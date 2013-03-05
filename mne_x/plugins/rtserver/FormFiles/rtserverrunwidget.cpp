//=============================================================================================================
/**
* @file     rtserverrunwidget.cpp
* @author   Christoph Dinh <chdinh@nmr.mgh.harvard.edu>;
*           Matti Hamalainen <msh@nmr.mgh.harvard.edu>
* @version  1.0
* @date     February, 2013
*
* @section  LICENSE
*
* Copyright (C) 2013, Christoph Dinh and Matti Hamalainen. All rights reserved.
*
* Redistribution and use in source and binary forms, with or without modification, are permitted provided that
* the following conditions are met:
*     * Redistributions of source code must retain the above copyright notice, this list of conditions and the
*       following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and
*       the following disclaimer in the documentation and/or other materials provided with the distribution.
*     * Neither the name of the Massachusetts General Hospital nor the names of its contributors may be used
*       to endorse or promote products derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
* PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL MASSACHUSETTS GENERAL HOSPITAL BE LIABLE FOR ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
* PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
* NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*
*
* @brief    Contains the implementation of the RTServerRunWidget class.
*
*/

//*************************************************************************************************************
//=============================================================================================================
// INCLUDES
//=============================================================================================================

#include "rtserverrunwidget.h"
#include "rtserveraboutwidget.h"
#include "../rtserver.h"


//*************************************************************************************************************
//=============================================================================================================
// USED NAMESPACES
//=============================================================================================================

using namespace RTServerPlugin;


//*************************************************************************************************************
//=============================================================================================================
// DEFINE MEMBER METHODS
//=============================================================================================================

RTServerRunWidget::RTServerRunWidget(RTServer *simulator, QWidget *parent)
: QWidget(parent)
, m_pRTServer(simulator)
{
    ui.setupUi(this);

    connect(ui.m_qPushButton_About, SIGNAL(released()), this, SLOT(showAboutDialog()));

    ui.m_qTextBrowser_Information->insertHtml(QString("Sampling Rate: %1sps").arg(m_pRTServer->m_fSamplingRate));
    ui.m_qTextBrowser_Information->insertHtml(QString("<br>Downsampling Factor: %1x").arg(m_pRTServer->m_iDownsamplingFactor));
    ui.m_qTextBrowser_Information->insertHtml(QString("<br><u>ECG I</u>"
            "<br>Enabled: ") + QString(m_pRTServer->m_pRTServerChannel_ECG_I->isEnabled()?"True":"False") +
            QString("<br>Visible: ") + QString(m_pRTServer->m_pRTServerChannel_ECG_I->isVisible()?"True":"False") +
            QString("<br>File: ") + m_pRTServer->m_pRTServerChannel_ECG_I->getChannelFile() +
            QString("<br>Simulation Signal Length: %1s").arg(m_pRTServer->m_pRTServerChannel_ECG_I->getSamples().size()/m_pRTServer->m_fSamplingRate));
    ui.m_qTextBrowser_Information->insertHtml(QString("<br><u>ECG II</u>"
            "<br>Enabled: ") + QString(m_pRTServer->m_pRTServerChannel_ECG_II->isEnabled()?"True":"False") +
            QString("<br>Visible: ") + QString(m_pRTServer->m_pRTServerChannel_ECG_II->isVisible()?"True":"False") +
            QString("<br>File: ") + m_pRTServer->m_pRTServerChannel_ECG_II->getChannelFile() +
            QString("<br>Simulation Signal Length: %1s").arg(m_pRTServer->m_pRTServerChannel_ECG_II->getSamples().size()/m_pRTServer->m_fSamplingRate));
    ui.m_qTextBrowser_Information->insertHtml(QString("<br><u>ECG III</u>"
            "<br>Enabled: ") + QString(m_pRTServer->m_pRTServerChannel_ECG_III->isEnabled()?"True":"False") +
            QString("<br>Visible: ") + QString(m_pRTServer->m_pRTServerChannel_ECG_III->isVisible()?"True":"False") +
            QString("<br>File: ") + m_pRTServer->m_pRTServerChannel_ECG_III->getChannelFile() +
            QString("<br>Simulation Signal Length: %1s").arg(m_pRTServer->m_pRTServerChannel_ECG_III->getSamples().size()/m_pRTServer->m_fSamplingRate));
}


//*************************************************************************************************************

RTServerRunWidget::~RTServerRunWidget()
{

}


//*************************************************************************************************************

void RTServerRunWidget::showAboutDialog()
{
    RTServerAboutWidget aboutDialog(this);
    aboutDialog.exec();
}
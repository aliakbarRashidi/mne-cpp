//=============================================================================================================
/**
* @file     noiseestimation.cpp
* @author   Christoph Dinh <chdinh@nmr.mgh.harvard.edu>;
*           Matti Hamalainen <msh@nmr.mgh.harvard.edu>
* @version  1.0
* @date     July, 2014
*
* @section  LICENSE
*
* Copyright (C) 2014, Christoph Dinh and Matti Hamalainen. All rights reserved.
*
* Redistribution and use in source and binary forms, with or without modification, are permitted provided that
* the following conditions are met:
*     * Redistributions of source code must retain the above copyright notice, this list of conditions and the
*       following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and
*       the following disclaimer in the documentation and/or other materials provided with the distribution.
*     * Neither the name of MNE-CPP authors nor the names of its contributors may be used
*       to endorse or promote products derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
* PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
* PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
* NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*
*
* @brief    Contains the implementation of the NoiseEstimation class.
*
*/

//*************************************************************************************************************
//=============================================================================================================
// INCLUDES
//=============================================================================================================

#include "noiseestimation.h"


//*************************************************************************************************************
//=============================================================================================================
// QT INCLUDES
//=============================================================================================================

#include <QDebug>


//*************************************************************************************************************
//=============================================================================================================
// USED NAMESPACES
//=============================================================================================================

using namespace XMEASLIB;


//*************************************************************************************************************
//=============================================================================================================
// DEFINE MEMBER METHODS
//=============================================================================================================

NoiseEstimation::NoiseEstimation(QObject *parent)
: NewMeasurement(QMetaType::type("NoiseEstimation::SPtr"), parent)
, m_bChInfoIsInit(false)
, m_bContainsValues(false)
{
}


//*************************************************************************************************************

NoiseEstimation::~NoiseEstimation()
{

}


//*************************************************************************************************************

void NoiseEstimation::init(QList<RealTimeSampleArrayChInfo> &chInfo)
{
    m_qListChInfo = chInfo;

    m_bChInfoIsInit = true;
}


//*************************************************************************************************************

void NoiseEstimation::initFromFiffInfo(FiffInfo::SPtr &p_pFiffInfo)
{
    m_qListChInfo.clear();
    m_bChInfoIsInit = false;

    bool t_bIsBabyMEG = false;

    if(p_pFiffInfo->acq_pars == "BabyMEG")
        t_bIsBabyMEG = true;

    for(qint32 i = 0; i < p_pFiffInfo->nchan; ++i)
    {
        RealTimeSampleArrayChInfo initChInfo;
        initChInfo.setChannelName(p_pFiffInfo->chs[i].ch_name);

        // set channel Unit
        initChInfo.setUnit(p_pFiffInfo->chs[i].unit);

        //Treat stimulus channels different
        if(p_pFiffInfo->chs[i].kind == FIFFV_STIM_CH)
        {
//            initChInfo.setUnit("");
            initChInfo.setMinValue(0);
            initChInfo.setMaxValue(1.0e6);
        }
//        else
//        {
////            qDebug() << "kind" << p_pFiffInfo->chs[i].kind << "unit" << p_pFiffInfo->chs[i].unit;

//            //Unit
//            switch(p_pFiffInfo->chs[i].unit)
//            {
//                case 101:
//                    initChInfo.setUnit("Hz");
//                    break;
//                case 102:
//                    initChInfo.setUnit("N");
//                    break;
//                case 103:
//                    initChInfo.setUnit("Pa");
//                    break;
//                case 104:
//                    initChInfo.setUnit("J");
//                    break;
//                case 105:
//                    initChInfo.setUnit("W");
//                    break;
//                case 106:
//                    initChInfo.setUnit("C");
//                    break;
//                case 107:
//                    initChInfo.setUnit("V");
////                    initChInfo.setMinValue(0);
////                    initChInfo.setMaxValue(1.0e-3);
//                    break;
//                case 108:
//                    initChInfo.setUnit("F");
//                    break;
//                case 109:
//                    initChInfo.setUnit("Ohm");
//                    break;
//                case 110:
//                    initChInfo.setUnit("MHO");
//                    break;
//                case 111:
//                    initChInfo.setUnit("Wb");
//                    break;
//                case 112:
//                    initChInfo.setUnit("T");
//                    if(t_bIsBabyMEG)
//                    {
//                        initChInfo.setMinValue(-1.0e-4);
//                        initChInfo.setMaxValue(1.0e-4);
//                    }
//                    else
//                    {
//                        initChInfo.setMinValue(-1.0e-10);
//                        initChInfo.setMaxValue(1.0e-10);
//                    }
//                    break;
//                case 113:
//                    initChInfo.setUnit("H");
//                    break;
//                case 114:
//                    initChInfo.setUnit("Cel");
//                    break;
//                case 115:
//                    initChInfo.setUnit("Lm");
//                    break;
//                case 116:
//                    initChInfo.setUnit("Lx");
//                    break;
//                case 201:
//                    initChInfo.setUnit("T/m");
//                    if(t_bIsBabyMEG)
//                    {
//                        initChInfo.setMinValue(-1.0e-4);
//                        initChInfo.setMaxValue(1.0e-4);
//                    }
//                    else
//                    {
//                        initChInfo.setMinValue(-1.0e-10);
//                        initChInfo.setMaxValue(1.0e-10);
//                    }
//                    break;
//                case 202:
//                    initChInfo.setUnit("Am");
//                    break;
//                default:
//                    initChInfo.setUnit("");
//            }
//        }

        // set channel Kind
        initChInfo.setKind(p_pFiffInfo->chs[i].kind);

        // set channel coil
        initChInfo.setCoil(p_pFiffInfo->chs[i].coil_type);

        m_qListChInfo.append(initChInfo);
    }

    m_pFiffInfo_orig = p_pFiffInfo;

    m_bChInfoIsInit = true;
}


//*************************************************************************************************************

MatrixXd NoiseEstimation::getValue() const
{
    return m_matValue;
}


//*************************************************************************************************************

void NoiseEstimation::setValue(MatrixXd& v)
{
    //Store
    m_matValue = v;
    emit notify();

    if(!m_bContainsValues)
        m_bContainsValues = true;
}


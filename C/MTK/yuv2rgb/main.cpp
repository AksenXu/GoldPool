/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein is
 * confidential and proprietary to MediaTek Inc. and/or its licensors. Without
 * the prior written permission of MediaTek inc. and/or its licensors, any
 * reproduction, modification, use or disclosure of MediaTek Software, and
 * information contained herein, in whole or in part, shall be strictly
 * prohibited.
 *
 * MediaTek Inc. (C) 2018. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER
 * ON AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL
 * WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR
 * NONINFRINGEMENT. NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH
 * RESPECT TO THE SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY,
 * INCORPORATED IN, OR SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES
 * TO LOOK ONLY TO SUCH THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO.
 * RECEIVER EXPRESSLY ACKNOWLEDGES THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO
 * OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES CONTAINED IN MEDIATEK
 * SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE
 * RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S
 * ENTIRE AND CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE
 * RELEASED HEREUNDER WILL BE, AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE
 * MEDIATEK SOFTWARE AT ISSUE, OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE
 * CHARGE PAID BY RECEIVER TO MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 * The following software/firmware and/or related documentation ("MediaTek
 * Software") have been modified by MediaTek Inc. All revisions are subject to
 * any receiver's applicable license agreements with MediaTek Inc.
 */
#include <stdio.h>
#include <stdlib.h>
#include <utils/Errors.h>
#include <utils/List.h>
#include <utils/RefBase.h>

#include <mtkcam/utils/imgbuf/IIonImageBufferHeap.h>
#include <mtkcam/utils/std/Format.h>
#include <mtkcam/drv/iopipe/SImager/IImageTransform.h>
#include <mtkcam3/feature/utils/ImageBufferUtils.h>
//
using namespace NSCam;
using namespace android;
using namespace std;
using namespace NSCam::NSIoPipe::NSSImager;

/******************************************************************************
*
******************************************************************************/

void testYUV2RGB(int argc, char** argv)
{
    if(argc < 5) {
        printf("%s YUVFile RGBFile width height\n", argv[0]);
        return;
    }
    
    FILE* yuvFile = fopen(argv[1], "rb");
    FILE* rgbFile = fopen(argv[2], "wb");
    size_t width = atoi(argv[3]);
    size_t height = atoi(argv[4]);
    
    printf("convert YUV %s %zdx%zd => RGB %s\n", argv[1], width, height, argv[2]);
    
    int yuvSize = width * height * 3 / 2;
    int rgbSize = width * height * 3;
    
    char* yuvData = (char*) malloc(yuvSize);
    char* rgbData = (char*) malloc(rgbSize);
    
    fread(yuvData, yuvSize, 1, yuvFile);
    
    sp<IImageBuffer> yuvBuffer = NULL;
    sp<IImageBuffer> rgbBuffer = NULL;
    ImageBufferUtils::getInstance().allocBuffer(yuvBuffer, width, height, eImgFmt_NV12);
    ImageBufferUtils::getInstance().allocBuffer(rgbBuffer, width, height, eImgFmt_RGB888);
            
    size_t yuv_plane_count = yuvBuffer->getPlaneCount();
    size_t rgb_plane_count = rgbBuffer->getPlaneCount();
    printf("yuv_plane_count %zd rgb_plane_count %zd\n", yuv_plane_count, rgb_plane_count);
    
    MRect roi;
    roi.p = MPoint(0,0);
    roi.s = yuvBuffer->getImgSize();
    printf("roi p %d,%d size %dx%d\n", roi.p.x, roi.p.y, roi.s.w, roi.s.h);

    MINTPTR y_vir_addr = yuvBuffer->getBufVA(0);
    memcpy((void*)y_vir_addr, yuvData, width * height);
    
    MINTPTR uv_vir_addr = yuvBuffer->getBufVA(1);
    memcpy((void*)uv_vir_addr, yuvData + width * height, width * height / 2);
    
    //sync cache?
    yuvBuffer->syncCache();

    auto startTime = ::systemTime();
    IImageTransform * transformer = IImageTransform::createInstance("YUV2RGB");
    MBOOL ret = transformer->execute(yuvBuffer.get(), rgbBuffer.get(), NULL, roi, 0, 0xFFFFFFFF);
    if(!ret) {
        printf("Failed to transform output buffer\n");
    }
    auto elapsedTime = ::systemTime() - startTime;
    
    printf("transformer->execute spend %d ms\n", (int)(elapsedTime / 1e6));

    //sync cache?
    rgbBuffer->syncCache();

    MINTPTR rgb_vir_addr = rgbBuffer->getBufVA(0);    
    //printf("rgb_vir_addr 0x%lx\n", rgb_vir_addr);
    //memset((void*)rgb_vir_addr, 0, rgbSize);
    fwrite((const void*)rgb_vir_addr, rgbSize, 1, rgbFile);

    free(yuvData);
    free(rgbData);
    fclose(yuvFile);
    fclose(rgbFile);
    
    ImageBufferUtils::getInstance().deallocBuffer(yuvBuffer);
    ImageBufferUtils::getInstance().deallocBuffer(rgbBuffer);
}

/******************************************************************************
 *
 ******************************************************************************/
int main(int argc, char** argv)
{
    testYUV2RGB(argc, argv);

    return 0;
}


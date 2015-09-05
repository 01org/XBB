/*
Copyright (c) 2014, Intel Corporation

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of Intel Corporation nor the names of its contributors
      may be used to endorse or promote products derived from this software
      without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <stdio.h>
#include <iostream>

#include <CATCH/catch.hpp>

#include <xbb/Transforms.h>
#include "ref/Matrix4x4.h"
#include "ScopedTimer.h"
#include "TestValues.h"
#include "TransformsAreClose.h"


using namespace xbb;


static const char * sInverseSxSHName = "Inverse S*SH";
TEST_CASE(sInverseSxSHName, "")
{
    // time inverse S*SH with reference vs XBB
    volatile TestValues tv = TestValues::get();

    const int repeatCount = 1000000;
    
    ref::Matrix4x4 refFinalTransform;
    // Compose a transform using traditional OO approach
    double refTime = 0.0;
    {
        ScopedTimer timer(refTime);
        XBB_INLINE_BLOCK
        {                    
            for (int rep=0; rep < repeatCount; ++rep) {
                ref::Matrix4x4 S;
                S.makeScale(tv.scaleX, tv.scaleY, tv.scaleZ);

                ref::Matrix4x4 SH;
                SH.makeShear3(tv.shearX, tv.shearY, tv.shearZ);

                refFinalTransform = (S*SH).inverse();

            }
        }
    }
    
    // Compose a transform using XBB
    xbb::Matrix4x3 xbbFinalTransform;
    double xbbTime = 0.0;
    {
        ScopedTimer timer(xbbTime);
        XBB_INLINE_BLOCK
        {                    
            for (int rep=0; rep < repeatCount; ++rep) {

                xbb::Scale S(tv.scaleX, tv.scaleY, tv.scaleZ);
                xbb::Shear3 SH(tv.shearX, tv.shearY, tv.shearZ);

                (S*SH).inverse().to(xbbFinalTransform);

            }
        }
    }
        
    
    std::cout << sInverseSxSHName << ":"<< std::endl;
//    std::cout << "Ref Transform " << refFinalTransform << std::endl;
//    std::cout << "xbb Transform " << xbbFinalTransform << std::endl;
    TRANSFORMS_ARE_CLOSE(xbbFinalTransform, refFinalTransform);
    
    std::cout << "    ref Time " << refTime << std::endl;
    std::cout << "    xbb Time " << xbbTime << std::endl;
    std::cout << "    speedup " << refTime/xbbTime << "x" << std::endl << std::endl;
}

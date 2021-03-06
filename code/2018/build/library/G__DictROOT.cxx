// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME G__DictROOT
#define R__NO_DEPRECATION

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "RConfig.h"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// The generated code does not explicitly qualifies STL entities
namespace std {} using namespace std;

// Header files passed as explicit arguments
#include "/media/felix/flush/ALICE_TRD/code/2018/library/include/analysis_base.hpp"
#include "/media/felix/flush/ALICE_TRD/code/2018/library/include/analysis_plots.hpp"
#include "/media/felix/flush/ALICE_TRD/code/2018/library/include/analysis_working.hpp"

// Header files passed via #pragma extra_include

namespace {
  void TriggerDictionaryInitialization_DictROOT_Impl() {
    static const char* headers[] = {
"/media/felix/flush/ALICE_TRD/code/2018/library/include/analysis_base.hpp",
"/media/felix/flush/ALICE_TRD/code/2018/library/include/analysis_plots.hpp",
"/media/felix/flush/ALICE_TRD/code/2018/library/include/analysis_working.hpp",
0
    };
    static const char* includePaths[] = {
"/media/felix/flush/ALICE_TRD/code/2018/library",
"/home/felix/Downloads/root_install/include/",
"/media/felix/flush/ALICE_TRD/code/2018/build/library/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "DictROOT dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "DictROOT dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "/media/felix/flush/ALICE_TRD/code/2018/library/include/analysis_base.hpp"
#include "/media/felix/flush/ALICE_TRD/code/2018/library/include/analysis_plots.hpp"
#include "/media/felix/flush/ALICE_TRD/code/2018/library/include/analysis_working.hpp"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("DictROOT",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_DictROOT_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_DictROOT_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_DictROOT() {
  TriggerDictionaryInitialization_DictROOT_Impl();
}

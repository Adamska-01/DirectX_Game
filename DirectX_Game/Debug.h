#pragma once
#include <dxerr.h>


//Custom asserts 
#if _DEBUG

#define DBG_HALT { __debugbreak(); }

#define DBG_ASSERT_MSG_H(h) { if(h != S_OK) { DBG_HALT; } } // h = HRESULT
#define DBG_ASSERT_MSG_STR(s) { if(s == "FILE NOT LOADED") { DBG_HALT; } } // f = string 

#endif
#pragma once

#define SAFE_RELEASE(x) if( x ) { (x)->Release(); (x) = NULL; }
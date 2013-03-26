#pragma once

#define SAFE_RELEASE(x) if( x ) { (x)->Release(); (x) = NULL; }

#define SAFE_DELETE(x) if( x ) { delete (x); (x) = NULL; }
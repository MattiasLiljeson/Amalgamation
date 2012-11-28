// =======================================================================================
//                                      Buffer
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Wrapper for buffer objects
///        
/// # Buffer
/// Detailed description.....
/// Created on: 28-11-2012 
///---------------------------------------------------------------------------------------
#pragma once

class Buffer
{
public:
	/*
	enum BUFFER_TYPE
	{
		VERTEX_BUFFER,
		INDEX_BUFFER,
		CONSTANT_BUFFER_VS,
		CONSTANT_BUFFER_GS,
		CONSTANT_BUFFER_PS,
		BUFFER_TYPE_COUNT
	};

	enum BUFFER_USAGE
	{
		BUFFER_DEFAULT,
		BUFFER_STREAM_OUT_TARGET,
		BUFFER_CPU_WRITE,
		BUFFER_CPU_WRITE_DISCARD,
		BUFFER_CPU_READ,
		BUFFER_USAGE_COUNT
	};

	struct BUFFER_INIT_DESC
	{
		BUFFER_TYPE		Type;
		UINT32			NumElements;
		UINT32			ElementSize;
		BUFFER_USAGE	Usage;
		void*			InitData;

		BUFFER_INIT_DESC()
		{
			InitData = NULL;
		}
	};
	*/

	Buffer();
	virtual ~Buffer();
protected:

private:

};
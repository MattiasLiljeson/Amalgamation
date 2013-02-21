#pragma once

template<typename T, unsigned int N>
class NumericBuffer
{
public:
	static const int VAL_CNT = N;

	NumericBuffer( T p_initVal = 0.0f );
	virtual ~NumericBuffer();

	void setAllVals( T p_val );
	T getAvg(); ///< Works well over the network. Long falloff
	T getLog(); ///< Small falloff. Works bad over the network.
	void addVal( T p_val );

private:
	float m_vals[VAL_CNT];
	int m_currIdx;
};

template<typename T, unsigned int N>
NumericBuffer<T,N>::NumericBuffer( T p_initVal )
{
	m_currIdx = 0;
	setAllVals( p_initVal );
}

template<typename T, unsigned int N>
NumericBuffer<T,N>::~NumericBuffer()
{

}

template<typename T, unsigned int N>
void NumericBuffer<T,N>::setAllVals( T p_val )
{
	for( int i=0; i<VAL_CNT; i++ ) {
		m_vals[i] = p_val;
	}
}

template<typename T, unsigned int N>
T NumericBuffer<T,N>::getAvg()
{
	float sum = 0.0f;
	for( int i=0; i<VAL_CNT; i++ ) {
		sum += m_vals[i];
	}
	return sum/VAL_CNT;
}

template<typename T, unsigned int N>
T NumericBuffer<T,N>::getLog()
{
	float mult = 0.5f;
	float sum = 0.0f;
	int idx = m_currIdx;
	int iterations = 0;
	while( iterations<VAL_CNT ){
		iterations++;
		sum += m_vals[idx]*mult;
		mult /= 2.0f;
		idx++;
		idx %= VAL_CNT;
	}
	return sum;
}

template<typename T, unsigned int N>
void NumericBuffer<T,N>::addVal( T p_val )
{
	m_currIdx++;
	m_currIdx %= VAL_CNT;
	m_vals[m_currIdx] = p_val;
}
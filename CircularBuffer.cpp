/*
  ==============================================================================

    CircularBuffer.cpp
    Created: 27 Mar 2020 1:01:27pm
    Author:  Henric Hiljanen

  ==============================================================================
*/

#include "CircularBuffer.h"
#include "PluginProcessor.h"

//==============================================================================

// Use when sample rate is unknown
CircularBuffer::CircularBuffer()
{
    /*
    _BufferLeft = nullptr;
    _BufferRight = nullptr;
    _WriteHead = 0;
    _ReadHead = 0;
    _SizeInSamples = 0;
     */
}

CircularBuffer::CircularBuffer(int maxDelayInSec, int sampleRate)
{
    _SizeInSamples = sampleRate * maxDelayInSec;
    
    if (_BufferLeft != nullptr)
    {
        delete [] _BufferLeft;
    }
    
    if (_BufferRight != nullptr)
    {
        delete [] _BufferRight;
    }
    
    _BufferLeft = new float[_SizeInSamples];
    _BufferRight = new float[_SizeInSamples];
    
    //_BufferLeft.reset(new float[_SizeInSamples]);
    //_BufferRight.reset(new float[_SizeInSamples]);
    initBuffer();
    
    _WriteHead = 0;
    _ReadHead = 0;
}

CircularBuffer::CircularBuffer(int sizeInSamples)
{
    _SizeInSamples = sizeInSamples;
    
    if (_BufferLeft != nullptr)
    {
        delete [] _BufferLeft;
    }
    
    if (_BufferRight != nullptr)
    {
        delete [] _BufferRight;
    }
    
    _BufferLeft = new float[_SizeInSamples];
    _BufferRight = new float[_SizeInSamples];
    
    //_BufferLeft.reset(new float[_SizeInSamples]);
    //_BufferRight.reset(new float[_SizeInSamples]);
    initBuffer();
    
    _WriteHead = 0;
    _ReadHead = 0;
}

CircularBuffer::~CircularBuffer()
{
    
    if (_BufferLeft != nullptr)
    {
        delete [] _BufferLeft;
        _BufferLeft = nullptr;
    }
    
    if (_BufferRight != nullptr)
    {
        delete [] _BufferRight;
        _BufferRight = nullptr;
    }
    
    _SizeInSamples = 0;
    _WriteHead = 0;
    _ReadHead = 0;
    
}

void CircularBuffer::setSize(int sampleRate, int maxDelayInSec)
{
    _SizeInSamples = sampleRate * maxDelayInSec;
}

bool CircularBuffer::isInstantiated()
{
    if (!(_BufferLeft != nullptr && _BufferRight != nullptr))
    {
        return false;
    }
    return true;
}

void CircularBuffer::add(float sampleL, float sampleR)
{
    _BufferLeft[_WriteHead] = sampleL;
    _BufferRight[_WriteHead] = sampleR;
    
    if (_WriteHead == (_SizeInSamples - 1) )
    {
        _WriteHead = 0;
    }
    else
    {
        _WriteHead++;
    }
    
}

void CircularBuffer::updateReadHead(float delayTimeInSamples)
{
    _ReadHead = _WriteHead - delayTimeInSamples;
    
    if ( _ReadHead < 0 )
    {
        _ReadHead += _SizeInSamples;
    }
}

float CircularBuffer::getReadHeadSample(int channel)
{
    if ( channel == LEFT_CHANNEL)
        return _BufferLeft[(int)_ReadHead];
    return _BufferRight[(int)_ReadHead];
}

float CircularBuffer::getReadHead()
{
    return _ReadHead;
}

float CircularBuffer::getSample(int index, int channel)
{
    if (index >= 0 && index < _SizeInSamples)
    {
        if (channel == LEFT_CHANNEL)
            return _BufferLeft[index];
        return _BufferRight[index];
    }
    return 0;
}

int CircularBuffer::getSize()
{
    return _SizeInSamples;
}

void CircularBuffer::setSample(int index, float newValueLeft, float newValueRight)
{
    if( index >= _SizeInSamples-1 )
        index = 0;
    
    _BufferLeft[index] = newValueLeft;
    _BufferRight[index] = newValueRight;
}

void CircularBuffer::initBuffer()
{
    if (_SizeInSamples > 0)
    {
        for (int i = 0; i < _SizeInSamples; i++)
        {
            _BufferLeft[i] = 0;
            _BufferRight[i] = 0;
        }
    }
}

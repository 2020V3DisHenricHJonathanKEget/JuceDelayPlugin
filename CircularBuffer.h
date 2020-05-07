/*
  ==============================================================================

    CircularBuffer.h
    Created: 27 Mar 2020 1:01:38pm
    Author:  Henric Hiljanen

  ==============================================================================
*/

#include <memory>

using namespace std;

class CircularBuffer {
public:
    CircularBuffer();
    CircularBuffer(int maxDelayInSec, int sampleRate);
    CircularBuffer(int sizeInSamples);
    ~CircularBuffer();
    void setSize(int sampleRate, int maxDelayInSec);
    void setSample(int index, float newValueLeft, float newValueRight);
    float getRight(int index);
    float getLeft(int index);
    bool isInstantiated();
    void add(float sampleL, float sampleR);
    void updateReadHead(float delayTimeInSamples);
    float getReadHeadSample(int channel);
    float getReadHead();
    float getSample(int index, int channel);
    int getSize();
    void initBuffer();
    
private:
    //std::unique_ptr<float []> _BufferLeft = nullptr;
    //std::unique_ptr<float []> _BufferRight = nullptr;
    float* _BufferLeft = nullptr;
    float* _BufferRight = nullptr;
    int _SizeInSamples = 0;
    int _WriteHead = 0;
    float _ReadHead = 0;
};

#pragma once

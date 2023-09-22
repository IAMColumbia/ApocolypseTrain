// Fill out your copyright notice in the Description page of Project Settings.


#include "PrintActor.h"


#ifdef TEXT
#undef TEXT
#endif
#include "D:\Epic Games D drive\Epic Games\UE_5.0\Engine\Source\ThirdParty\Windows\XAudio2_9\Include\xaudio2redist.h"

#ifdef _XBOX //Big-Endian
#define fourccRIFF 'RIFF'
#define fourccDATA 'data'
#define fourccFMT 'fmt '
#define fourccWAVE 'WAVE'
#define fourccXWMA 'XWMA'
#define fourccDPDS 'dpds'
#endif

#ifndef _XBOX //Little-Endian
#define fourccRIFF 'FFIR'
#define fourccDATA 'atad'
#define fourccFMT ' tmf'
#define fourccWAVE 'EVAW'
#define fourccXWMA 'AMWX'
#define fourccDPDS 'sdpd'
#endif
HRESULT FindChunk(HANDLE hFile, DWORD fourcc, DWORD& dwChunkSize, DWORD& dwChunkDataPosition)
{
    HRESULT hr = S_OK;
    if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN))
        return HRESULT_FROM_WIN32(GetLastError());

    DWORD dwChunkType;
    DWORD dwChunkDataSize;
    DWORD dwRIFFDataSize = 0;
    DWORD dwFileType;
    DWORD bytesRead = 0;
    DWORD dwOffset = 0;

    while (hr == S_OK)
    {
        DWORD dwRead;
        if (0 == ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL))
            hr = HRESULT_FROM_WIN32(GetLastError());

        if (0 == ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL))
            hr = HRESULT_FROM_WIN32(GetLastError());

        switch (dwChunkType)
        {
        case fourccRIFF:
            dwRIFFDataSize = dwChunkDataSize;
            dwChunkDataSize = 4;
            if (0 == ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL))
                hr = HRESULT_FROM_WIN32(GetLastError());
            break;

        default:
            if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT))
                return HRESULT_FROM_WIN32(GetLastError());
        }

        dwOffset += sizeof(DWORD) * 2;

        if (dwChunkType == fourcc)
        {
            dwChunkSize = dwChunkDataSize;
            dwChunkDataPosition = dwOffset;
            return S_OK;
        }

        dwOffset += dwChunkDataSize;

        if (bytesRead >= dwRIFFDataSize) return S_FALSE;

    }

    return S_OK;

}

HRESULT ReadChunkData(HANDLE hFile, void* buffer, DWORD buffersize, DWORD bufferoffset)
{
    HRESULT hr = S_OK;
    if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, bufferoffset, NULL, FILE_BEGIN))
        return HRESULT_FROM_WIN32(GetLastError());
    DWORD dwRead;
    if (0 == ReadFile(hFile, buffer, buffersize, &dwRead, NULL))
        hr = HRESULT_FROM_WIN32(GetLastError());
    return hr;
}

HRESULT mainaudio() {

    HRESULT hr;
    hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);


    IXAudio2* pXAudio2 = nullptr;
    hr = XAudio2Create(&pXAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
    IXAudio2MasteringVoice* pMasterVoice = nullptr;
    hr = pXAudio2->CreateMasteringVoice(&pMasterVoice);


    WAVEFORMATEXTENSIBLE wfx = { 0 };
    XAUDIO2_BUFFER buffer = { 0 };

#ifdef _XBOX
    char* strFileName = "game:\\media\\MusicMono.wav";
#else
    //TCHAR* strFileName = _TEXT("media\\MusicMono.wav");
#endif
    // Open the file
    HANDLE hFile = CreateFile(
        (LPCWSTR)"C:\\All Projects C Drive\\Visual Studio\\RhythmPrototypeXAudio2\\AudioFile\\waffles 146bpm cminor.wav",
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        0,
        NULL);

    //Locate the 'RIFF' chunk in the audio file, and check the file type.
    DWORD dwChunkSize;
    DWORD dwChunkPosition;
    //check the file type, should be fourccWAVE or 'XWMA'
    FindChunk(hFile, fourccRIFF, dwChunkSize, dwChunkPosition);
    DWORD filetype;
    ReadChunkData(hFile, &filetype, sizeof(DWORD), dwChunkPosition);

    //Locate the 'fmt ' chunk, and copy its contents into a WAVEFORMATEXTENSIBLE structure.
    FindChunk(hFile, fourccFMT, dwChunkSize, dwChunkPosition);
    ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);
    //Locate the 'data' chunk, and read its contents into a buffer.
    //fill out the audio data buffer with the contents of the fourccDATA chunk
    FindChunk(hFile, fourccDATA, dwChunkSize, dwChunkPosition);
    BYTE* pDataBuffer = new BYTE[dwChunkSize];
    ReadChunkData(hFile, pDataBuffer, dwChunkSize, dwChunkPosition);
    //Populate an XAUDIO2_BUFFER structure.
    buffer.AudioBytes = dwChunkSize;  //size of the audio buffer in bytes
    buffer.pAudioData = pDataBuffer;  //buffer containing audio data
    buffer.Flags = XAUDIO2_END_OF_STREAM; // tell the source voice not to expect any data after this buffer

    IXAudio2SourceVoice* pSourceVoice;
    if (FAILED(hr = pXAudio2->CreateSourceVoice(&pSourceVoice, (WAVEFORMATEX*)&wfx))) return hr;

    if (FAILED(hr = pSourceVoice->SubmitSourceBuffer(&buffer)))
        return hr;

    if (FAILED(hr = pSourceVoice->Start(0)))
        return hr;

    return hr;
}

// Sets default values
APrintActor::APrintActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
}

// Called when the game starts or when spawned
void APrintActor::BeginPlay()
{
	Super::BeginPlay();
	PrintHello();

    mainaudio();
}

// Called every frame
void APrintActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APrintActor::PrintHello() {
	UE_LOG(LogTemp, Warning, TEXT("Hello World!"));
	GEngine->AddOnScreenDebugMessage(1, 3, FColor::Cyan, TEXT("POOPY PANTS IN MY UNDERBOTTOMS"));
}


#ifdef TEXT
#undef TEXT // Delete macro Win32
#endif

#define TEXT(quote) __ TEXT(quote)
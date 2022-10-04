/*
 * Copyright © 2011-2020 Frictional Games
 * 
 * This file is part of Amnesia: A Machine For Pigs.
 * 
 * Amnesia: A Machine For Pigs is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version. 

 * Amnesia: A Machine For Pigs is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Amnesia: A Machine For Pigs.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifdef WIN32
#pragma comment(lib, "fmodvc.lib")
#endif

#include "impl/LowLevelSoundFmod.h"
#include "system/String.h"
#include "impl/FmodSoundData.h"

#include "math/Math.h"

#include "system/LowLevelSystem.h"

#include <fmod.hpp>

namespace hpl {

	//-----------------------------------------------------------------------

	cSoundDeviceIdentifierFmod::cSoundDeviceIdentifierFmod(FMOD::System* apSystem, int alID)
		: msName(256, '\0')
	{
		if(apSystem->getDriverInfo(alID, msName.data(), msName.size(), nullptr, nullptr, nullptr, nullptr) != FMOD_OK)
			Error(" Unable to get FMOD driver info.\n");

		mbDefault = alID == 0;
	}

	//-----------------------------------------------------------------------

	void iLowLevelSound::PopulateAvailableSoundDevices(tSoundDeviceVec& avSoundDeviceVec)
	{
		FMOD::System* system = nullptr;
    	if(FMOD::System_Create(&system) != FMOD_OK)
		{
			FatalError(" Unable to create FMOD system.");
			return;
		}

		STLDeleteAll(avSoundDeviceVec);

		int alNumDrivers = 0;
		if(system->getNumDrivers(&alNumDrivers) != FMOD_OK)
		{
			FatalError(" Unable to get FMOD number of output drivers available.");
			return;
		}

		for(int i = 0; i < alNumDrivers; ++i)
		{
			auto device = hplNew(cSoundDeviceIdentifierFmod, (system, i));
			avSoundDeviceVec.push_back(device);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// CONSTRUCTORS
	//////////////////////////////////////////////////////////////////////////

	//-----------------------------------------------------------------------

	cLowLevelSoundFmod::cLowLevelSoundFmod()
	{
		mvFormats[0] = "WAV";
		mvFormats[1] = "OGG";
		mvFormats[2] = "MP3";
		mvFormats[3] = "";

		mpSystem = nullptr;
	}

	//-----------------------------------------------------------------------

	cLowLevelSoundFmod::~cLowLevelSoundFmod()
	{
		if(mpSystem && mpSystem->close() != FMOD_OK)
			Error(" Unable to close FMOD system.\n");

		if(mpSystem && mpSystem->release() != FMOD_OK)
			Error(" Unable to release FMOD system.\n");
	}

	//-----------------------------------------------------------------------

	//////////////////////////////////////////////////////////////////////////
	// PUBLIC METHOD
	//////////////////////////////////////////////////////////////////////////

	//-----------------------------------------------------------------------

	iSoundData* cLowLevelSoundFmod::LoadSoundData(const tString& asName, const tWString& asFilePath,
												const tString& asType, bool abStream,bool abLoopStream)
	{
		cFmodSoundData* pSoundData = new cFmodSoundData(mpSystem, asName,abStream);
		
		pSoundData->SetLoopStream(abLoopStream);
		
		if(pSoundData->CreateFromFile(asFilePath)==false)
		{
			delete pSoundData;
			return NULL;
		}

		return pSoundData;
	}

	//-----------------------------------------------------------------------

	void cLowLevelSoundFmod::GetSupportedFormats(tStringList &alstFormats)
	{
		int lPos = 0;

		while(mvFormats[lPos]!="")
		{
			alstFormats.push_back(mvFormats[lPos]);
			lPos++;
		}
	}
	//-----------------------------------------------------------------------
	
	void cLowLevelSoundFmod::UpdateSound(float afTimeStep)
	{
		mpSystem->update();
	}
	
	//-----------------------------------------------------------------------

	void cLowLevelSoundFmod::SetListenerAttributes(const cVector3f &avPos,const cVector3f &avVel,
							const cVector3f &avForward,const cVector3f &avUp)
	{
		mvListenerPosition = avPos;
		mvListenerVelocity = avVel;
		mvListenerForward = avForward;
		mvListenerUp = avUp;

		mvListenerRight = cMath::Vector3Cross(mvListenerForward,mvListenerUp);

		m_mtxListener = cMatrixf(
				-mvListenerRight.x, -mvListenerRight.y,-mvListenerRight.z, avPos.x,
				-mvListenerUp.x, -mvListenerUp.y,-mvListenerUp.z, avPos.y,
				-mvListenerForward.x, -mvListenerForward.y,-mvListenerForward.z, avPos.z,
				0, 0,0, 1
			);

		FMOD_VECTOR fmodPosition{avPos.x, avPos.y, avPos.z};
		FMOD_VECTOR fmodVelocity{0.0f, 0.0f, 0.0f};
		FMOD_VECTOR fmodForward{mvListenerForward.x, mvListenerForward.y, mvListenerForward.z};
		FMOD_VECTOR fmodUp{mvListenerUp.x, mvListenerUp.y, mvListenerUp.z};

		if(mpSystem->set3DListenerAttributes(0, &fmodPosition, &fmodVelocity, &fmodForward, &fmodUp) != FMOD_OK)
			Error(" Unable to set FMOD listener attributes\n");
	}

	//-----------------------------------------------------------------------

	void cLowLevelSoundFmod::SetListenerPosition(const cVector3f &avPos)
	{
		mvListenerPosition = avPos;
	
		FMOD_VECTOR fmodPosition{avPos.x, avPos.y, avPos.z};
		FMOD_VECTOR fmodVelocity{mvListenerVelocity.x, mvListenerVelocity.y, mvListenerVelocity.z};
		FMOD_VECTOR fmodForward{mvListenerForward.x, mvListenerForward.y, mvListenerForward.z};
		FMOD_VECTOR fmodUp{mvListenerUp.x, mvListenerUp.y, mvListenerUp.z};

		if(mpSystem->set3DListenerAttributes(0, &fmodPosition, &fmodVelocity, &fmodForward, &fmodUp) != FMOD_OK)
			Error(" Unable to set FMOD listener attributes\n");
	}
	
	//-----------------------------------------------------------------------
		
	void cLowLevelSoundFmod::SetListenerAttenuation (bool abEnabled)
	{
		mbListenerAttenuation = abEnabled;
	}

	//-----------------------------------------------------------------------

	void cLowLevelSoundFmod::SetSetRolloffFactor(float afFactor)
	{
		//FSOUND_3D_SetRolloffFactor(afFactor);
	}
	
	//-----------------------------------------------------------------------

	void cLowLevelSoundFmod::SetVolume(float afVolume)
	{
		mfVolume = afVolume;

		int lVol = (int)(255.0f*afVolume);

        //FSOUND_SetSFXMasterVolume(lVol);
	}

	//-----------------------------------------------------------------------

	void cLowLevelSoundFmod::Init(int alSoundDeviceID, bool abUseEnvAudio,int alMaxChannels, 
					int alStreamUpdateFreq, bool abUseThreading, bool abUseVoiceManagement,
					int alMaxMonoSourceHint, int alMaxStereoSourceHint,
					int alStreamingBufferSize, int alStreamingBufferCount, bool abEnableLowLevelLog)
	{
		Log(" Initializing FMOD.\n");

    	if(FMOD::System_Create(&mpSystem) != FMOD_OK)
			FatalError("  Unable to create FMOD system.");
	
    	if(mpSystem->init(alMaxChannels, FMOD_INIT_NORMAL, nullptr) != FMOD_OK)
			FatalError("  Unable to initialize FMOD system.");
		
		//Setup channel limit.
		int lChannels = 0;
		auto result = mpSystem->getSoftwareChannels(&lChannels);
		Log("  Number of software channels: %d\n", lChannels);

		//Default listener settings.
		FMOD_VECTOR Pos{0.0f, 0.0f, 0.0f};
		FMOD_VECTOR Vel{0.0f, 0.0f, 0.0f};

		FMOD_VECTOR Forward{0.0f, 0.0f, -1.0f};
		FMOD_VECTOR Up{0.0f, 1.0f, 0.0f};
		
		mvListenerForward = cVector3f(0,0,-1);
		mvListenerUp = cVector3f(0,1,0);

		if(mpSystem->set3DListenerAttributes(0, &Pos, &Vel, &Forward, &Up) != FMOD_OK)
			Error("  Unable to set FMOD listener attributes.");

		//Default volume:
		SetVolume(1.0f);
	}

	//-----------------------------------------------------------------------

	void cLowLevelSoundFmod::SetEnvVolume( float afEnvVolume )
	{

	}

	iSoundEnvironment* cLowLevelSoundFmod::LoadSoundEnvironment (const tString& asFilePath)
	{

	}
	void cLowLevelSoundFmod::SetSoundEnvironment ( iSoundEnvironment* apSoundEnv )
	{

	}
	void cLowLevelSoundFmod::FadeSoundEnvironment( iSoundEnvironment* apSourceSoundEnv, iSoundEnvironment* apDestSoundEnv, float afT )
	{

	}

	iSoundDeviceIdentifier* cLowLevelSoundFmod::GetCurrentSoundDevice()
	{
		if(mvSoundDevices.empty())
			return nullptr;
			
		return mvSoundDevices[0];
	}
}

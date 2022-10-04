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

#include "impl/FmodSoundChannel.h"
#include "impl/FmodSoundData.h"
#include "resources/SoundManager.h"

#include "math/Math.h"

namespace hpl {

	//////////////////////////////////////////////////////////////////////////
	// CONSTRUCTORS
	//////////////////////////////////////////////////////////////////////////

	//-----------------------------------------------------------------------
	
	cFmodSoundChannel::cFmodSoundChannel(iSoundData* apData, FMOD::Channel* alChannel,cSoundManager* apSoundManger)
	: iSoundChannel(apData, apSoundManger)
	{
		mlChannel = alChannel;
		mlChannel->getFrequency(&mlDefaultFreq);

		mfPosition.x = 0.0f;
		mfPosition.y = 0.0f;
		mfPosition.z = 0.0f;

		mfVelocity.x = 0.0f;
		mfVelocity.y = 0.0f;
		mfVelocity.z = 0.0f;

		mlChannel->set3DAttributes(&mfPosition, &mfVelocity);
		mlChannel->set3DMinMaxDistance(100000.0f, 200000.0f);
	}
	
	//-----------------------------------------------------------------------

	cFmodSoundChannel::~cFmodSoundChannel()
	{
		if(mlChannel != nullptr)
			mlChannel->stop();

		DestroyData();
	}

	//-----------------------------------------------------------------------

	//////////////////////////////////////////////////////////////////////////
	// PUBLIC METHODS
	//////////////////////////////////////////////////////////////////////////

	//-----------------------------------------------------------------------
	
	void cFmodSoundChannel::Play()
	{
		SetPaused(false);

		mbStopUsed = false;
	}
	
	//-----------------------------------------------------------------------
	
	void cFmodSoundChannel::Stop()
	{
		mlChannel->stop();
		mlChannel = nullptr;

		mbStopUsed = true;
	}
	
	//-----------------------------------------------------------------------
	
	void cFmodSoundChannel::SetPaused(bool abX)
	{
		mbPaused = abX;
		mlChannel->setPaused(abX);

		if(mbPaused)
		{
			//Log("Setting %s paused!\n",mpData->GetName().c_str());
		}
	}
	
	//-----------------------------------------------------------------------
	
	void cFmodSoundChannel::SetSpeed(float afSpeed)
	{
		mfSpeed = afSpeed;
		
		int lFreq = mlDefaultFreq * afSpeed;

		mlChannel->setFrequency(lFreq);
	}
	
	//-----------------------------------------------------------------------
	
	void cFmodSoundChannel::SetVolume(float afVolume)
	{
		mfVolume = afVolume;
		
		mlChannel->setVolume(afVolume);
	}
	
	//-----------------------------------------------------------------------
	
	void cFmodSoundChannel::SetLooping (bool abLoop)
	{
		mbLooping = abLoop;

		mlChannel->setMode(mbLooping ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF);
	}

	//-----------------------------------------------------------------------

	void cFmodSoundChannel::SetPan (float afPan)
	{
		//Log("Pan: %d\n", lPan);
		
		mlChannel->setPan(afPan);
	}
	
	//-----------------------------------------------------------------------

	void cFmodSoundChannel::Set3D(bool ab3D)
	{
		mb3D = ab3D;		
	}
	
	//-----------------------------------------------------------------------
	
	void cFmodSoundChannel::SetPositionIsRelative(bool abRelative)
	{
		mbPositionRelative = abRelative;
	}

	//-----------------------------------------------------------------------
	
	void cFmodSoundChannel::SetPosition(const cVector3f &avPos)
	{
		mvPosition = avPos;

		mfPosition.x = avPos.x;
		mfPosition.y = avPos.y;
		mfPosition.z = avPos.z;
		
		mlChannel->set3DAttributes(&mfPosition, &mfVelocity);
	}
	
	//-----------------------------------------------------------------------

	void cFmodSoundChannel::SetVelocity(const cVector3f &avVel)
	{
		mvVelocity = avVel;

		mfVelocity.x = avVel.x;
		mfVelocity.y = avVel.y;
		mfVelocity.z = avVel.z;
		
		mlChannel->set3DAttributes(&mfPosition, &mfVelocity);
	}
	
	//-----------------------------------------------------------------------
	
	void cFmodSoundChannel::SetMinDistance(float afMin)
	{
		mfMinDistance = afMin;

		//FSOUND_3D_SetMinMaxDistance(mlChannel, mfMinDistance, mfMaxDistance);
	}

	//-----------------------------------------------------------------------
	
	void cFmodSoundChannel::SetMaxDistance(float afMax)
	{
		mfMaxDistance = afMax;

		//FSOUND_3D_SetMinMaxDistance(mlChannel, mfMinDistance, mfMaxDistance);
	}
	//-----------------------------------------------------------------------
	
	bool cFmodSoundChannel::IsPlaying()
	{
		bool isPlaying = false;

		mlChannel->isPlaying(&isPlaying);

		return isPlaying;
	}
	//-----------------------------------------------------------------------
	
	void cFmodSoundChannel::SetPriority(int alX)
	{
		int lPrio = alX + mlPriorityModifier;
		if(lPrio>255)
			lPrio =255;
		
		mlChannel->setPriority(lPrio);
	}
	
	//-----------------------------------------------------------------------
	
	int cFmodSoundChannel::GetPriority()
	{
		int priority = 0;
		mlChannel->getPriority(&priority);
		
		return priority;
	}
	
	//-----------------------------------------------------------------------

}

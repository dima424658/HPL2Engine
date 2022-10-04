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

#include "impl/FmodSoundData.h"
#include "impl/FmodSoundChannel.h"

#include "system/LowLevelSystem.h"
#include "system/String.h"

namespace hpl {

	//////////////////////////////////////////////////////////////////////////
	// CONSTRUCTORS
	//////////////////////////////////////////////////////////////////////////

	//-----------------------------------------------------------------------
	
	cFmodSoundData::cFmodSoundData(FMOD::System* system, const tString& asName, bool abStream) : iSoundData(asName,_W(""),abStream)
	{
		gpSystem = system;
		mpSound = nullptr;
	}
	
	//-----------------------------------------------------------------------

	cFmodSoundData::~cFmodSoundData()
	{
		if(mpSound)
		{
			mpSound->release();
			mpSound = nullptr;
		}
	}
	
	//-----------------------------------------------------------------------

	//////////////////////////////////////////////////////////////////////////
	// PUBLIC METHODS
	//////////////////////////////////////////////////////////////////////////

	//-----------------------------------------------------------------------
	
	bool cFmodSoundData::CreateFromFile(const tWString &asFile)
	{
		SetFullPath(asFile);

		FMOD_RESULT result;

		if(mbStream)
		{

			if(mbLoopStream)
				result = gpSystem->createStream(cString::To8Char(asFile).c_str(), FMOD_LOOP_NORMAL | FMOD_2D, nullptr, &mpSound);
			else
				result = gpSystem->createStream(cString::To8Char(asFile).c_str(), FMOD_2D, nullptr, &mpSound);

			if(result != FMOD_OK){
				Error("Couldn't load sound stream '%s'\n", cString::To8Char(asFile).c_str());
				return false;
			}

		}
		else
		{
			result = gpSystem->createSound(cString::To8Char(asFile).c_str(), FMOD_3D, nullptr, &mpSound);

			if(result != FMOD_OK){
				Error("Couldn't load sound data '%s'\n", asFile.c_str());
				return false;
			}

			mpSound->set3DMinMaxDistance(4.0f, 10000.0f);
			mpSound->setMode(FMOD_LOOP_NORMAL);
		}
				
		return true;
	}
	
	//-----------------------------------------------------------------------

	iSoundChannel* cFmodSoundData::CreateChannel(int alPriority)
	{
		if(mpSound == nullptr)
			return nullptr;

		FMOD::Channel* fmodChannel = nullptr;
		iSoundChannel *pSoundChannel = NULL;
		
		if(gpSystem->playSound(mpSound, nullptr, true, &fmodChannel) != FMOD_OK)
			return nullptr;

		pSoundChannel = hplNew( cFmodSoundChannel,(this,fmodChannel, mpSoundManger));

		return pSoundChannel;
	}

	//-----------------------------------------------------------------------

}

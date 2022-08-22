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

#include "../common/StdAfx.h"
using namespace hpl;

#include "ModelEditor.h"

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

int hplMain(const tString& asCommandLine)
{
	//////////////////////////
	// Init BlackBox
	#ifdef WIN32
		HINSTANCE hBlackBoxLib = LoadLibrary( "BlackBox.dll" );
	#endif

	//cMemoryManager::SetLogCreation(true);

	cModelEditor* pEditor = hplNew(cModelEditor, ());

	cEngine* pEngine = pEditor->Init(NULL, false);
	
    pEngine->Run();	

	hplDelete(pEditor);

	DestroyHPLEngine(pEngine);

	cMemoryManager::LogResults();

	//////////////////////////
	// Exit BlackBox
	#ifdef WIN32
			if(hBlackBoxLib) FreeLibrary(hBlackBoxLib);
	#endif

	return 0;
}

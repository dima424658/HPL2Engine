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

#ifndef HPLEDITOR_EDITOR_WINDOW_ENTITY_EDIT_BOX_SUBMESH_H
#define HPLEDITOR_EDITOR_WINDOW_ENTITY_EDIT_BOX_SUBMESH_H

#include "../common/StdAfx.h"
using namespace hpl;

#include "EditorWindowEntityEditBox.h"
#include "EditorEditModeSelect.h"

//----------------------------------------------------------------------

class iEditorBase;
class iEntityWrapper;

class cEditorEditModeSelect;

class cEntityWrapperSubMesh;
class cEntityWrapperBody;

class cEntitySelectorBodyHighlighter;

//----------------------------------------------------------------------

class cEditorWindowEntityEditBoxSubMesh : public cEditorWindowEntityEditBox
{
public:
	cEditorWindowEntityEditBoxSubMesh(cEditorEditModeSelect* apEditMode,cEntityWrapperSubMesh* apObject);
	virtual ~cEditorWindowEntityEditBoxSubMesh();

	void Create();
protected:
	void OnUpdate(float afTimeStep);

	bool Button_OnPressed(iWidget* apWidget, const cGuiMessageData& aData);
	kGuiCallbackDeclarationEnd(Button_OnPressed);

	bool WindowSpecificInputCallback(iEditorInput* apInput);

	//////////////////////////////////
	// Data
	cWidgetTab* mpTabGeneral;
	cWidgetTab* mpTabSubMesh;

	cEditorInputFile* mpInpMaterial;

	cEntityWrapperSubMesh* mpEntity;
};

//----------------------------------------------------------------------


#endif // HPLEDITOR_EDITOR_WINDOW_ENTITY_EDIT_BOX_SUBMESH_H

/*=========================================================================

  Name:        vtkWin32OpenGLMultiChannelRenderWindow.cxx

  Author:      David Borland, The Renaissance Computing Institute (RENCI)

  Copyright:   The Renaissance Computing Institute (RENCI)

  License:     Licensed under the RENCI Open Source Software License v. 1.0.
               
               See included License.txt or 
               http://www.renci.org/resources/open-source-software-license
               for details.

=========================================================================*/

#include "vtkWin32OpenGLMultiChannelRenderWindow.h"

#include "vtkCamera.h"
#include "vtkMultiChannelRenderWindowHelper.h"
#include "vtkObjectFactory.h"
#include "vtkRendererCollection.h"

vtkCxxRevisionMacro(vtkWin32OpenGLMultiChannelRenderWindow, "$Revision: 1.0 $");
vtkStandardNewMacro(vtkWin32OpenGLMultiChannelRenderWindow);

vtkCxxSetObjectMacro(vtkWin32OpenGLMultiChannelRenderWindow, Helper, vtkMultiChannelRenderWindowHelper);

//----------------------------------------------------------------------------
vtkWin32OpenGLMultiChannelRenderWindow::vtkWin32OpenGLMultiChannelRenderWindow() 
{
  this->Helper = NULL;
}

//----------------------------------------------------------------------------
vtkWin32OpenGLMultiChannelRenderWindow::~vtkWin32OpenGLMultiChannelRenderWindow() 
{
  if (this->Helper)
    {
    this->Helper->UnRegister(this);
    }
}

//----------------------------------------------------------------------------
void vtkWin32OpenGLMultiChannelRenderWindow::DoStereoRender()
{
  if (this->Helper->GetChannels()->GetNumberOfItems() == 0) 
    {
    // Default rendering
    vtkWin32OpenGLRenderWindow::DoStereoRender();
    }
  else 
    {
    this->Start();
    this->Helper->Render(this->Renderers);
    }
}

//----------------------------------------------------------------------------
void vtkWin32OpenGLMultiChannelRenderWindow::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);

  if (this->Helper)
    {
    os << indent << "Helper:\n"; 
    this->Helper->PrintSelf(os,indent.GetNextIndent());
    } 
}
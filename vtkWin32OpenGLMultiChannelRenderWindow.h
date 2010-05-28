/*=========================================================================

  Name:        vtkWin32OpenGLMultiChannelRenderWindow.h

  Author:      David Borland, The Renaissance Computing Institute (RENCI)

  Copyright:   The Renaissance Computing Institute (RENCI)

  License:     Licensed under the RENCI Open Source Software License v. 1.0.
               
               See included License.txt or 
               http://www.renci.org/resources/open-source-software-license
               for details.

=========================================================================*/
// .NAME vtkWin32OpenGLMultiChannelRenderWindow
// .SECTION Description
// vtkWin32OpenGLMultiChannelRenderWindow is a subclass of 
// vtkWin32OpenGLRenderWindow that adds multi-channel rendering
// support via a vtkMultiChannelRenderWindowHelper

// .SECTION see also
// vtkMultiChannelRenderWindowManager vtkMultiChannelRenderWindowHelper

#ifndef __vtkWin32OpenGLMultiChannelRenderWindow_h
#define __vtkWin32OpenGLMultiChannelRenderWindow_h

#include "vtkMultiChannelConfigure.h"

#include "vtkWin32OpenGLRenderWindow.h"

class vtkMultiChannelRenderWindowHelper;

class VTK_MULTICHANNEL_EXPORT vtkWin32OpenGLMultiChannelRenderWindow : public vtkWin32OpenGLRenderWindow
{
public:
  static vtkWin32OpenGLMultiChannelRenderWindow *New();
  vtkTypeRevisionMacro(vtkWin32OpenGLMultiChannelRenderWindow,vtkWin32OpenGLRenderWindow);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Holds channel information and sets up rendering for each channel
  void SetHelper(vtkMultiChannelRenderWindowHelper*);

protected:
  vtkWin32OpenGLMultiChannelRenderWindow();
  ~vtkWin32OpenGLMultiChannelRenderWindow();

  vtkMultiChannelRenderWindowHelper* Helper;

  // Description:
  // Override the default behavior for multi-channel rendering
  void DoStereoRender();

private:
  vtkWin32OpenGLMultiChannelRenderWindow(const vtkWin32OpenGLMultiChannelRenderWindow&);  // Not implemented.
  void operator=(const vtkWin32OpenGLMultiChannelRenderWindow&);  // Not implemented.
};

#endif
/*=========================================================================

 Name:        vtkMultiChannelRenderWindowManager.h

  Author:      David Borland, The Renaissance Computing Institute (RENCI)

  Copyright:   The Renaissance Computing Institute (RENCI)

  License:     Licensed under the RENCI Open Source Software License v. 1.0.
               
               See included License.txt or 
               http://www.renci.org/resources/open-source-software-license
               for details.

=========================================================================*/
// .NAME vtkMultiChannelRenderWindowManager
// .SECTION Description
// vtkMultiChannelRenderWindowManager is a class for adding multi-channel
// rendering support to a single vtkRenderWindow, specifically intended
// for immersive environments in which multiple views are tied to the same 
// camera.  Only one window/renderer/interactor/camera need be used, which 
// makes dealing with interaction much simpler.  The user specifies an 
// arbitrary number of views, and vtkMultiChannelRenderWindowManager 
// returns a platform-specific subclass of vtkRenderWindow that can 
// perform multi-channel rendering.  vtkMultiChannelRenderWindowManager 
// also returns a vtkRenderer with a camera suitable for multi-channel
// rendering.  The vtkRenderWindows and vtkRenderers returned should be 
// deleted  by the user.  An example of its use is to render a left eye 
// and right eye view of the scene in two channels next to each other, 
// which is a standard configuration for passive stereo and head-mounted 
// display devices.  Similarly, multiple views of a scene may be necessary 
// to render to immersive environments such as domes.

// .SECTION see also
// vtkRenderWindow vtkMultiChannelRenderWindowHelper vtkRenderWindowChannel
// vtkOpenGLMultiChannelCamera

#ifndef __vtkMultiChannelRenderWindowManager_h
#define __vtkMultiChannelRenderWindowManager_h

#include "vtkMultiChannelConfigure.h"

#include "vtkObject.h"

class vtkMultiChannelRenderWindowHelper;
class vtkRenderWindow;
class vtkRenderWindowChannel;
class vtkRenderer;

class VTK_MULTICHANNEL_EXPORT vtkMultiChannelRenderWindowManager : public vtkObject
{
public:
  static vtkMultiChannelRenderWindowManager *New();
  vtkTypeRevisionMacro(vtkMultiChannelRenderWindowManager,vtkObject);
  void PrintSelf(ostream&, vtkIndent);

  // Description:
  // Add a channel for rendering.
  void AddChannel(vtkRenderWindowChannel*);
  void ClearChannels();

  // Description:
  // Returns a platform-specific subclass of vtkRenderWindow that can
  // perform multi-channel rendering.
  vtkRenderWindow* GetRenderWindow();

  // Description:
  // Returns a vtkRenderer suitable for multi-channel rendering.
  // This creates a standard vtkRenderer, but replaces the vtkCamera
  // with one that supports an aspect ratio different from that 
  // of the viewport.
  vtkRenderer* GetRenderer();

protected:
  vtkMultiChannelRenderWindowManager();
  ~vtkMultiChannelRenderWindowManager();

  vtkMultiChannelRenderWindowHelper *Helper;

  bool NeedsStereo;

private:
  vtkMultiChannelRenderWindowManager(const vtkMultiChannelRenderWindowManager&);  // Not implemented.
  void operator=(const vtkMultiChannelRenderWindowManager&);  // Not implemented.
};

#endif
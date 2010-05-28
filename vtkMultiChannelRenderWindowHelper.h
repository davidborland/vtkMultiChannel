/*=========================================================================

  Name:        vtkMultiChannelRenderWindowHelper.h

  Author:      David Borland, The Renaissance Computing Institute (RENCI)

  Copyright:   The Renaissance Computing Institute (RENCI)

  License:     Licensed under the RENCI Open Source Software License v. 1.0.
               
               See included License.txt or 
               http://www.renci.org/resources/open-source-software-license
               for details.

=========================================================================*/
// .NAME vtkMultiChannelRenderWindowHelper
// .SECTION Description
// vtkMultiChannelRenderWindowHelper is a class for adding  multi-channel
// rendering support to a single vtkRenderWindow, specifically intended
// for immersive environments in which multiple views are tied to the same 
// camera.  Only one window/renderer/interactor/camera need be used, which 
// makes dealing with interaction much simpler.  
// vtkMultiChannelRender uses a collection of vtkRenderWindowChannels
// to render the scene to each of the channels

// .SECTION see also
// vtkRenderWindow vtkMultiChannelRenderWindowManger 
// vtkRenderWindowChannel

#ifndef __vtkMultiChannelRenderWindowHelper_h
#define __vtkMultiChannelRenderWindowHelper_h

#include "vtkMultiChannelConfigure.h"

#include "vtkObject.h"

class vtkCollection;
class vtkRendererCollection;
class vtkRenderWindowChannel;

class VTK_MULTICHANNEL_EXPORT vtkMultiChannelRenderWindowHelper : public vtkObject 
{
public:  
  static vtkMultiChannelRenderWindowHelper *New();
  vtkTypeRevisionMacro(vtkMultiChannelRenderWindowHelper,vtkObject);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Return the collection of channels
  vtkCollection *GetChannels();

  // Description:
  // Perform multi-channel rendering
  void Render(vtkRendererCollection*);

protected:
  vtkMultiChannelRenderWindowHelper();
  ~vtkMultiChannelRenderWindowHelper();

  vtkCollection* Channels;

private:    
  vtkMultiChannelRenderWindowHelper(const vtkMultiChannelRenderWindowHelper&);  // Not implemented.
  void operator=(const vtkMultiChannelRenderWindowHelper&);  // Not implemented.
};

#endif
/*=========================================================================

  Name:        vtkOpenGLMultiChannelCamera.h

  Author:      David Borland, The Renaissance Computing Institute (RENCI)

  Copyright:   The Renaissance Computing Institute (RENCI)

  License:     Licensed under the RENCI Open Source Software License v. 1.0.
               
               See included License.txt or 
               http://www.renci.org/resources/open-source-software-license
               for details.

=========================================================================*/
// .NAME vtkOpenGLMultiChannelCamera
// .SECTION Description
// vtkOpenGLMultiChannelCamera adds support for an aspect ratio that
// does not match the aspect ratio of the renderer being used.

// .SECTION see also
// vtkRenderWindowChannel vtkMultiChannelRenderWindowManger 
// vtkMultiChannelRenderWindowHelper

#ifndef __vtkOpenGLMultiChannelCamera_h
#define __vtkOpenGLMultiChannelCamera_h

#include "vtkMultiChannelConfigure.h"

#include "vtkOpenGLCamera.h"

class VTK_MULTICHANNEL_EXPORT vtkOpenGLMultiChannelCamera : public vtkOpenGLCamera
{
public:
  static vtkOpenGLMultiChannelCamera *New();
  vtkTypeRevisionMacro(vtkOpenGLMultiChannelCamera,vtkOpenGLCamera);
  virtual void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Set the aspect ratio to use when overriding the default 
  // aspect ratio
  vtkGetMacro(AspectRatio,double);
  vtkSetMacro(AspectRatio,double);

  // Description:  
  vtkGetMacro(UseAspectRatio,int);
  vtkSetMacro(UseAspectRatio,int);
  vtkBooleanMacro(UseAspectRatio,int);

  // Description:
  // Renders with the supplied aspect ratio if requested
  void Render(vtkRenderer*);

protected:
  vtkOpenGLMultiChannelCamera();
  ~vtkOpenGLMultiChannelCamera();

  double AspectRatio;
  int UseAspectRatio;

private:
  vtkOpenGLMultiChannelCamera(const vtkOpenGLMultiChannelCamera&);  // Not implemented.
  void operator=(const vtkOpenGLMultiChannelCamera&);  // Not implemented.
};

#endif
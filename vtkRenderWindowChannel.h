/*=========================================================================

  Name:        vtkRenderWindowChannel.h

  Author:      David Borland, The Renaissance Computing Institute (RENCI)

  Copyright:   The Renaissance Computing Institute (RENCI)

  License:     Licensed under the RENCI Open Source Software License v. 1.0.
               
               See included License.txt or 
               http://www.renci.org/resources/open-source-software-license
               for details.

=========================================================================*/
// .NAME vtkRenderWindowChannel
// .SECTION Description
// vtkRenderWindowChannel defines a single channel for performing
// multi-channel rendering using vtkMultiChannelRenderWindowManager.

// .SECTION see also
// vtkMultiChannelRenderWindowManager vtkMultiChannelRenderWindowHelper
// vtkOpenGLMultiChannelCamera

#ifndef __vtkRenderWindowChannel_h
#define __vtkRenderWindowChannel_h

#include "vtkMultiChannelConfigure.h"

#include "vtkObject.h"

class vtkDoubleArray;
class vtkIntArray;
class vtkRenderer;

// Stereo types
#define VTK_MULTICHANNEL_STEREO_NONE    0
#define VTK_MULTICHANNEL_STEREO_LEFT    1
#define VTK_MULTICHANNEL_STEREO_RIGHT   2

// Rotation types
#define VTK_MULTICHANNEL_YAW                    0
#define VTK_MULTICHANNEL_PITCH                  1
#define VTK_MULTICHANNEL_ROLL                   2
#define VTK_MULTICHANNEL_ORTHOGONALIZE_VIEW_UP  3

class VTK_MULTICHANNEL_EXPORT vtkRenderWindowChannel : public vtkObject 
{
public:
  static vtkRenderWindowChannel *New();
  vtkTypeRevisionMacro(vtkRenderWindowChannel,vtkObject);
  void PrintSelf(ostream&, vtkIndent);
  
  // Description:
  // Specify the viewport for the Viewport to draw in the rendering window. 
  // Coordinates are expressed as (xmin,ymin,xmax,ymax), where each
  // coordinate is 0 <= coordinate <= 1.0.
  vtkSetVector4Macro(Viewport,double); 

  // Description:
  // Set the stereo type for this channel
  void SetStereoTypeToNone();
  void SetStereoTypeToLeft();
  void SetStereoTypeToRight();
  vtkGetMacro(StereoType,int);

  // Description:
  // Describe a series of rotations for this channel.
  // Rotations will be applied in the order given.
  void Yaw(double);
  void Pitch(double);
  void Roll(double);
  void OrthogonalizeViewUp();

  // Description:
  // Set the vertical field of view for this channel
  void SetViewAngle(double);

  // Description:
  // Set the aspect ratio for this channel
  void SetAspectRatio(double);

  // Description:
  // Render this channel using the given renderer
  void Render(vtkRenderer*);

protected:
  vtkRenderWindowChannel();
  ~vtkRenderWindowChannel();

  double Viewport[4];

  int StereoType;

  vtkDoubleArray* Rotations;
  vtkIntArray* RotationTypes;

  double ViewAngle;
  bool UseViewAngle;

  double AspectRatio;
  bool UseAspectRatio;

  // Description:
  // For use in PrintSelf()
  const char *GetStereoTypeAsString();

private:
  vtkRenderWindowChannel(const vtkRenderWindowChannel&);  // Not implemented.
  void operator=(const vtkRenderWindowChannel&);  // Not implemented.
};

#endif
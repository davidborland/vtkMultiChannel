/*=========================================================================

  Name:        vtkRenderWindowChannel.h

  Author:      David Borland, The Renaissance Computing Institute (RENCI)

  Copyright:   The Renaissance Computing Institute (RENCI)

  License:     Licensed under the RENCI Open Source Software License v. 1.0.
               
               See included License.txt or 
               http://www.renci.org/resources/open-source-software-license
               for details.

=========================================================================*/

#include "vtkRenderWindowChannel.h"

#include "vtkDoubleArray.h"
#include "vtkIntArray.h"
#include "vtkOpenGLMultiChannelCamera.h"
#include "vtkMatrix4x4.h"
#include "vtkObjectFactory.h"
#include "vtkRenderer.h"

vtkCxxRevisionMacro(vtkRenderWindowChannel, "$Revision: 1.0 $");
vtkStandardNewMacro(vtkRenderWindowChannel);

//----------------------------------------------------------------------------
vtkRenderWindowChannel::vtkRenderWindowChannel() 
{
  this->Viewport[0] = 0;
  this->Viewport[1] = 0;
  this->Viewport[2] = 1;
  this->Viewport[3] = 1;

  this->StereoType = VTK_MULTICHANNEL_STEREO_NONE;

  this->Rotations = vtkDoubleArray::New();
  this->RotationTypes = vtkIntArray::New();

  this->ViewAngle = 45;
  this->UseViewAngle = false;

  this->AspectRatio = 1;
  this->UseAspectRatio = false;
}

//----------------------------------------------------------------------------
vtkRenderWindowChannel::~vtkRenderWindowChannel() 
{
  this->Rotations->Delete();
  this->RotationTypes->Delete();
}

//----------------------------------------------------------------------------
void vtkRenderWindowChannel::SetStereoTypeToNone() 
{
  this->StereoType = VTK_MULTICHANNEL_STEREO_NONE;
}

//----------------------------------------------------------------------------
void vtkRenderWindowChannel::SetStereoTypeToLeft() 
{
  this->StereoType = VTK_MULTICHANNEL_STEREO_LEFT;
}

//----------------------------------------------------------------------------
void vtkRenderWindowChannel::SetStereoTypeToRight() 
{
  this->StereoType = VTK_MULTICHANNEL_STEREO_RIGHT;
}

//----------------------------------------------------------------------------
void vtkRenderWindowChannel::Yaw(double angle)
{
  this->Rotations->InsertNextValue(angle);
  this->RotationTypes->InsertNextValue(VTK_MULTICHANNEL_YAW);
}

//----------------------------------------------------------------------------
void vtkRenderWindowChannel::Pitch(double angle)
{
  this->Rotations->InsertNextValue(angle);
  this->RotationTypes->InsertNextValue(VTK_MULTICHANNEL_PITCH);
}

//----------------------------------------------------------------------------
void vtkRenderWindowChannel::Roll(double angle)
{
  this->Rotations->InsertNextValue(angle);
  this->RotationTypes->InsertNextValue(VTK_MULTICHANNEL_ROLL);
}

//----------------------------------------------------------------------------
void vtkRenderWindowChannel::OrthogonalizeViewUp()
{
  this->Rotations->InsertNextValue(0);
  this->RotationTypes->InsertNextValue(VTK_MULTICHANNEL_ORTHOGONALIZE_VIEW_UP);
}

//----------------------------------------------------------------------------
void vtkRenderWindowChannel::SetViewAngle(double fov)
{
  this->ViewAngle = fov;
  this->UseViewAngle = true;
}

//----------------------------------------------------------------------------
void vtkRenderWindowChannel::SetAspectRatio(double ratio)
{
  this->AspectRatio = ratio;
  this->UseAspectRatio = true;
}

//----------------------------------------------------------------------------
void vtkRenderWindowChannel::Render(vtkRenderer* renderer)
{
  double x = this->Viewport[0];
  double y = this->Viewport[1];
  double w = this->Viewport[2] - this->Viewport[0];
  double h = this->Viewport[3] - this->Viewport[1];

#if defined(VTK_USE_MANGLED_MESA)
  vtkErrorMacro(<< "Multi-channel not implemented for this rendering library yet.");
  return;
#else
  vtkOpenGLMultiChannelCamera *camera = vtkOpenGLMultiChannelCamera::SafeDownCast(renderer->GetActiveCamera());
#endif

  // Set up stereo
  if (this->StereoType == VTK_MULTICHANNEL_STEREO_RIGHT)
    {
    camera->SetLeftEye(0);
    }
  else 
    {
    camera->SetLeftEye(1);
    }

  // Set the viewport
  double vp[4];
  renderer->GetViewport(vp);
  renderer->SetViewport(vp[0] * w + x, vp[1] * h + y, vp[2] * w + x, vp[3] * h + y);

  // Save the current camera settings
  double focalPoint[3];
  camera->GetFocalPoint(focalPoint);

  double viewUp[3];
  camera->GetViewUp(viewUp);

  // Rotate
  for (int i = 0; i < this->Rotations->GetNumberOfTuples(); i++)
    {
    double angle = this->Rotations->GetValue(i);
    int type = this->RotationTypes->GetValue(i);

    switch (type)
      {
      case VTK_MULTICHANNEL_YAW:
        camera->Yaw(angle);
        break;
      case VTK_MULTICHANNEL_PITCH:
        camera->Pitch(angle);
        break;
      case VTK_MULTICHANNEL_ROLL:
        camera->Roll(angle);
        break;
      case VTK_MULTICHANNEL_ORTHOGONALIZE_VIEW_UP:
        camera->OrthogonalizeViewUp();
        break;
      }
    }

  // Field of view
  int useHorizontalViewAngle = camera->GetUseHorizontalViewAngle();
  double viewAngle = camera->GetViewAngle();
  if (this->UseViewAngle)
    {
    camera->UseHorizontalViewAngleOff();
    camera->SetViewAngle(this->ViewAngle);
    }

  // Aspect ratio
  if (this->UseAspectRatio)
    {
    camera->UseAspectRatioOn();
    camera->SetAspectRatio(this->AspectRatio);
    }

  // Render
  renderer->ResetCameraClippingRange();
  renderer->Render();

  // Restore settings
  renderer->SetViewport(vp);

  camera->SetFocalPoint(focalPoint);
  camera->SetViewUp(viewUp);

  camera->SetUseHorizontalViewAngle(useHorizontalViewAngle);
  camera->SetViewAngle(viewAngle);

  renderer->ResetCameraClippingRange();
}

//----------------------------------------------------------------------------
const char *vtkRenderWindowChannel::GetStereoTypeAsString()
{
  switch (this->StereoType)
    {
    case VTK_MULTICHANNEL_STEREO_NONE:
      return "None";
    case VTK_MULTICHANNEL_STEREO_LEFT:
      return "Left";
    case VTK_MULTICHANNEL_STEREO_RIGHT:
      return "Right";
    default:
      return "";
    }
}

//----------------------------------------------------------------------------
void vtkRenderWindowChannel::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);

  os << indent << "Viewport: (" << this->Viewport[0] << ", " 
                                << this->Viewport[1] << ", " 
                                << this->Viewport[2] << ", " 
                                << this->Viewport[3] << ")\n";

  os << indent << "Stereo Type: " << this->GetStereoTypeAsString() << "\n";

  os << indent << "Rotations:\n"; 
  this->Rotations->PrintSelf(os, indent.GetNextIndent());

  os << indent << "Rotation Types:\n";
  this->RotationTypes->PrintSelf(os, indent.GetNextIndent());

  os << indent << "View Angle: " << this->ViewAngle << "\n";
  os << indent << "Use View Angle: " << this->UseViewAngle << "\n";
}
/*=========================================================================

  Name:        vtkOpenGLMultiChannelCamera.cxx

  Author:      David Borland, The Renaissance Computing Institute (RENCI)

  Copyright:   The Renaissance Computing Institute (RENCI)

  License:     Licensed under the RENCI Open Source Software License v. 1.0.
               
               See included License.txt or 
               http://www.renci.org/resources/open-source-software-license
               for details.

=========================================================================*/

#include "vtkOpenGLMultiChannelCamera.h"

#include "vtkgluPickMatrix.h"
#include "vtkMatrix4x4.h"
#include "vtkObjectFactory.h"
#include "vtkOpenGLRenderer.h"
#include "vtkOpenGLRenderWindow.h"
#include "vtkOpenGL.h"

#ifndef VTK_IMPLEMENT_MESA_CXX
vtkCxxRevisionMacro(vtkOpenGLMultiChannelCamera, "$Revision: 1.0 $");
vtkStandardNewMacro(vtkOpenGLMultiChannelCamera);
#endif

//----------------------------------------------------------------------------
vtkOpenGLMultiChannelCamera::vtkOpenGLMultiChannelCamera()
{
  this->AspectRatio = 1;
  this->UseAspectRatio = 0;
}

//----------------------------------------------------------------------------
vtkOpenGLMultiChannelCamera::~vtkOpenGLMultiChannelCamera()
{
}

//----------------------------------------------------------------------------
void vtkOpenGLMultiChannelCamera::Render(vtkRenderer *ren)
{
  if (!this->UseAspectRatio)
    {
    // Render as usual
    vtkOpenGLCamera::Render(ren);
    return;
    }

  // Render with the camera's aspect ratio, 
  // borrowing heavily from vtkOpenGLCamera::Render()
  int lowerLeft[2];
  int usize, vsize;
  vtkMatrix4x4 *matrix = vtkMatrix4x4::New();

  vtkOpenGLRenderWindow *win = vtkOpenGLRenderWindow::SafeDownCast(ren->GetRenderWindow());
  
  // Find out if we should stereo render
  this->Stereo = (ren->GetRenderWindow())->GetStereoRender();
  ren->GetTiledSizeAndOrigin(&usize,&vsize,lowerLeft,lowerLeft+1);
  
  // If were on a stereo renderer draw to special parts of screen
  if (this->Stereo)
    {
    switch ((ren->GetRenderWindow())->GetStereoType())
      {
      case VTK_STEREO_CRYSTAL_EYES:
        if (this->LeftEye)
          {
          if(ren->GetRenderWindow()->GetDoubleBuffer())
            {
            glDrawBuffer(static_cast<GLenum>(win->GetBackLeftBuffer()));
            glReadBuffer(static_cast<GLenum>(win->GetBackLeftBuffer()));
            }
          else
            {
            glDrawBuffer(static_cast<GLenum>(win->GetFrontLeftBuffer()));
            glReadBuffer(static_cast<GLenum>(win->GetFrontLeftBuffer()));
            }
          }
        else
          {
           if(ren->GetRenderWindow()->GetDoubleBuffer())
            {
            glDrawBuffer(static_cast<GLenum>(win->GetBackRightBuffer()));
            glReadBuffer(static_cast<GLenum>(win->GetBackRightBuffer()));
            }
          else
            {
            glDrawBuffer(static_cast<GLenum>(win->GetFrontRightBuffer()));
            glReadBuffer(static_cast<GLenum>(win->GetFrontRightBuffer()));
            }
          }
        break;
      case VTK_STEREO_LEFT:
        this->LeftEye = 1;
        break;
      case VTK_STEREO_RIGHT:
        this->LeftEye = 0;
        break;
      default:
        break;
      }
    }
  else
    {
    if (ren->GetRenderWindow()->GetDoubleBuffer())
      {
      glDrawBuffer(static_cast<GLenum>(win->GetBackBuffer()));
      
      // Reading back buffer means back left. see OpenGL spec.
      // because one can write to two buffers at a time but can only read from
      // one buffer at a time.
      glReadBuffer(static_cast<GLenum>(win->GetBackBuffer()));
      }
    else
      {
      glDrawBuffer(static_cast<GLenum>(win->GetFrontBuffer()));
      
      // Reading front buffer means front left. see OpenGL spec.
      // because one can write to two buffers at a time but can only read from
      // one buffer at a time.
      glReadBuffer(static_cast<GLenum>(win->GetFrontBuffer()));
      }
    }
  
  glViewport(lowerLeft[0], lowerLeft[1], usize, vsize);
  glEnable(GL_SCISSOR_TEST);
  glScissor(lowerLeft[0], lowerLeft[1], usize, vsize);
  
  glMatrixMode(GL_PROJECTION);
  if(usize && vsize)
    {
    // Use the user-supplied aspect ratio
    matrix->DeepCopy(this->GetProjectionTransformMatrix(
                     this->AspectRatio, -1, 1));
    matrix->Transpose();
    }

  if(ren->GetIsPicking())
    {
    int size[2]; size[0] = usize; size[1] = vsize;
    glLoadIdentity();
    vtkgluPickMatrix(ren->GetPickX(), ren->GetPickY(), 
                     ren->GetPickWidth(), ren->GetPickHeight(),
                     lowerLeft, size);
    glMultMatrixd(matrix->Element[0]);
    }
  else
    {
    // Insert camera view transformation 
    glLoadMatrixd(matrix->Element[0]);
    }
  
  // Push the model view matrix onto the stack, make sure we 
  // adjust the mode first
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();

  matrix->DeepCopy(this->GetViewTransformMatrix());
  matrix->Transpose();
  
  // Insert camera view transformation 
  glMultMatrixd(matrix->Element[0]);

  if ((ren->GetRenderWindow())->GetErase() && ren->GetErase() 
      && !ren->GetIsPicking())
    {
    ren->Clear();
    }
  
  matrix->Delete();
}

//----------------------------------------------------------------------------
void vtkOpenGLMultiChannelCamera::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);

  os << indent << "Aspect Ratio: " << this->AspectRatio << "\n";
  os << indent << "Use Aspect Ratio: " << this->UseAspectRatio << "\n";
}
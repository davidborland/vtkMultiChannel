/*=========================================================================

  Name:        vtkMultiChannelTest.cpp

  Author:      David Borland, The Renaissance Computing Institute (RENCI)

  Copyright:   The Renaissance Computing Institute (RENCI)

  License:     Licensed under the RENCI Open Source Software License v. 1.0.
               
               See included RENCI_License.txt or 
               http://www.renci.org/resources/open-source-software-license
               for details.

  Description: Program for testing vtkMultiChannel library.

=========================================================================*/


#include <vtkActor.h>
#include <vtkConeSource.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenciRenderWindowManager.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>

// 0 for command-line, 1 for Dome, 2 for TeleImmersionHD, 3 for TeleImmersion4K, 4 for UncHmd
int mode = 1;


void main(int argc, char* argv[]) {
    // Normal geometry creation
    vtkConeSource* cone = vtkConeSource::New();
    
    vtkPolyDataMapper* mapper = vtkPolyDataMapper::New();
    mapper->SetInputConnection(cone->GetOutputPort());

    vtkActor* actor = vtkActor::New();
    actor->SetMapper(mapper);


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Begin vtkMultiChannel stuff


    vtkRenciRenderWindowManager* manager = vtkRenciRenderWindowManager::New();   

    vtkRenderer* renderer = manager->GetRenderer();
    renderer->AddViewProp(actor);

    vtkRenderWindow* window;

    if (mode == 0) {
        window = manager->GetRenciRenderWindow(argc, argv);
    }
    else if (mode == 1) {
        window = manager->GetDomeRenderWindow();
    }
    else if (mode == 2) {
        window = manager->GetTeleImmersionHDRenderWindow();
    }
    else if (mode == 3) {
        window = manager->GetTeleImmersion4KRenderWindow();
    }
    else if (mode == 4) {
        window = manager->GetUncHmdRenderWindow();
    }


// End vtkMultiChannel stuff
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


    window->AddRenderer(renderer);


    // Normal interactor creation
    vtkInteractorStyleTrackballCamera* interactorStyle = vtkInteractorStyleTrackballCamera::New();

    vtkRenderWindowInteractor* interactor = vtkRenderWindowInteractor::New();
    interactor->SetRenderWindow(window);
    interactor->SetInteractorStyle(interactorStyle);

    // Must call ResetCamera() to size correctly on initialization
    renderer->ResetCamera();


    // Initialize, clean up, and start interaction
    interactor->Initialize();

    cone->Delete();
    mapper->Delete();
    actor->Delete();
    renderer->Delete();
    manager->Delete();
    window->Delete();
    interactorStyle->Delete();

    interactor->Start();
}
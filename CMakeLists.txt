CMAKE_MINIMUM_REQUIRED( VERSION 2.6 )

PROJECT( vtkMultiChannel )

SET( LIBRARY_OUTPUT_PATH "${vtkMultiChannel_BINARY_DIR}/lib" )


#######################################
# Include VTK
#######################################

FIND_PACKAGE( VTK REQUIRED )
INCLUDE( ${VTK_USE_FILE} )  


#######################################
# Static or Shared Libraries
#######################################

# Shared libraries are necessary for wrapping Python
OPTION( BUILD_SHARED_LIBS
        "Build with shared libraries."
        ${VTK_BUILD_SHARED_LIBS} )
# Copy the CMake option to a setting with vtkMultiChannel_ prefix for use in
# our project.  This name is used in vtkmsConfigure.h.in.
SET( vtkMultiChannel_BUILD_SHARED_LIBS ${BUILD_SHARED_LIBS} )


#######################################
# Wrap Python
#######################################

IF( VTK_WRAP_PYTHON )

  OPTION( vtkMultiChannel_WRAP_PYTHON
          "Wrap classes in Python."
          ON )

  IF( vtkMultiChannel_WRAP_PYTHON )
    SET( VTK_WRAP_PYTHON3_INIT_DIR "${vtkMultiChannel_SOURCE_DIR}/Wrapping" )
    INCLUDE( ${VTK_CMAKE_DIR}/vtkWrapPython.cmake )
    IF( WIN32 )
      IF( NOT BUILD_SHARED_LIBS )
        MESSAGE( FATAL_ERROR "Python support requires BUILD_SHARED_LIBS to be ON." )
        SET( vtkMultiChannel_CAN_BUILD 0 )
      ENDIF( NOT BUILD_SHARED_LIBS )
    ENDIF( WIN32 )
  ENDIF( vtkMultiChannel_WRAP_PYTHON )

ELSE( VTK_WRAP_PYTHON )

  IF( vtkMultiChannel_WRAP_PYTHON )
    MESSAGE( "Warning. vtkMultiChannel_WRAP_PYTHON is ON but the VTK version you have "
             "chosen does not support Python (VTK_WRAP_PYTHON is OFF).  "
             "Please set vtkMultiChannel_WRAP_PYTHON to OFF." )
    SET( vtkMultiChannel_WRAP_PYTHON OFF )
  ENDIF( vtkMultiChannel_WRAP_PYTHON )

ENDIF( VTK_WRAP_PYTHON )


#######################################
# Configure file
#######################################

CONFIGURE_FILE( ${vtkMultiChannel_SOURCE_DIR}/vtkMultiChannelConfigure.h.in
                ${vtkMultiChannel_BINARY_DIR}/vtkMultiChannelConfigure.h )
                
INCLUDE_DIRECTORIES( ${vtkMultiChannel_BINARY_DIR} )


#######################################
# Include vtkMultiChannel code
#######################################

INCLUDE_DIRECTORIES( ${vtkMultiChannel_SOURCE_DIR} )

SET( SRC vtkMultiChannelRenderWindowManager.h vtkMultiChannelRenderWindowManager.cxx
         vtkMultiChannelRenderWindowHelper.h vtkMultiChannelRenderWindowHelper.cxx
         vtkOpenGLMultiChannelCamera.h vtkOpenGLMultiChannelCamera.cxx
         vtkRenciRenderWindowManager.h vtkRenciRenderWindowManager.cxx
         vtkRenderWindowChannel.h vtkRenderWindowChannel.cxx
         vtkWin32OpenGLMultiChannelRenderWindow.h vtkWin32OpenGLMultiChannelRenderWindow.cxx )
ADD_LIBRARY( vtkMultiChannel ${SRC} )


#######################################
# Create Python library
#######################################

IF( VTK_WRAP_PYTHON AND vtkMultiChannel_WRAP_PYTHON)
  VTK_WRAP_PYTHON3( vtkMultiChannelPython vtkMultiChannelPython_SRC "${SRC}")
  ADD_LIBRARY( vtkMultiChannelPythonD ${vtkMultiChannelPython_SRC} )
  ADD_LIBRARY( vtkMultiChannelPython MODULE vtkMultiChannelPythonInit.cxx )
  TARGET_LINK_LIBRARIES( vtkMultiChannelPythonD vtkMultiChannel vtkRenderingPythonD )
  TARGET_LINK_LIBRARIES( vtkMultiChannelPython vtkMultiChannelPythonD )
ENDIF( VTK_WRAP_PYTHON AND vtkMultiChannel_WRAP_PYTHON )


#######################################
# Include vtkMultiChannelTest
#######################################

ADD_SUBDIRECTORY( Test )
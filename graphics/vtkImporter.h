/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkImporter.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


Copyright (c) 1993-2000 Ken Martin, Will Schroeder, Bill Lorensen 
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

 * Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.

 * Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

 * Neither name of Ken Martin, Will Schroeder, or Bill Lorensen nor the names
   of any contributors may be used to endorse or promote products derived
   from this software without specific prior written permission.

 * Modified source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS IS''
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

=========================================================================*/
// .NAME vtkImporter - importer abstract class
// .SECTION Description
// vtkImporter is an abstract class that specifies the protocol for
// importing actors, cameras, lights and proeperties into a
// vtkRenderWindow. The following takes place:
// 1) Create a RenderWindow and Renderer if none is provided.
// 2) Open the import file
// 3) Import the Actors
// 4) Import the cameras
// 5) Import the lights
// 6) Import the Properties
// 7) Close the import file
//
// Subclasses optionally implement the ImportActors, ImportCameras,
// ImportLights and ImportProperties methods. An ImportBegin and
// ImportEnd can optionally be provided to perform Importer-specific
// initialization and termination.  The Read method initiates the import
// process. If a RenderWindow is provided, its Renderer will contained
// the imported objects. If the RenderWindow has no Renderer, one is
// created. If no RenderWindow is provided, both a RenderWindow and
// Renderer will be created. Both the RenderWindow and Renderer can be
// accessed using Get methods.

// .SECTION See Also
// vtk3DSImporter vtkExporter

#ifndef __vtkImporter_h
#define __vtkImporter_h

#include <stdio.h>
#include "vtkObject.h"
#include "vtkRenderWindow.h"
#include "vtkRenderer.h"
#include "vtkPolyData.h"
#include "vtkActorCollection.h"
#include "vtkLightCollection.h"

class VTK_EXPORT vtkImporter : public vtkObject
{
public:
  vtkTypeMacro(vtkImporter,vtkObject);
  void PrintSelf(vtkOstream& os, vtkIndent indent);

  // Description:
  // Specify the name of the file to read.
  vtkSetStringMacro(FileName);
  vtkGetStringMacro(FileName);

  // Description
  // Get the renderer that contains the imported actors, cameras and
  // lights.
  vtkGetObjectMacro(Renderer,vtkRenderer);

  // Description
  // Set the vtkRenderWindow to contain the imported actors, cameras and
  // lights, If no vtkRenderWindow is set, one will be created and can be
  // obtained with the GetRenderWindow method. If the vtkRenderWindow has been
  // specified, the first vtkRenderer it has will be used to import the
  // objects. If the vtkRenderWindow has no Renderer, one will be created and
  // can be accessed using GetRenderer.
  vtkSetObjectMacro(RenderWindow,vtkRenderWindow);
  vtkGetObjectMacro(RenderWindow,vtkRenderWindow);

  // Description:
  // Set/Get the computation of normals. If on, imported geometry will
  // be run through vtkPolyDataNormals.
  vtkSetMacro(ComputeNormals,int);
  vtkGetMacro(ComputeNormals,int);
  vtkBooleanMacro(ComputeNormals,int);

  // Description
  // Import the actors, cameras, lights and properties into a vtkRenderWindow.
  void Read();
  void Update() {this->Read();};
  
  // Description:
  // Return the file pointer to the open file.
  FILE *GetFileFD() {return this->FileFD;};

protected:
  vtkImporter();
  ~vtkImporter();
  vtkImporter(const vtkImporter&) {};
  void operator=(const vtkImporter&) {};

  int OpenImportFile();
  void CloseImportFile();
  virtual int ImportBegin () {return 1;};
  virtual void ImportActors (vtkRenderer *vtkNotUsed(renderer)) {};
  virtual void ImportCameras (vtkRenderer *vtkNotUsed(renderer)) {};
  virtual void ImportLights (vtkRenderer *vtkNotUsed(renderer)) {};
  virtual void ImportProperties (vtkRenderer *vtkNotUsed(renderer)) {};
  virtual void ImportEnd () {};

  char *FileName;
  FILE *FileFD;
  vtkRenderer *Renderer;
  vtkRenderWindow *RenderWindow;
  int ComputeNormals;

};

#endif





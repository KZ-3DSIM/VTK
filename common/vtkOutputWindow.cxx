/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkOutputWindow.cxx
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
#include "vtkOutputWindow.h"
#ifdef _WIN32
#include "vtkWin32OutputWindow.h"
#endif
#include "vtkObjectFactory.h"

vtkOutputWindow* vtkOutputWindow::Instance = 0;
vtkOutputWindowSmartPointer vtkOutputWindow::SmartPointer(NULL);


void vtkOutputWindowDisplayText(const char* message)
{
  vtkOutputWindow::GetInstance()->DisplayText(message);
}

vtkOutputWindowSmartPointer::~vtkOutputWindowSmartPointer()
{
  if (Pointer)
    {
    Pointer->Delete();
    }
}

vtkOutputWindow::vtkOutputWindow()
{
  this->PromptUser = 0;
}

vtkOutputWindow::~vtkOutputWindow()
{
}

void vtkOutputWindow::PrintSelf(vtkOstream& os, vtkIndent indent)
{
  this->vtkObject::PrintSelf(os, indent);

  os << indent << "vtkOutputWindow Single instance = "
     << (void*)vtkOutputWindow::Instance << vtkEndl;
  os << indent << "Prompt User: " 
     << (this->PromptUser ? "On\n" : "Off\n");
}


// default implementation outputs to cerr only
void vtkOutputWindow::DisplayText(const char* txt)
{
  vtkCerr << txt;
  if (this->PromptUser)
    {
    char c = 'n';
    vtkCerr << "\nDo you want to suppress any further messages (y,n)?." 
              << vtkEndl;
    vtkCin >> c;
    if (c == 'y')
      {
      vtkObject::GlobalWarningDisplayOff(); 
      }
    }
}

// Up the reference count so it behaves like New
vtkOutputWindow* vtkOutputWindow::New()
{
  vtkOutputWindow* ret = vtkOutputWindow::GetInstance();
  ret->Register(NULL);
  return ret;
}


// Return the single instance of the vtkOutputWindow
vtkOutputWindow* vtkOutputWindow::GetInstance()
{
  if(!vtkOutputWindow::Instance)
    {
    // Try the factory first
    vtkOutputWindow::Instance = (vtkOutputWindow*)
      vtkObjectFactory::CreateInstance("vtkOutputWindow");
    // if the factory did not provide one, then create it here
    if(!vtkOutputWindow::Instance)
      {
#ifdef _WIN32    
      vtkOutputWindow::Instance = vtkWin32OutputWindow::New();
#else
      vtkOutputWindow::Instance = new vtkOutputWindow;
#endif
      }
    // set the smart pointer to the instance, so
    // it will be UnRegister'ed at exit of the program
    vtkOutputWindow::SmartPointer.SetPointer(vtkOutputWindow::Instance );
    }
  // return the instance
  return vtkOutputWindow::Instance;
}

void vtkOutputWindow::SetInstance(vtkOutputWindow* instance)
{
  if (vtkOutputWindow::Instance==instance)
    {
    return;
    }
  vtkOutputWindow::SmartPointer.SetPointer( instance );
  // preferably this will be NULL
  if (vtkOutputWindow::Instance)
    {
    vtkOutputWindow::Instance->Delete();;
    }
  vtkOutputWindow::Instance = instance;
  // Should be safe to send a message now as instance is set
  if (instance->GetReferenceCount()!=1)
    {
    vtkGenericWarningMacro(<<"OutputWindow should have reference count = 1");
    }
  // user will call ->Delete() after setting instance
  instance->Register(NULL);
}



/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkGraphItem.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

// .NAME vtkGraphItem - a vtkContextItem that draws a block (optional label).
//
// .SECTION Description
// This is a vtkContextItem that can be placed into a vtkContextScene. It draws
// a block of the given dimensions, and reacts to mouse events.

#ifndef vtkGraphItem_h
#define vtkGraphItem_h

#include "vtkContextItem.h"

class vtkContext2D;
class vtkGraph;

class vtkGraphItem : public vtkContextItem
{
public:
  vtkTypeMacro(vtkGraphItem, vtkContextItem);
  virtual void PrintSelf(ostream &os, vtkIndent indent);

  static vtkGraphItem *New();

  vtkGetObjectMacro(Graph, vtkGraph);
  virtual void SetGraph(vtkGraph* g);

  // Description:
  // Paint event for the item.
  virtual bool Paint(vtkContext2D *painter);

  // Description:
  // Returns true if the supplied x, y coordinate is inside the item.
  virtual bool Hit(const vtkContextMouseEvent &mouse);

  // Description:
  // Mouse enter event.
  virtual bool MouseEnterEvent(const vtkContextMouseEvent &mouse);

  // Description:
  // Mouse move event.
  virtual bool MouseMoveEvent(const vtkContextMouseEvent &mouse);

  // Description:
  // Mouse leave event.
  virtual bool MouseLeaveEvent(const vtkContextMouseEvent &mouse);

  // Description:
  // Mouse button down event.
  virtual bool MouseButtonPressEvent(const vtkContextMouseEvent &mouse);

  // Description:
  // Mouse button release event.
  virtual bool MouseButtonReleaseEvent(const vtkContextMouseEvent &mouse);

  void UpdatePositions();

protected:
  vtkGraphItem();
  ~vtkGraphItem();

  float LastPosition[2];

  bool MouseOver;
  int MouseButtonPressed;

  vtkGraph* Graph;
  vtkIdType HitVertex;

  class Implementation;
  Implementation* Impl;

private:
  vtkGraphItem(const vtkGraphItem &); // Not implemented.
  void operator=(const vtkGraphItem &);   // Not implemented.

};

#endif //vtkGraphItem_h

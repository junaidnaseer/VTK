/*=========================================================================

  Copyright (c) Kitware Inc.
  All rights reserved.

=========================================================================*/
// .SECTION Thanks
// This test was written by Philippe Pebay and Charles Law, Kitware 2012
// This work was supported in part by Commissariat a l'Energie Atomique (CEA/DIF)

#include "vtkHyperTreeGrid.h"
#include "vtkHyperTreeGridToUnstructuredGrid.h"
#include "vtkHyperTreeGridSource.h"

#include "vtkCamera.h"
#include "vtkCellData.h"
#include "vtkDataSetMapper.h"
#include "vtkNew.h"
#include "vtkOutlineFilter.h"
#include "vtkProperty.h"
#include "vtkRegressionTestImage.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkShrinkFilter.h"
#include "vtkUnstructuredGrid.h"

int TestHyperTreeGridTernary3DMaterialUnstructured( int argc, char* argv[] )
{
  // Hyper tree grid
  vtkNew<vtkHyperTreeGridSource> htGrid;
  int maxLevel = 5;
  htGrid->SetMaximumLevel( maxLevel );
  htGrid->SetGridSize( 3, 3, 2 );
  htGrid->SetGridScale( 1.5, 1., .7 );
  htGrid->SetDimension( 3 );
  htGrid->SetBranchFactor( 3 );
  htGrid->DualOn();
  htGrid->UseMaterialMaskOn();
  htGrid->SetDescriptor( "RRR .R. .RR ..R ..R .R.|R.......................... ........................... ........................... .............R............. ....RR.RR........R......... .....RRRR.....R.RR......... ........................... ........................... ...........................|........................... ........................... ........................... ...RR.RR.......RR.......... ........................... RR......................... ........................... ........................... ........................... ........................... ........................... ........................... ........................... ............RRR............|........................... ........................... .......RR.................. ........................... ........................... ........................... ........................... ........................... ........................... ........................... ...........................|........................... ..........................." );
  htGrid->SetMaterialMask( "111 011 011 111 011 110|111111111111111111111111111 111111111111111111111111111 000000000100110111111111111 111111111111111111111111111 111111111111111111111111111 111111111111111111111111111 111111111111111111111111111 111111111111111111111111111 000110011100000100100010100|111111111111111111111111111 111111111111111111111111111 111111111111111111111111111 111111111111111111111111111 111111111111111111111111111 111111111111111111111111111 111111111111111111111111111 111111111111111111111111111 111111111111111111111111111 111111111111111111111111111 111111111111111111111111111 111111111111111111111111111 111111111111111111111111111 111111111111111111111111111|111111111111111111111111111 111111111111111111111111111 111111111111111111111111111 111111111111111111111111111 111111111111111111111111111 111111111111111111111111111 111111111111111111111111111 111111111111111111111111111 111111111111111111111111111 111111111111111111111111111 110110110100111110111000000|111111111111111111111111111  11111111111111111111111111" );

  // Outline
  vtkNew<vtkOutlineFilter> outline;
  outline->SetInputConnection( htGrid->GetOutputPort() );

  // Hyper tree grid to unstructured grid filter
  vtkNew<vtkHyperTreeGridToUnstructuredGrid> htg2ug;
  htg2ug->SetInputConnection( htGrid->GetOutputPort() );
  htg2ug->Update();
  vtkUnstructuredGrid* pd = htg2ug->GetOutput();

  // Shrink filter
  vtkNew<vtkShrinkFilter> shrink;
  shrink->SetInputConnection( htg2ug->GetOutputPort() );
  shrink->SetShrinkFactor( .8 );

  // Mappers
  vtkNew<vtkDataSetMapper> mapper1;
  mapper1->SetInputConnection( shrink->GetOutputPort() );
  mapper1->SetScalarRange( pd->GetCellData()->GetScalars()->GetRange() );
  mapper1->SetResolveCoincidentTopologyToPolygonOffset();
  mapper1->SetResolveCoincidentTopologyPolygonOffsetParameters( 0, 1 );
  vtkNew<vtkDataSetMapper> mapper2;
  mapper2->SetInputConnection( htg2ug->GetOutputPort() );
  mapper2->ScalarVisibilityOff();
  mapper2->SetResolveCoincidentTopologyToPolygonOffset();
  mapper2->SetResolveCoincidentTopologyPolygonOffsetParameters( 1, 1 );

  // Actors
  vtkNew<vtkActor> actor1;
  actor1->SetMapper( mapper1.GetPointer() );
  vtkNew<vtkActor> actor2;
  actor2->SetMapper( mapper2.GetPointer() );
  actor2->GetProperty()->SetRepresentationToWireframe();
  actor2->GetProperty()->SetColor( .7, .7, .7 );

  // Camera
  vtkHyperTreeGrid* ht = htGrid->GetOutput();
  double bd[6];
  ht->GetBounds( bd );
  vtkNew<vtkCamera> camera;
  camera->SetClippingRange( 1., 100. );
  camera->SetFocalPoint( ht->GetCenter() );
  camera->SetPosition( -.8 * bd[1], 2.1 * bd[3], -4.8 * bd[5] );

  // Renderer
  vtkNew<vtkRenderer> renderer;
  renderer->SetActiveCamera( camera.GetPointer() );
  renderer->SetBackground( 1., 1., 1. );
  renderer->AddActor( actor1.GetPointer() );
  renderer->AddActor( actor2.GetPointer() );

  // Render window
  vtkNew<vtkRenderWindow> renWin;
  renWin->AddRenderer( renderer.GetPointer() );
  renWin->SetSize( 300, 300 );
  renWin->SetMultiSamples( 0 );

  // Interactor
  vtkNew<vtkRenderWindowInteractor> iren;
  iren->SetRenderWindow( renWin.GetPointer() );

  // Render and test
  renWin->Render();

  int retVal = vtkRegressionTestImage( renWin.GetPointer() );
  if ( retVal == vtkRegressionTester::DO_INTERACTOR )
    {
    iren->Start();
    }

  return !retVal;
}

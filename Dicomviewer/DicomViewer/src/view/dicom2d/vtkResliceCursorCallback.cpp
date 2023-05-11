#include "vtkResliceCursorCallback.h"

vtkResliceCursorCallback::vtkResliceCursorCallback()
{

}

vtkResliceCursorCallback::~vtkResliceCursorCallback()
{

}

void vtkResliceCursorCallback::Execute(vtkObject* caller, unsigned long, void* callData)
{
    vtkImagePlaneWidget* ipw = dynamic_cast<vtkImagePlaneWidget*>(caller);
    if (ipw)
    {
        double* wl = static_cast<double*>(callData);

        if (ipw == this->IPW[0])
        {
            this->IPW[1]->SetWindowLevel(wl[0], wl[1], 1);
            this->IPW[2]->SetWindowLevel(wl[0], wl[1], 1);
        }
        else if (ipw == this->IPW[1])
        {
            this->IPW[0]->SetWindowLevel(wl[0], wl[1], 1);
            this->IPW[2]->SetWindowLevel(wl[0], wl[1], 1);
        }
        else if (ipw == this->IPW[2])
        {
            this->IPW[0]->SetWindowLevel(wl[0], wl[1], 1);
            this->IPW[1]->SetWindowLevel(wl[0], wl[1], 1);
        }
    }
    vtkResliceCursorWidget* rcw = dynamic_cast<vtkResliceCursorWidget*>(caller);
    if (rcw)
    {
        vtkResliceCursorLineRepresentation* rep = dynamic_cast<vtkResliceCursorLineRepresentation*>(rcw->GetRepresentation());
        vtkResliceCursor* rc = rep->GetResliceCursorActor()->GetCursorAlgorithm()->GetResliceCursor();
        for (int i = 0; i < 3; i++)
        {
            vtkPlaneSource* ps = static_cast<vtkPlaneSource*>(this->IPW[i]->GetPolyDataAlgorithm());
            ps->SetNormal(rc->GetPlane(i)->GetNormal());
            ps->SetCenter(rc->GetPlane(i)->GetOrigin());
            this->IPW[i]->UpdatePlacement();
        }
    }
    this->RCW[0]->Render();
}
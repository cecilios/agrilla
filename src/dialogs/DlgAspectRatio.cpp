#include "DlgAspectRatio.h"
#include <wx/valnum.h>
#include <wx/msgdlg.h>
#include <sstream>

namespace agrilla
{


//---------------------------------------------------------------------------------------
// DlgAspectRatio class constructor.
DlgAspectRatio::DlgAspectRatio(wxWindow* parent, double currentAspectRatio)
    : wxDialog(parent, wxID_ANY, "Set Aspect Ratio", wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
    , m_initialAspectRatio(currentAspectRatio)
{
    // Store the initial aspect ratio as the default value
    m_aspectRatio = m_initialAspectRatio;
    wxLogMessage("[DlgAspectRatio::DlgAspectRatio] aspect ratio: initial=%.4f, current=%.4f", m_initialAspectRatio, currentAspectRatio);

    // Main sizer for the dialog
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    // Sizer for Aspect Ratio option
    wxBoxSizer* aspectRatioSizer = new wxBoxSizer(wxHORIZONTAL);
    m_radioAspectRatio = new wxRadioButton(this, wxID_ANY, "Aspect ratio");
    m_textCtrlAspectRatio = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_RIGHT);
    m_staticTextAspectRatioError = new wxStaticText(this, wxID_ANY, "", wxDefaultPosition, wxSize(200, -1), wxALIGN_LEFT);
    m_staticTextAspectRatioError->SetForegroundColour(*wxRED);
    aspectRatioSizer->Add(m_radioAspectRatio, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 10);
    aspectRatioSizer->Add(m_textCtrlAspectRatio, 1, wxALIGN_CENTER_VERTICAL | wxRIGHT, 10);

    // Sizer for Paper Size option
    wxBoxSizer* paperSizeSizer = new wxBoxSizer(wxHORIZONTAL);
    m_radioPaperSize = new wxRadioButton(this, wxID_ANY, "Paper size (w x h)");
    m_textCtrlWidth = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_RIGHT);
    m_textCtrlHeight = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_RIGHT);
    m_staticTextWidthError = new wxStaticText(this, wxID_ANY, "", wxDefaultPosition, wxSize(200, -1), wxALIGN_LEFT);
    m_staticTextWidthError->SetForegroundColour(*wxRED);
    m_staticTextHeightError = new wxStaticText(this, wxID_ANY, "", wxDefaultPosition, wxSize(200, -1), wxALIGN_LEFT);
    m_staticTextHeightError->SetForegroundColour(*wxRED);

    wxBoxSizer* paperSizeTextCtrlSizer = new wxBoxSizer(wxHORIZONTAL);
    paperSizeTextCtrlSizer->Add(m_textCtrlWidth, 1, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);
    paperSizeTextCtrlSizer->Add(new wxStaticText(this, wxID_ANY, "x"), 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);
    paperSizeTextCtrlSizer->Add(m_textCtrlHeight, 1, wxALIGN_CENTER_VERTICAL | wxRIGHT, 10);

    paperSizeSizer->Add(m_radioPaperSize, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 10);
    paperSizeSizer->Add(paperSizeTextCtrlSizer, 1, wxALIGN_CENTER_VERTICAL);

    // Buttons sizer
    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    m_buttonAccept = new wxButton(this, wxID_OK, "Accept");
    wxButton* buttonCancel = new wxButton(this, wxID_CANCEL, "Cancel");
    buttonSizer->Add(m_buttonAccept, 0, wxRIGHT, 10);
    buttonSizer->Add(buttonCancel, 0);

    // Add sizers to main sizer
    mainSizer->Add(aspectRatioSizer, 0, wxEXPAND | wxALL, 10);
    mainSizer->Add(m_staticTextAspectRatioError, 0, wxEXPAND | wxLEFT, 130);
    mainSizer->Add(paperSizeSizer, 0, wxEXPAND | wxALL, 10);
    mainSizer->Add(m_staticTextWidthError, 0, wxEXPAND | wxLEFT, 130);
    mainSizer->Add(m_staticTextHeightError, 0, wxEXPAND | wxLEFT, 130);
    mainSizer->Add(buttonSizer, 0, wxALIGN_CENTER | wxTOP | wxBOTTOM, 10);

    // Initial setup
    m_radioAspectRatio->SetValue(true);
    m_textCtrlWidth->Disable();
    m_textCtrlHeight->Disable();
    m_buttonAccept->Disable();

    // Bind event handlers
    Bind(wxEVT_INIT_DIALOG, &DlgAspectRatio::on_init_dialog, this);
    Bind(wxEVT_RADIOBUTTON, &DlgAspectRatio::on_radio_aspect_ratio_selected, this, m_radioAspectRatio->GetId());
    Bind(wxEVT_RADIOBUTTON, &DlgAspectRatio::on_radio_paper_size_selected, this, m_radioPaperSize->GetId());
    Bind(wxEVT_TEXT, &DlgAspectRatio::on_text_ctrl_updated, this, m_textCtrlAspectRatio->GetId());
    Bind(wxEVT_TEXT, &DlgAspectRatio::on_text_ctrl_updated, this, m_textCtrlWidth->GetId());
    Bind(wxEVT_TEXT, &DlgAspectRatio::on_text_ctrl_updated, this, m_textCtrlHeight->GetId());
    Bind(wxEVT_BUTTON, &DlgAspectRatio::on_accept_button, this, m_buttonAccept->GetId());
    Bind(wxEVT_BUTTON, &DlgAspectRatio::on_cancel_button, this, buttonCancel->GetId());

    SetSizerAndFit(mainSizer);
    Centre();
}

//---------------------------------------------------------------------------------------
// Returns the final aspect ratio selected by the user.
double DlgAspectRatio::get_aspect_ratio() const
{
    return m_aspectRatio;
}

//---------------------------------------------------------------------------------------
// Called when the dialog is initialized, sets the initial value.
void DlgAspectRatio::on_init_dialog(wxInitDialogEvent& WXUNUSED(event))
{
    m_textCtrlAspectRatio->SetValue(wxString::Format("%.4f", m_initialAspectRatio));
    wxLogMessage("[DlgAspectRatio::on_init_dialog] Current aspect ratio: %.4f", m_initialAspectRatio);
    validate_input();
}

//---------------------------------------------------------------------------------------
// Handles event when the "Aspect ratio" radio button is selected.
void DlgAspectRatio::on_radio_aspect_ratio_selected(wxCommandEvent& WXUNUSED(event))
{
    m_radioAspectRatio->SetValue(true);
    m_radioPaperSize->SetValue(false);
    m_textCtrlAspectRatio->Enable();
    m_textCtrlWidth->Disable();
    m_textCtrlHeight->Disable();
    validate_input();
}

//---------------------------------------------------------------------------------------
// Handles event when the "Paper size" radio button is selected.
void DlgAspectRatio::on_radio_paper_size_selected(wxCommandEvent& WXUNUSED(event))
{
    m_radioAspectRatio->SetValue(false);
    m_radioPaperSize->SetValue(true);
    m_textCtrlAspectRatio->Disable();
    m_textCtrlWidth->Enable();
    m_textCtrlHeight->Enable();
    validate_input();
}

//---------------------------------------------------------------------------------------
// Handles text changes in any of the text controls.
void DlgAspectRatio::on_text_ctrl_updated(wxCommandEvent& event)
{
    // If text is entered in width or height, auto-select the paper size radio.
    if ((event.GetEventObject() == m_textCtrlWidth || event.GetEventObject() == m_textCtrlHeight) && !m_radioPaperSize->GetValue())
    {
        m_radioPaperSize->SetValue(true);
        m_radioAspectRatio->SetValue(false);
        m_textCtrlAspectRatio->Disable();
        m_textCtrlWidth->Enable();
        m_textCtrlHeight->Enable();
    }

    validate_input();
}

//---------------------------------------------------------------------------------------
// Handles the "Accept" button click.
void DlgAspectRatio::on_accept_button(wxCommandEvent& WXUNUSED(event))
{
    validate_input();
    if (m_buttonAccept->IsEnabled())
    {
        // Compute the final aspect ratio and store it
        if (m_radioAspectRatio->GetValue())
        {
            double parsedValue;
            if (parse_double(m_textCtrlAspectRatio->GetValue(), parsedValue))
            {
                m_aspectRatio = parsedValue;
            }
        }
        else // Paper size selected
        {
            wxLogMessage("[DlgAspectRatio::on_accept_button] Paper size selected");
            double width, height;
            if (parse_double(m_textCtrlWidth->GetValue(), width) && parse_double(m_textCtrlHeight->GetValue(), height))
            {
                if (height != 0.0)
                {
                    m_aspectRatio = width / height;
                }
            }
            bool fWidthOk = parse_double(m_textCtrlWidth->GetValue(), width);
            bool fHeightOk = parse_double(m_textCtrlHeight->GetValue(), height);
            wxLogMessage("[DlgAspectRatio::on_accept_button] Paper size selected. W: %.2f (%s), H: %.2f (%s)",
                         width, (fWidthOk ? "true" : "false"), height, (fHeightOk ? "true" : "false"));
        }
        wxLogMessage("[DlgAspectRatio::on_accept_button] New aspect ratio: %.4f", m_aspectRatio);
        EndModal(wxID_OK);
    }
}

//---------------------------------------------------------------------------------------
// Handles the "Cancel" button click.
void DlgAspectRatio::on_cancel_button(wxCommandEvent& WXUNUSED(event))
{
    EndModal(wxID_CANCEL);
}

//---------------------------------------------------------------------------------------
// Validates the current input in the fields and updates the UI accordingly.
void DlgAspectRatio::validate_input()
{
    bool fValidInput = false;

    set_error_message(m_staticTextAspectRatioError, "");
    set_error_message(m_staticTextWidthError, "");
    set_error_message(m_staticTextHeightError, "");

    if (m_radioAspectRatio->GetValue())
    {
        double value;
        if (!m_textCtrlAspectRatio->GetValue().IsEmpty())
        {
            if (parse_double(m_textCtrlAspectRatio->GetValue(), value) && value > 0.0)
            {
                fValidInput = true;
            }
            else
            {
                set_error_message(m_staticTextAspectRatioError, "Invalid number");
            }
        }
        else
        {
            set_error_message(m_staticTextAspectRatioError, "Field cannot be empty");
        }
    }
    else // Paper size option
    {
        double width, height;
        bool fWidthValid = parse_double(m_textCtrlWidth->GetValue(), width) && width > 0.0;
        bool fHeightValid = parse_double(m_textCtrlHeight->GetValue(), height) && height > 0.0;

        if (m_textCtrlWidth->GetValue().IsEmpty() || m_textCtrlHeight->GetValue().IsEmpty())
        {
            if (m_textCtrlWidth->GetValue().IsEmpty())
            {
                set_error_message(m_staticTextWidthError, "Field cannot be empty");
            }
            if (m_textCtrlHeight->GetValue().IsEmpty())
            {
                set_error_message(m_staticTextHeightError, "Field cannot be empty");
            }
        }
        else
        {
            if (!fWidthValid)
            {
                set_error_message(m_staticTextWidthError, "Invalid number");
            }
            if (!fHeightValid)
            {
                set_error_message(m_staticTextHeightError, "Invalid number");
            }

            fValidInput = fWidthValid && fHeightValid;
        }
    }

    if (fValidInput)
    {
        m_buttonAccept->Enable();
    }
    else
    {
        m_buttonAccept->Disable();
    }
}

//---------------------------------------------------------------------------------------
// Parses a string to a double, handling both '.' and ',' as decimal points.
bool DlgAspectRatio::parse_double(const wxString& text, double& value)
{
    wxString tempText = text;
    tempText.Replace(",", ".");

    try
    {
        std::istringstream iss(tempText.ToStdString());
        iss >> value;
        return !iss.fail() && iss.eof();
    }
    catch (...)
    {
        return false;
    }
}

//---------------------------------------------------------------------------------------
// Sets the error message on a static text control, and shows/hides it.
void DlgAspectRatio::set_error_message(wxStaticText* staticText, const wxString& message)
{
    staticText->SetLabel(message);
    staticText->Show(!message.IsEmpty());
}

} // namespace agrilla


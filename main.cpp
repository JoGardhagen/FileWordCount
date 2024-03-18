#include <wx/wxprec.h>// strunta i att detta visar rött understrykning....
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <fstream>
#include <sstream>

class MyFrame : public wxFrame {
public:
    MyFrame(const wxString& title) : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(400, 200)) {
        wxPanel *panel = new wxPanel(this, wxID_ANY);

        // Skapa ett textfält för att visa den valda filen
        m_textSelectedFile = new wxTextCtrl(panel, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, wxTE_READONLY);

        // Skapa en knapp för att öppna filväljaren
        m_buttonBrowse = new wxButton(panel, wxID_ANY, wxT("Bläddra..."));

        // Skapa ett textfält för sökord
        m_textSearch = new wxTextCtrl(panel, wxID_ANY);

        // Skapa en knapp för att utföra sökningen
        m_buttonSearch = new wxButton(panel, wxID_ANY, wxT("Sök"));

        // Skapa en statisk text för att visa resultatet av sökningen
        m_staticResult = new wxStaticText(panel, wxID_ANY, wxT(""));

        // Skapa en sizer för att placera komponenterna
        wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
        sizer->Add(new wxStaticText(panel, wxID_ANY, wxT("Fil:")), 0, wxALL, 5);
        sizer->Add(m_textSelectedFile, 0, wxALL | wxEXPAND, 5);
        sizer->Add(m_buttonBrowse, 0, wxALL | wxALIGN_RIGHT, 5);
        sizer->Add(new wxStaticText(panel, wxID_ANY, wxT("Sökord:")), 0, wxALL, 5);
        sizer->Add(m_textSearch, 0, wxALL | wxEXPAND, 5);
        sizer->Add(m_buttonSearch, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);
        sizer->Add(m_staticResult, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);

        panel->SetSizerAndFit(sizer);

        // Lägg till händelsehanterare för knapparna
        m_buttonBrowse->Bind(wxEVT_BUTTON, &MyFrame::OnBrowse, this);
        m_buttonSearch->Bind(wxEVT_BUTTON, &MyFrame::OnSearch, this);
    }

    void OnBrowse(wxCommandEvent& event) {
        wxFileDialog openFileDialog(this, wxT("Välj fil"), "", "", wxT("Alla filer (*.*)|*.*"), wxFD_OPEN | wxFD_FILE_MUST_EXIST);

        if (openFileDialog.ShowModal() == wxID_CANCEL)
            return;     // Användaren avbröt filväljningen

        wxString filePath = openFileDialog.GetPath();
        m_textSelectedFile->SetValue(filePath);
    }

    void OnSearch(wxCommandEvent& event) {
        wxString filename = m_textSelectedFile->GetValue();
        wxString searchString = m_textSearch->GetValue();
        int result = CountString(filename.ToStdString(), searchString.ToStdString());
        m_staticResult->SetLabel(wxString::Format(wxT("Antal förekomster av \"%s\" i filen \"%s\": %d"), searchString, filename, result));
    }

private:
    wxTextCtrl *m_textSelectedFile;
    wxButton *m_buttonBrowse;
    wxTextCtrl *m_textSearch;
    wxButton *m_buttonSearch;
    wxStaticText *m_staticResult;

    int CountString(const std::string& filename, const std::string& searchString) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            wxLogError(wxT("Kunde inte öppna filen för läsning."));
            return -1;
        }

        int count = 0;
        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string word;
            while (iss >> word) {
                if (word == searchString) {
                    count++;
                }
            }
        }
        return count;
    }
};

class MyApp : public wxApp {
public:
    virtual bool OnInit() {
        MyFrame *frame = new MyFrame(wxT("Sökning av ord"));
        frame->Show(true);
        return true;
    }
};

wxIMPLEMENT_APP(MyApp);
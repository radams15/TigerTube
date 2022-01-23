//
// Created by rhys on 24/10/2020.
//

#ifndef WXWIDGETS_TABLE_H
#define WXWIDGETS_TABLE_H


#include <wx/grid.h>
#include <vector>

class Table : public wxGridTableBase{

private:
    std::vector< wxString > headers;
    std::vector< std::vector< wxString > > data;

    bool Exists(int row, int col);

public:
    int GetNumberRows();

    void Clear();

    int GetNumberCols();

    bool IsEmptyCell(int row, int col);

    wxString GetValue(int row, int col);

    wxString GetColLabelValue(int col);

    void SetValue(int row, int col, const wxString &value);

    void append(const std::vector<wxString>& to_add);

    void set_headers(std::vector<wxString> new_headers);
};


#endif //WXWIDGETS_TABLE_H

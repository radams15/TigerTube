//
// Created by rhys on 24/10/2020.
//

#include <iostream>
#include "Table.h"

using namespace std;

int Table::GetNumberRows() {
    return data.size();
}

int Table::GetNumberCols() {
    return headers.size();
}

bool Table::IsEmptyCell(int row, int col) {
    if(!Exists(row, col)){
        return true;
    }else{
        if(GetValue(row, col).empty()){
            return true;
        }
    }

    return false;
}

wxString Table::GetValue(int row, int col) {
    if(Exists(row, col)){
        return data.at(row).at(col);
    }

    return "NONE";
}

wxString Table::GetColLabelValue(int col) {
    return headers.at(col);
}

void Table::SetValue(int row, int col, const wxString &value) {
    if(Exists(row, col)){
        data.at(row).at(col) = value;
    }
}

bool Table::Exists(int row, int col) {
    if(row >= GetNumberRows() || col >= GetNumberCols()){
        return false;
    }
    return true;
}

void Table::append(const vector<wxString>& to_add) {
    if(to_add.size() == headers.size()){
        data.push_back(to_add);
    }else{
        std::cerr << "Invalid Append List Size!" << std::endl;
    }
}

void Table::set_headers(vector<wxString> new_headers) {
    headers = move(new_headers);
}

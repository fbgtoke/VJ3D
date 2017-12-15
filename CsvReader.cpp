#include "CsvReader.h"

CsvReader::CsvReader() {}

CsvReader::~CsvReader() {}

void CsvReader::loadFromFile(const std::string& filename, char separator) {
  std::ifstream stream(filename);
  if (!stream.is_open()) {
    std::cout << "Could not open CSV file " << filename << std::endl;
    return;
  }

  mValues.clear();

  std::string line;
  while (getline(stream,line)) {
    int values = std::count(line.begin(), line.end(), separator);
    std::istringstream sstream(line);

    int v;
    char c;
    std::vector<int> row(values + 1);
    for (int i = 0; i < values + 1; ++i) {
      sstream >> v;
      if (i < values) sstream >> c;

      row[i] = v;
    }
    mValues.push_back(row);
  }
  stream.close();
}

int CsvReader::getNumberOfRows() const {
  return mValues.size();
}

int CsvReader::getNumberOfColumns() const {
  if (mValues.size() == 0)
    return 0;
  return mValues[0].size();
}

int CsvReader::getCell(unsigned int row, unsigned int column) const {
  return mValues[row][column];
}

void CsvReader::getRow(unsigned int row, std::vector<int>& values) const {
  values = mValues[row];
}

void CsvReader::getValues(std::vector<std::vector<int>>& values) const {
  values = mValues;
}
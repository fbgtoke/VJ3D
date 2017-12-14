#ifndef _CSV_READER_INCLUDE
#define _CSV_READER_INCLUDE

#include "utils.h"

class CsvReader {
public:
  CsvReader();
  ~CsvReader();

  void loadFromFile(const std::string& filename, char separator = ',');

  int getNumberOfRows() const;
  int getNumberOfColumns() const;

  int getCell(unsigned int row, unsigned int column) const;
  void getRow(unsigned int row, std::vector<int>& values) const;
  void getValues(std::vector<std::vector<int>>& values) const;

private:
  std::vector<std::vector<int>> mValues;
};

#endif // _CSV_READER
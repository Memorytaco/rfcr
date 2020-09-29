#include "rfcdoc.h"

using std::string;

RFCDoc::RFCDoc(RFConfig& conf, int number)
{
  std::fstream file{
    conf.query<string>("repo") + "rfc" + std::to_string(number) + ".txt",
    std::ios_base::in
  };
  string line;
  while (!file.eof()) {
    getline(file, line);
    lines.push_back(line);
  }
  file.close();

  int count = 0;
  int maxcount = lines.size();
  Range page{1,1};
  while (count < maxcount) {
    int len = lines[count].length();
    if (len > 0 && lines[count][0] == 0x0c) {
      page.set_end(count);
      pages.push_back(page);
      page.set_begin(count+2);
    }
    count++;
  }
}

const string& RFCDoc::at(int i) const
{
  if (i<=0) throw std::out_of_range("invalid number below 1");
  if (i <= lines.size())
    return lines.at(i-1);
  else
    throw std::out_of_range("document access exceed");
}

#include "rfcdoc.h"

using std::string;
using std::endl;
using std::cout;

/* RFCIndex */
RFCIndex::RFCIndex(RFConfig& conf, std::string name)
{
  std::fstream file{conf.query<std::string>("repo") + name, std::ios_base::in};
  std::string tmp{};
  std::string entry{};
  std::vector<std::string> list{};
  while (true) {
    getline(file, tmp);
    if (tmp.substr(0, 4) == "0001") break;
  }
  entry.append(tmp);
  while (!file.eof()) {
    getline(file, tmp);
    if (tmp.length() == 0) {
      list.push_back(entry);
      entry.clear();
    } else {
      int start = tmp.find_first_not_of(' ');
      start = start <= 0? 0: start - 1;
      entry.append(tmp.substr(start));
    }
  }
  // number of entries
  file.close();

  // fill the entry to hashtable
  for (auto &i: list) {
    keys.push_back(stoi(i.substr(0,4)));
    db[i.substr(0, 4)] = add_entry(i);
  }
}

const std::map<string, string>& RFCIndex::query(int key) const
{
  string query = std::to_string(key);
  while (query.length() < 4) {
    query.insert(0, "0");
  }
  return db.at(query);
}


void RFCIndex::consoleprint(int num) const
{
  std::map<string, string> entry = query(num);
  if (entry.count("Title") == 0) {
    cout << "No Entry" << endl;
    return;
  }
  cout << "\033[38;5;136mTitle: \033[0m" << entry["Title"] << endl;
  cout << "\033[38;5;136mAuthors: \033[0m" << entry["Authors"] << endl;
  cout << "\033[38;5;136mDate: \033[0m" << entry["Date"] << endl;
  cout << "\033[38;5;136mAttributes: \033[0m" << entry["Attrs"] << endl;
}


std::map<string, string> RFCIndex::add_entry(string& entry)
{
  std::map<string, string> table;
  table["Content"] = entry;

  std::string lookup = entry.substr(5);
  int mark = lookup.find('.') + 1;
  table["Title"] = lookup.substr(0, mark);

  // this entry has only the title
  if (lookup.length() == mark + 1)
    return table;

  lookup = lookup.substr(mark+1);

  using std::smatch;
  smatch m;
  static std::regex pattern(".(January|February|March|April|May|June|July|August|September|October|November|December) [0-9]{4}..");
  regex_search(lookup, m, pattern);
  table["Authors"] = m.prefix();
  table["Date"] = m[0];
  table["Date"] = table["Date"].substr(1);
  table["Attrs"] = m.suffix();

  return table;
}

std::vector<int> RFCIndex::regexsearch(std::string regx)
{
  std::vector<int> list{};
  std::regex pattern(regx);
  std::smatch m;
  for (auto key : keys) {
    auto& val = query(key);
    string str = val.at("Title");
    regex_search(str, m, pattern);
    if (m.size() > 0) list.push_back(key);
  }
  return list;
}

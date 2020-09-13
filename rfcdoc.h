#ifndef RFCDOC
#define RFCDOC

#include "core.h"

class RFConfig
{
  public:
    std::string basedir;
    std::string fontdir;
    std::string shaderdir;

    RFConfig(std::string base, std::string font, std::string shader) :
      basedir{base}, fontdir{font}, shaderdir{shader}
    {}
    RFConfig(RFConfig& conf) {
      basedir =  conf.basedir;
      fontdir = conf.fontdir;
      shaderdir = conf.shaderdir;
    }
};

class RFCIndex
{
  public:
    RFCIndex(RFConfig& conf, std::string name);
    const std::map<std::string, std::string>& query(int key) const;
    int size() const { return db.size(); }
    void consoleprint(int num) const;
  private:
    std::map<std::string, std::map<std::string, std::string>> db;
    std::map<std::string, std::string> add_entry(std::string& entry);
};

class RFCDoc
{
  public:
    RFCDoc(RFConfig& conf, int number);
    ~RFCDoc() {}

    int line_num() const { return lines.size(); }
    int page_num() const { return pages.size(); }
    // 1 based index
    const std::string& at(int i) const;
    // 1 based page
    const Range& get_page(int i) const { return pages.at(i-1); }
  private:
    std::vector<std::string> lines;
    std::vector<Range> pages;
};

#endif

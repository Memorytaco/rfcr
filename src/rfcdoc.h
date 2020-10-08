#ifndef RFCDOC
#define RFCDOC

#include "core.h"

#include "yaml-cpp/yaml.h"
class RFConfig
{
  public:
    RFConfig(std::string path) : yamlconf{path} {
      conf = YAML::LoadFile(yamlconf);
    }
    RFConfig(RFConfig&& conf) : yamlconf{conf.yamlconf} {
      this->conf = std::move(conf.yamlconf);
    }

    template<typename T>
    T query(std::string key) {
      return conf[key].as<T>();
    }

  public:
    std::string yamlconf;
    YAML::Node conf;
};

class RFCIndex
{
  public:
    RFCIndex(RFConfig& conf, std::string name);
    const std::map<std::string, std::string>& query(int key) const;
    int size() const { return db.size(); }
    void consoleprint(int num) const;
    std::vector<int> regexsearch(std::string);
  private:
    std::map<std::string, std::map<std::string, std::string>> db;
    std::vector<int> keys{};  // with default empty keys
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

#ifndef RFCUTIL
#define RFCUTIL

class Range
{
  public:
    Range(int begin, int end) : begin(begin), end(end) {}
    /* Range(Range& r) { */
    /*   begin = r.begin; */
    /*   end = r.end; */
    /* } */
    Range(int beg) {
      end = begin = beg;
    }
    ~Range() {}
    int length() const {
      return end - begin + 1;
    }
    int set_begin(int i) {
      return begin = i;
    }
    int set_end(int i) {
      return end = i;
    }
  // private:
    int begin;
    int end;
};

#endif

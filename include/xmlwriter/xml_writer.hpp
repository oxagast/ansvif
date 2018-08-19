#ifndef XML_WRITER_HPP
# define XML_WRITER_HPP

# define HEADER "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
# define INDENT "  "
# define NEWLINE "\n"

# include <string.h>
#ifdef __linux
#include "include/stack.h"
#endif
#ifdef __ANDROID__
# include "../include/stack.h"
#endif
# include <iostream>

class Writer
{
public:

  Writer(std::ostream& os) : os(os), tag_open(false), new_line(true) {os << HEADER;}
  ~Writer() {}

  Writer& openElt(const char* tag) {
    this->closeTag();
    if (elt_stack.size() > 0)
      os << NEWLINE;
    this->indent();
    this->os << "<" << tag;
    elt_stack.push(tag);
    tag_open = true;
    new_line = false;
    return *this;
  }

  Writer& closeElt() {
    this->closeTag();
    std::string elt = elt_stack.top();
    this->elt_stack.pop();
    if (new_line)
      {
        os << NEWLINE;
        this->indent();
      }
    new_line = true;
    this->os << "</" << elt << ">";
    return *this;
  }

  Writer& closeAll() {
    while (elt_stack.size())
      this->closeElt();
    return *this;
}

  Writer& attr(const char* key, const char* val) {
    this->os << " " << key << "=\"";
    this->write_escape(val);
    this->os << "\"";
    return *this;
  }

  Writer& attr(const char* key, std::string val) {
    return attr(key, val.c_str());
  }

  Writer& content(const char* val) {
    this->closeTag();
    this->write_escape(val);
    return *this;
  }

private:
  std::ostream& os;
  bool tag_open;
  bool new_line;
  stack<std::string> elt_stack;

  inline void closeTag() {
    if (tag_open)
      {
        this->os << ">";
        tag_open = false;
      }
  }

  inline void indent() {
    int elt_stack_size = elt_stack.size();
    for (int i = 0; i < elt_stack_size; i++)
      os << (INDENT);
  }

  inline void write_escape(const char* str) {
    for (; *str; str++)
      switch (*str) {
      case '&': os << "&amp;"; break;
      case '<': os << "&lt;"; break;
      case '>': os << "&gt;"; break;
      case '\'': os << "&apos;"; break;
      case '"': os << "&quot;"; break;
      default: os.put(*str); break;
      }
  }
};

#endif /* !XML_WRITER_HPP */

// Predicate.h
#ifndef PREDICATE_H
#define PREDICATE_H

#include <string>

class Predicate {
  private:
    std::string name;

  public:
    Predicate(const std::string &n) : name(n) {}

    std::string getName() const { return name; }

    std::string toString() const { return name + "()"; }
};

#endif // PREDICATE_H

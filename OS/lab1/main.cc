#include <iostream>
#include "Shell.h"

int main() {
  std::string script_name = "script.txt";
  /*************************************************
  if you want run to run console,
  command Shell s(script_name);
  uncommand "Shell s;"
  *************************************************/
  Shell s(script_name);
 // Shell s;
  s.run();
  return 0;
}
#include"Filter.hpp"
int main(int argc,char**argv)
{
  Arko::Filter f(argc,argv);
  int status=f.run();
  return status;
}

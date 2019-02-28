#include "cnf.h"
#include <iostream>
using std::cin;
using std::cout;
using std::endl;

static uint32_t lenInt(int v);

//#define LINE_MAX_LEN 80

Cnf::Cnf()
{
  nVar = 0;
  nClause = 0;
  nCur = 0;
  clVec = nullptr;
  bmap = nullptr;
}

void Cnf::parseFile(const char *fpath)
{
  FILE *fp = fopen(fpath, "r");

  char buf[LINE_MAX_LEN];
  memset(buf, 0, LINE_MAX_LEN);

  // 跳过注释
  fgets(buf, LINE_MAX_LEN, fp);
  while ('c' == buf[0] || '\n' == buf[0]) {
    fgets(buf, LINE_MAX_LEN, fp);
  }
  // p
  sscanf(buf, "p cnf %d %d", &nVar, &nClause);

  cout << "nVar: " << nVar << endl;
  cout << "nClause: " << nClause << endl;

  clVec = new Clause*[nClause];
  bmap = new BitMap(nClause);

  for (nCur = 0; nCur < nClause; ++nCur) {
    fgets(buf, LINE_MAX_LEN, fp);
    clVec[nCur] = parseLine(buf);
  }
}

Clause *Cnf::parseLine(char buf[])
{
  char c;
  int var;
  int max_var;

//  uint32_t nVar = countNvar(buf);

  char* token = strtok(buf, " ");
  while ( token != NULL ) {
    token = strtok(NULL, " ");
    var = atoi(token);
  }
  return nullptr;
}

uint32_t Cnf::countNvar(char buf[])
{
  uint32_t nVar = 0;
  char pre = ' ';
  for (uint32_t i = 0; buf[i] != '\0'; ++i) {
    if (isdigit(buf[i]) && (' ' == pre || '-' == pre))
      ++nVar;
    pre = buf[i];
  }
  return nVar;
}

bool Cnf::existClause(unsigned int pos)
{
  return bmap->get(pos);
}

void Cnf::rmClause(unsigned int pos)
{
  bmap->set(pos, false);
}

void Cnf::restore(CnfEdit *edit)
{
//  BitMap *deletedClause = edit->deletedClause;
  bmap->OR(edit->deletedClause); // 还原子句的存在情况

  BitMap **varVec = edit->varVec;
  if (nullptr == varVec)
    return;
  for (uint32_t var = 0; var < nVar; ++var) {
    if (nullptr == varVec[var])
      continue;
    BitMap *posMap = varVec[var];
    for (uint32_t p = 0; p < posMap->len; ++p) {
      if (posMap->get(p)) {
        clVec[p]->addVar(var);
      }
    }
    posMap->reset();
  }
}

static uint32_t lenInt(int v)
{
  if (v < 0)
    v = -v;
  unsigned int len = 1;

  v /= 10;
  while (v != 0) {
    ++len;
    v /= 10;
  }
  return len;
}

// -*- C++ -*-
//
// icd.h
//

#ifndef ICD_H
#define ICD_H

#include <set>
#include <stack>
#include <string>
#include "bb.h"
#include "du_chain.h"

//
// class ICD indirect control dependence
//
class ICD{
  // define
  typedef std::set< int > SET;
  typedef SET::iterator SI;
  typedef std::stack< int > STACK;

  // current function number/bb size
  int func;
  int bb_size;
  std::string fname;

  // ���뼱�̻Ҥ���������ꤹ����ܥ֥�å��ν���
  SET **icd;

public:
  // const/destructor
  ICD(Program_Info &, const int &);
  ~ICD();

  // icd analysis
  void analysis(Program_Info &, DU_Chain &);

  // file out
  void print(std::ofstream &);

private:
  // �ؿ��λ�����������ޤǤ��̲᤹����ܥ֥�å��ν�������
  SET pass_bb(Program_Info &, SET &);

  // stack argolism subroutine
  void sub_insert(SET &, STACK &, const int &);

  // ���������Ȥ��
  SET defined_bb(Program_Info &, SET &);
};

#endif

// -*- C++ -*-
//
// loop.h
//
//  Time-stamp: <04/01/26 15:52:53 nakajima>
//

#ifndef LOOP_H
#define LOOP_H

#include <map>
#include <set>
#include "bb.h"
#include "trace.h"

//
// class Loop
//

enum Skip{
  NOT_FOUND,
  CONST_PC,
  INDUCT_PC,
  CONST_CAND,
  INDUCT_CAND,
  EXIT_BRANCH
};


class Loop{
  // define
  typedef std::set< int > SET;
  typedef SET::iterator SI;
  typedef std::map< int, int > MAP;
  typedef MAP::iterator MI;

  // function size
  int func_size;

  // loop head bb
  // a[func].{ set of header BBs }
  SET *header_bb;

  // a[func][pc].loop header bb
  // loop exit branch pc, loop header bb
  MAP *exit_pcs;
  // loop constant variable pc, loop header bb
  MAP *const_pcs;
  // loop induction variable pc, loop header bb
  MAP *induct_pcs;

  // counter
  int induct_c;// loop induction variable
  int const_c; // loop constant variable
  int unroll_c;// loop unroll branch

public:
  // const/destructor
  Loop();
  ~Loop();

  // file read
  void file_read();

  // ignore/skip pc check
  Skip flag_check(const Pipe_Inst &);

  const int ignore_inst();

  // loop header
  bool header(const Func_Bb& fbb){
    return( header_bb[fbb.func].count(fbb.bb) );
  }

  // counter
  void count_unroll();
  void count_loop_inst(const Skip &);

  // result
  void print_result();

private:
  // �롼�פ�ʿ�ѷ����֤����
  class Loop_Count{
    int bedge_c; // loop backedge count
    int header_c;// loop header count

  public:
    // constructor
    Loop_Count() { bedge_c = header_c = 0; }

    // counting
    void bedge() { bedge_c ++; }
    void header() { header_c ++; }

    // 0��0 �����å�
    bool check(){ return( header_c ); }
    // �ƥ롼�פ�ʿ�ѷ����֤����
    double calc_ave(){
      // �롼�ײ�� / �롼�פ����ä����
      return( (double)( bedge_c + header_c ) / (double)header_c );
    }
    void print(){
      std::cout << "bedge, header, ave: " << bedge_c << ", " << header_c
  	   << ", " << calc_ave() << std::endl;
    }
    int exec() { return( header_c ); }
    int loop() { return( bedge_c + header_c ); }
  };

  // �롼�׼¹Բ���ȡ�header�˸���ʬ���������
  Loop_Count **loop_c;

  void allocate_loop_c();
  void print_count_loop();

public:
  // �롼�פ�ʿ�ѷ����֤����
  void count_bedge(const Func_Bb &);
  void count_header(const Func_Bb &);

private:
  // check code
  void print();
};


//
// extern
//

extern Loop lp;

#endif

// -*- C++ -*-
//
// fork.h
//
//  Time-stamp: <04/02/04 16:29:01 nakajima>
//

#ifndef FORK_H
#define FORK_H

#include <map>
#include <stack>
#include "bb.h"
#include "re.h"
#include "trace.h"

// INT_MAX
const int null_data = 1 << 30;// 1,073,741,824

//
// class Func_Stack (function data stack)
class Func_Stack{
public:
  class Bb_Data{
  public:
    int exec_time;// ���Υ���åɼ¹Բ�ǽ����
    int order_tc;// bb����Ƭ̿���trace count
    int send_time;// send��ǽ�ʻ���
    int lp_exec_time;// loop unroll: loop headerŸ��������åɼ¹Բ�ǽ����
    bool lp_unroll;// loop �����ä���硢true

    // constructor
    Bb_Data(){
      order_tc = -1;
      exec_time = null_data;
      lp_exec_time = null_data;
      send_time = -1;
      lp_unroll = false;
    }
  };

  // define
  typedef std::stack< int > STACK;
  // require stack size
  static const int stack_size = 600;// ���ߥ�졼�����˽�ʬ�ʿ�

  // for setjmp/longjmp
  STACK fstack_setjmp;
  STACK fstack_setjmp_diff;

  // function stack (data and func num)
  int end;
  Bb_Data *fstack[stack_size];
  int fstack_func[stack_size];

  // function stack (func num) (for reexec model)
  int end_diff;
  Bb_Data *fstack_diff[stack_size];
  int fstack_func_diff[stack_size];

  // reexec state check
  bool re_state;

  // fork (loop unroll)
  bool exec_loop_fork;

public:
  // const/destructor
  Func_Stack();
  ~Func_Stack();

  // init trace skip
  void init_trace_skip();

  // cd analysis
  void analysis(const Pipe_Inst&);
  void analysis_no_CD(const Pipe_Inst&);

  // lp.flag_check()
  const bool not_zero(const Func_Bb&);

  // read Bb_Data
  Bb_Data & data_read(const Func_Bb&);

  // for reexec mode
  void re_start();
  void re_end();

private:
  // pop/push stack Func_Data
  void pop_stack();
  void push_stack();
  // write Bb_Data
  void data_write(const Func_Bb&, const Bb_Data &);

  // fork
  void fork_child_process(const int &);

  // cd_analysis sub
  void cd_ana_jr_return();
  void cd_ana_jall_call();
  void cd_ana_others();
};

//
// extern
//

extern Func_Stack func_data;

#endif

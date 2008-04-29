// -*- C++ -*-
//
//  re.h reexec mode
//
//  Time-stamp: <04/02/04 16:11:12 nakajima>
//

#ifndef RE_H
#define RE_H

using namespace std;

#include <list>
#include "trace.h"

//
// class Re_Exec (reexec mode, pipe instruction spool)
//

// �¹Ԥξ�������
enum Re_Mode{
  Normal, // �̾�¹�
  No,     // ���¹�: fork������ss�¹Ԥξ��֡����¹�̿���(����fork���ޤ�)�η׻�
  Change, // No -> Fork�˾������ܤ�������������¹�fork�����顢Fork������
  Fork,   // ���¹�: fork���ss�¹Ԥξ���
  Finish  // ���¹ԥ⡼�ɽ�λ��fork���뤷�ʤ�����ꤷ��Normal������
};

class Re_Exec{
  // define
  typedef list< Pipe_Inst > LIST;
  typedef LIST::iterator LI;

  // reexec mode instruction spool
  LIST spool;
  LI spool_i;

  // reexec mode
  Re_Mode re;// for reexec mode

  // check state
  bool re_state;// "Normal" or other mode
  bool ch_state;// "Change" or other mode

  // trace counter
  int trace_count;
  int re_trace_count;// for reexec mode
  int spool_size;// �ºݤ˲��¹Ԥ���̿��

  // reexec exec_time
  int no_max_exec;  // for reexec mode
  int fork_max_exec;// for reexec mode

  // for Ex_EQ
  int fork_exec_time;

  // counter
  int re_count;
  int gain_fork;
  int no_gain_no_fork;
  int no_gain_fork;
  int eq_no_fork;
  int eq_fork;

public:
  // const/destructor
  Re_Exec();
  ~Re_Exec();

  // mode check and fetch new instructon
  const bool check_mode(Pipe_Inst &);
  // change Normal -> No
  void change_no();
  // check fork_child_process
  const bool check_fork_process(const int&);

private:
  // change No -> Change
  void change();
  // change Change -> Fork
  void change_fork();
  // change Finish -> Normal
  void change_normal();

public:
  // spool instructons
  void init_inst_spool();
  // no reexec mode && lp mode trace skip
  void init_trace_skip();
  // delete current instruction, push next instructions
  void pop_and_push_inst();

public:
  // calc gain
  void gain(const int &);
  // current reexec state
  const Re_Mode mode() { return re; }
  // current trace count
  const int tc();

  // print result
  const int get_re_count() { return re_count; }
  void print_result();

private:
  // backup/restore data
  void backup_data();
  void restore_data();

private:
  // debug
  void state();
};


//
// extern
//

extern Re_Exec reexec;

#endif

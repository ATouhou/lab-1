// -*- C++ -*-
//
// sim.h
//
//  Time-stamp: <04/02/04 19:02:14 nakajima>
//

#ifndef SIM_H
#define SIM_H

#include <string>

//
// class Sim_Model simulation model
//

// simulation processor type
enum Sim_Type{
  BASE   = 0x00,// base
  SP     = 0x01,// sp (branch prediction)
  CD     = 0x02,// (control dependence)
  MF     = 0x04,// (multiple control-flow)
  ORACLE = 0x08,// oracle 

  SP_CD  = 0x03,// sp + cd
  CD_MF  = 0x06,// cd + mf (posdom)

  PD     = 0x07,// PD (posdom sp + cd + mf)
  LP     = 0x17,// LP (loop sp + cd + mf)
  FC     = 0x27,// FC (func sp + cd + mf)
  CE     = 0x47 // CE (ctrl-eq sp + cd + mf)
};

// data dependence model
enum Data_Model{
  // ʪ���쥸�����˴ؤ����ǥ�
  Reg_None,
  Reg_Finite,
  Reg_Perfect,
  // ����å�������¸�˴ؤ����ǥ�
  SP_Sequential,
  SP_Reorder,
  SP_Perfect,
  // ����åɴ֥����¸�˴ؤ����ǥ�
  MP_Blind,
  MP_Analysis,
  MP_Predict,
  MP_Perfect,
  MP_Profile,
  // ��ͽ¬
  VP_Nopred,
  VP_Send,  // �̿���ͽ¬
  VP_Result // �̿��ͤ�ɬ�פȤ���̿��η����ͽ¬
};

// �굡Ū�¹�
enum Sp_Exec{
  SP_None = 0x00,// ���굡
  SP_Fork = 0x01,// �굡Ū����å�����
  SP_Send = 0x02,// �굡Ū�̿�
  SP_Both = 0x03 // SP_Fork + SP_Send
};

// simulation model
class Sim_Model{
  std::string time_stamp;
  std::string fix_me;
  std::string argv0;
  std::string data_dir;

public:
  // filename
  std::string bb_info;
  std::string posdom_out;
  std::string ctrleq_out;
  std::string icd_info;
  std::string brn_pred;
  std::string loop_del_pc;
  std::string mp_analysis;
  std::string val_pred_send;
  std::string val_pred_result;
  std::string bb_statistic;

  // branch pred static
  bool static_brn_pred;

  // inline unrolling
  // stack���(add), call/return̿�����
  bool func_inline;
  // loop unrolling
  bool loop_unroll;
  // indirect control dependence
  bool icd;

  // sp,gp perfect disambiguate
  bool perf_disamb;

  // �굡Ūfork, �굡Ūsend
  Sp_Exec sp_exec;
  // send, fork latency
  int send_latency;
  int fork_latency;

  // data dependence model
  Data_Model reg;// ʪ���쥸�����˴ؤ����ǥ�
  Data_Model sp;// ����å�������¸�˴ؤ����ǥ�
  Data_Model mp;// ����åɴ֥����¸�˴ؤ����ǥ�
  Data_Model vp;// ��ͽ¬

  // ʪ���쥸������
  int reg_physical;
  // MP_Analysis, MP_Predict store num
  int mp_store_num;
  // MP_Predict warmup/no warmup
  bool mdpred_warmup;
  // MP_Predict ʣ������Ū�ʥ��ȥ������äƤ�����Ū��Ʊ����Ȥ뤳�Ȥ��Ǥ���
  bool mdpred_perf;
  // value predictor threshold
  int val_pred_th;

  // reexec mode
  bool reexec;
  // reexec mode spool size
  int spool_size;

  // ���ߥ�졼����󤹤�̿���
  int trace_limit;
  // trace skip
  int fastfwd_tc;

  // print
  int debuglevel;
  int print_freq;
  int print_freq_def;

  // ����
  bool statistic;

public:
  // constructor
  Sim_Model();

  // argment check
  void arg_check(const int &, char **);
  // print
  void print_argment();
  // debug flag
  const bool debug(const int &);
  void set_mp_profile();

private:
  const int check_atoi(const std::string);
  void usage();
};

//
// extern
//

extern Sim_Type sim_type;
extern Sim_Model model;

#endif

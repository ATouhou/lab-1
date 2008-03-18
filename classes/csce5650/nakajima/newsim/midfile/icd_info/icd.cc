//
// icd.cc create icd_info
//

#include <iostream>
#include <fstream>
#include "icd.h"

#include "main.h"

//
// class ICD (Indirect Control Dependence)
//

// constructor
ICD::ICD(Program_Info &program, const int &f){
  func = f;
  bb_size = program.size(func);
  fname = program.funcname(func);

  try{
    icd = new SET*[bb_size];
  }
  catch( std::bad_alloc ){
    error("ICD::ICD() bad_alloc");
  }

  for( int bb = 0; bb < bb_size; bb ++ ){
    try{
      icd[bb] = new SET[REG];
    }
    catch( std::bad_alloc ){
      error("ICD::ICD() bad_alloc");
    }
  }
}

// destructor
ICD::~ICD(){
  for( int bb = bb_size - 1; bb >= 0; bb -- ){
    delete[] icd[bb];
  }
  delete[] icd;
}

// icd analysis
void ICD::analysis(Program_Info &program, DU_Chain &du_chain){
  for( int bb = 0; bb < bb_size; bb ++ ){// LOOP bb
    // construction
    Func_Bb fbb(func, bb);
    Bb_Info info = program.get_info(fbb);
    DU_Chain::MMAP use_chain = du_chain.get_use(bb);

    // candiate defined same register at other paths
    SET def_bb[REG];

    // �����2�ս�ʾ�Τ�Τ��Ȥ��
    for( int pc = info.start; pc <= info.end; pc ++ ){// LOOP pc
      // ���̻Ҥϰ㤦���⤷��ʤ������������ս�ʾ夢��
      if( use_chain.count(pc) > 1 ){
	DU_Chain::MI_PAIR pair = use_chain.equal_range(pc);

	for( DU_Chain::MI map_i = pair.first;
	     map_i != pair.second; map_i ++ ){// LOOP def
	  DU_Data def = map_i->second;

	  // ���̻����ʬ��
	  def_bb[def.reg].insert(def.bb);
	}// LOOP def
      }
    }// LOOP pc

    for( int reg = 0; reg < REG; reg ++ ){// LOOP reg
      // �ؿ��λ�����������ޤǤ��̲᤹����ܥ֥�å��ν���
      SET all_pass_bb;

      // Ʊ�����̻Ҥ������2�ս�ʾ�ξ��
      if( def_bb[reg].size() > 1 ){
	// �ؿ��λ�����������ޤǤ��̲᤹����ܥ֥�å��ν�������
	all_pass_bb = pass_bb(program, def_bb[reg]);

	// �ƥ쥸������˳��������Ȥ��
	icd[bb][reg] = defined_bb(program, all_pass_bb);
      }
    }// LOOP reg
  }// LOOP bb
}

// �ؿ��λ�����������ޤǤ��̲᤹����ܥ֥�å��ν�������
ICD::SET ICD::pass_bb(Program_Info &program, SET &def_bb){
  SET all_pass_bb;

  for( SI set_i = def_bb.begin(); set_i != def_bb.end(); set_i ++ ){
    // construct
    const int bb = *set_i;
    STACK st;
    SET pass_bb;

    // search pass bb (predecessor)
    pass_bb.insert(bb);
    sub_insert(pass_bb, st, 0);

    while( !st.empty() ){
      const int m = st.top();
      Program_Info::SET pred = program.get_pred(Func_Bb(func, m));

      st.pop();

      for( Program_Info::SI set_i = pred.begin();
	   set_i != pred.end(); set_i ++ ){// LOOP pred
	sub_insert(pass_bb, st, *set_i);
      }// LOOP pred
    }

    // merge pass_bb
    for( SI set_i = pass_bb.begin(); set_i != pass_bb.end(); set_i ++ ){
      all_pass_bb.insert(*set_i);
    }
  }

  return all_pass_bb;
}

// stack argolism subroutine
void ICD::sub_insert(SET &pass, STACK &st, const int &m){
  // "m" not include "pass_bb"
  if( !pass.count(m) ){
    pass.insert(m);// pass = pass | {m}
    st.push(m);
  }
}

// �ƥ쥸������˳��������Ȥ��
ICD::SET ICD::defined_bb(Program_Info &program, SET &all_pass_bb){
  SET icd_bb;

  // all_pass_bb: �ؿ��λ�����������ޤǤ��̲᤹����ܥ֥�å���������
  for( SI set_i = all_pass_bb.begin(); set_i != all_pass_bb.end(); set_i ++ ){
    const int pass_bb = *set_i;
    Program_Info::SET succ = program.get_succ(Func_Bb(func, pass_bb));
    bool mark = false, no_mark = false;

    for( Program_Info::SI set_i = succ.begin();
	 set_i != succ.end(); set_i ++ ){// LOOP succ
      if( all_pass_bb.count(*set_i) ){
	mark = true;
      }else{
	no_mark = true;
      }
    }// LOOP succ

    if( mark && no_mark ){
      // no_mark�����������椬�ܹԤ�����硢��������ꤹ�뤬��
      // mark�����������椬�ܹԤ�����硢����ϳ��ꤷ�ʤ���

      // ���Υ֥�å��θ�³��ϡ�all_pass_bb�˴ޤޤ�Ƥ����Τ�
      // �ޤޤ�Ƥ��ʤ���Τ����롣

      for( Program_Info::SI set_i = succ.begin();
	   set_i != succ.end(); set_i ++ ){// LOOP succ
	if( !all_pass_bb.count(*set_i) ){
	  icd_bb.insert(*set_i);
	}
      }// LOOP succ
    }else if( !mark && no_mark ){
      // ���δ��ܥ֥�å��ǡ���������ꡣ
      // ���Υ֥�å��θ�³���all_pass_bb�˴ޤޤ�ʤ���

      icd_bb.insert(pass_bb);
    }
  }

  return icd_bb;
}

// file out
void ICD::print(std::ofstream &fout){
  fout << "{" << func << ":" << fname << std::endl;

  for( int bb = 0; bb < bb_size; bb ++ ){// LOOP bb
    for( int reg = 0; reg < REG; reg ++ ){// LOOP reg
      if( icd[bb][reg].empty() ){
	continue;
      }

      fout << bb << ":" << reg << ":";

      for( SI set_i = icd[bb][reg].begin();
	   set_i != icd[bb][reg].end(); set_i ++ ){
	fout << *set_i << ",";
      }

      fout << ";" << std::endl;
    }// LOOP reg
  }// LOOP bb

  fout << "}" << std::endl;
}

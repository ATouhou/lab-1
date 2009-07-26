//
// def_use.cc
//

#include <iostream>
#include <fstream>
#include <cstdio>
#include "def_use.h"

#include "main.h"

//
// class Def_Use
//

// constructor
Def_Use::Def_Use(Program_Info &program, const int &f){
  func = f;
  bb_size = program.size(func);
  fname = program.funcname(func);

  SET return_use_reg;
  SET jal_use_reg;

  // �ؿ��ƤӽФ��ʹߤǻ��Ѥ�����ǽ���Τ���쥸����
  // global register�ϤȤꤢ����̵��
  for( int reg = 0; reg < REG; reg ++ ){
    if( reg == 2 ){
      // return value
      return_use_reg.insert(reg);
    }else if( reg == 3 ){
      // return value
//        return_use_reg.insert(reg);
    }else if( 4 <= reg && reg <= 7 ){
      // argment
      jal_use_reg.insert(reg);
    }else if( 16 <= reg && reg <= 23 ){
      // save reg
      return_use_reg.insert(reg);
    }else if( reg == 29 || reg == 30 ){
      // stack pointer, frame pointer
//        return_use_reg.insert(reg);
    }
  }

  try{
    // kill/gen
    kill = new BITSET[bb_size];
    gen = new BITSET[bb_size];
    // def/use
    def = new BITSET[bb_size];
    use = new BITSET[bb_size];
  }
  catch( bad_alloc ){
    error("Def_Use::Def_Use() bad_alloc");
  }

  // file read
  string buf;

  // function name check
  getline(model.fin_asm_du, buf);
  if( model.fin_asm_du.eof() ){
    error("Def_Use::Def_Use() EOF");
  }
  if( buf.find("{") == string::npos ){// function name
    error("Def_Use::Def_Use() {");
  }
  if( program.funcname(func) != buf.substr(buf.find(":") + 1, string::npos) ){
    cerr << "bb_info funcnaem " << program.funcname(func)
	 << ", asm_du funcname "
	 <<  buf.substr(buf.find(":") + 1, string::npos);
    error("Def_Use::Def_Use() funcname");
  }

  while(true){// LOOP all du of func
    getline(model.fin_asm_du, buf);
    if( buf == "}" ){
      break;
    }
    if( buf.find(":") == string::npos || buf.find(",") == string::npos
	|| buf.find(" ") == string::npos || buf.find(";") == string::npos ){
      cerr << buf << endl;
      error("Def_Use::Def_Use() file format error");
    }

    int pc, def_reg, use_reg1, use_reg2;
    SET use_reg;
    bool jal = false;

    sscanf( buf.substr(0, buf.find(":")).c_str(), "%x", &pc );
    buf.erase(0, buf.find(":") + 1);
    def_reg = atoi( buf.substr(0, buf.find(",")).c_str() );
    buf.erase(0, buf.find(",") + 1);

    // atoi("C")�ξ��ϡ�0����������
    use_reg1 = atoi( buf.substr(0, buf.find(" ")).c_str() );
    buf.erase(0, buf.find(" ") + 1);

    if( buf.find(":") == string::npos ){
      // atoi("C")�ξ��ϡ�0����������
      use_reg2 = atoi( buf.substr(0, buf.find(" ")).c_str() );
    }else{
      // atoi("C")�ξ��ϡ�0����������
      use_reg2 = atoi( buf.substr(0, buf.find(" ")).c_str() );
      buf.erase(0, buf.find(":") + 1);

      if( buf == "JAL;" ){
	use_reg = jal_use_reg;
	jal = true;
	// �ؿ�������� 2, 3 (v1�ϴؿ��֤ǰ�¸�ط����ʤ��Ȼפ���)
	def_reg = 2;
      }else if( buf == "JR-RA;" ){
	// jr ra 
	// ���Ѥ�����ǽ���Τ���쥸����: 16-23, 29, 30
	use_reg = return_use_reg;
      }else{
	error("Def_Use::Def_Use() inst type flag");
      }
    }

    if( use_reg1 ){
      use_reg.insert(use_reg1);
    }
    if( use_reg2 ){
      use_reg.insert(use_reg2);
    }

    // construction
    DU_Data du_data(pc, def_reg, use_reg, jal);

    // insert readdata and correspondence pc/def id number
    insert_data(du_data);
  }// LOOP all du of func

  // check overflow
  if( pc_id_num.size() > bitset_size ){
    cerr << "pc_id_num.size():" << pc_id_num.size()
	 << " bitset_size " << bitset_size << endl;
    error("Def_Use::Def_Use() bitset_size (main.h)");
  }
}

// destructor
Def_Use::~Def_Use(){
  // read data
  pc_id_num.clear();
  du_data.clear();
  // clear kill/gen
  delete[] kill;
  delete[] gen;
  // clear def/use
  delete[] def;
  delete[] use;
}

// insert readdata and correspondence pc/def id number
void Def_Use::insert_data(DU_Data &data){
  data.set_id( du_data.size() );
  pc_id_num.insert( make_pair( data.get_pc(), data.get_id() ) );
  du_data.push_back(data);
}

// make kill/gen for analysis reach definiton and make du-chain
void Def_Use::make_kill_gen(Program_Info &program){
  // GEN[B]: �������줿����ν��硣�֥�å�B�ν������ã����֥�å�B������
  for( int bb = 0; bb < bb_size; bb ++ ){// LOOP bb
    Func_Bb fbb(func, bb);
    const int start_id = get_id( program.get_info(fbb).start );
    const int end_id = get_id( program.get_info(fbb).end );
    // defined flag (Ʊ�����ܥ֥�å����Ʊ���쥸������2��ʾ�����������)
    bitset< REG > check_reg;

    for( int id = end_id; id >= start_id; id -- ){// LOOP id of bb
      const int def = get_data(id).get_def();

      // Ʊ����ܥ֥�å����Ʊ���쥸������2��ʾ�����������򤵤���
      if( def != 0 && !check_reg[def] ){
	// gen
	gen[bb].set(id);
	// identifire
	def_id[def].insert(id);
	// defined flag
	check_reg.set(def);
      }
    }// LOOP id of bb
  }// LOOP bb

  // KILL[B]: B��Ǥ��������Ƥ��뼱�̻Ҥ�������Ƥ���B�γ��ˤ�������ν���
  for( int bb = 0; bb < bb_size; bb ++ ){// LOOP bb
    Func_Bb fbb(func, bb);
    const int start_id = get_id( program.get_info(fbb).start );
    const int end_id = get_id( program.get_info(fbb).end );
    // defined flag (Ʊ�����ܥ֥�å����Ʊ���쥸������2��ʾ�����������)
    bitset< REG > check_reg;

    for( int id = end_id; id >= start_id; id -- ){// LOOP id
      const int def = get_data(id).get_def();

      if( def != 0 && !check_reg[def] ){
	if( def_id[def].count(id) ){
	  for( SI set_i = def_id[def].begin();
	       set_i!= def_id[def].end(); set_i ++ ){// LOOP def_id
	    const int id = *set_i;

	    if( id < start_id || end_id < id ){
	      kill[bb].set(id);
	    }
	  }// LOOP def_id
	}
	// defined flag
	check_reg.set(def);
      }
    }// LOOP id
  }// LOOP bb
}

// make get def/use for analysis livereg
void Def_Use::make_def_use(Program_Info &program){
  // ���ܥ֥�å�B���Ф���IN[n]�����
  // �֥�å�n��ľ�������������Ƥ���̾���ν���

  for( int bb = 0; bb < bb_size; bb ++ ){// LOOP bb
    Func_Bb fbb(func, bb);
    const int start_id = get_id( program.get_info(fbb).start );
    const int end_id = get_id( program.get_info(fbb).end );

    for( int id = start_id; id <= end_id; id ++ ){// LOOP id
    }// LOOP id
  }// LOOP bb

  // ���ܥ֥�å�B���Ф���OUT[n]�ʲ��ν�������
  // �֥�å�n��ľ������������Ƥ���̾���ν���

  // USE[n]: n������򤹤�����n���Ѥ��Ƥ���̾���ν���
}

// test code
void Def_Use::print(){
  for( int id = 0; (unsigned)id < du_data.size(); id ++ ){
    DU_Data data = get_data(id);
    DU_Data::SET use = data.get_use();

    cout << hex << data.get_pc() << ":" << dec << data.get_def() << ",";

    for( DU_Data::SI set_i = use.begin(); set_i != use.end(); set_i ++ ){ 
      cout << *set_i << " ";
    }
    cout << endl;
  }

  exit(0);
}

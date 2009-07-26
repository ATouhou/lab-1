#!/usr/bin/perl -w

# analysis jump table and make bb_info

####################################################################
#
# sslittle-na-sstrix-objdump -Dw *.ss > objdump�����ϥե�����Ȥ��롣
# objdump���顢jr̿���������ν���Ǥ��롢jump table����Ϥ��롣
# ��᤿leader��ʬ����/���������ꡢsucceccor, predecessor�ν���
# ����ܥ֥�å���˵�ᡢ���ܥ֥�å�����bb_info��������롣
#
####################################################################

# main
my( $true, $false ) = ( 1, 0 );
# �����ν���
my( $objdump, $bb_info, $jr_table ) = &init;
# jump table
my( %jump_table, $total_func );
# bb info
my( %target, @j_pc, @return_pc, @break_pc, @leader, %succ, %pred );

# jump table�β���
&search_jump_table;
&print_jump_table;

# bb_info������
&make_all_bb_info;
exit 0;
# main end

# subroutine #######################################################
#
# init: �����ν���
# eof_text: text segment�ν�λ�򸡺�
#
# search_jump_table: ��jump table��base address���������pc��õ��
# print_jump_table: ��jr $2̿���pc�ˤĤ��ơ��б�����jump_table�����
#
# make_all_bb_info: ���ؿ���bb_info��׻�
# branch_jump_pc: �ؿ����ʬ��̿�ᡢjump̿�ᡢ�ؿ��ƤӽФ��򸡺�
# make_bb_info: �ؿ����leader��׻�����successor��predecessor��׻�
# print_bb_info: �ؿ����bb_info�����
#
####################################################################

# �����ν���
sub init{
    my( $objdump, $bb_info, $jr_table );

    if( @ARGV == 2 && $ARGV[0] eq "-dir" ){
	# filename
	$objdump = $ARGV[1] . "/objdump";
	$jr_table = $ARGV[1] . "/jr_table";
	$bb_info = $ARGV[1] . "/bb_info";

	open( INFO, ">$bb_info" ) || die " can't open $bb_info";

	return( $objdump, $bb_info, $jr_table )
    }else{
	# usage
	printf "Usage: $0 -dir [TARGET DIR]\n";
	printf "\ttarget file \"objdump\"\n";
	exit 1;
    }
}

# text segment�ν�λ�򸡺�
sub eof_text{
    ( $_ ) = @_;
    if( /^Disassembly of section .rdata:$/ ){
	return $true;
    }else{
	return $false;
    }
}

# ��jump table��base address���������pc��õ��
sub search_jump_table{
    my( @table, @jr_pc );

    open( OBJ, "$objdump" ) || die "Can't open $objdump:";
    $total_func = 0;

    # �ե���������å��ȥإå��򥹥��å�
    while( <OBJ> ){
	if( /^$/ ){
	    next;
	}

	if( /^Disassembly of section .text:$/ ){
	    if( <OBJ> =~ /^$/ ){
		last;
	    }else{
		die "File format error $objdump";
	    }
	}elsif( !/^.+\sfile format ss-coff-little$/ ){
	    die "File format error $objdump:";
	}
    }

    print STDERR "\rsearching jr \$2";

    # �쥸����jump̿��θ���
    while( <OBJ> ){
	if( /^00\w+\s\<.+$/ ){
	    # �ؿ��γ���
	    $total_func ++;
	}elsif( /^\s+(\w+):.+\s\sjr\s\$2/ ){
	    # jr $2̿��
	    push( @jr_pc, hex $1 );
	}elsif( &eof_text( $_ ) == $true ){
	    last;
	}
    }

    # ��Ƭpc����pop�Ǥ���褦�˽��֤��ѹ�
    @jr_pc = reverse @jr_pc;

    my( $table_flag ) = ( $false );

    # jump table��õ��
    while( <OBJ> ){
	if( $table_flag == $false ){
	    # jump table��header��õ��
	    if( /^1.+\s\<\$L\d+\>:$/ ){
		$table_flag = $true;
	    }elsif( !@jr_pc ){
		last;
	    }
	}else{
	    # jump table��
	    if( /^1.+\s\s0x00(\w+):00(\w+)$/ ){
		# jump table���data
		my( $addr_1, $addr_2 ) = ( hex $1, hex $2 );

		push( @table, $addr_1 ) unless $addr_1 == 0;
		push( @table, $addr_2 ) unless $addr_2 == 0;
	    }elsif( /^1.+:\s([\da-f ]+)\s[\w\.]+\s.+$/ ){
		# jump table���data (op code���Ѵ�����Ƥ��ޤäƤ�����)
		my( @two ) = split( " ", $1 );
		my( $addr_1 ) = ( hex $two[3].$two[2].$two[1].$two[0] );
		my( $addr_2 ) = ( hex $two[7].$two[6].$two[5].$two[4] );

		push( @table, $addr_1 ) unless $addr_1 == 0;
		push( @table, $addr_2 ) unless $addr_2 == 0;
	    }elsif( /^$/ ){
		# jump table�ν�ü
		# @table��sort, unique
		my( %seen, $pc );

		foreach $pc ( sort @table ){
		    push( @{ $jump_table{ $jr_pc[-1] } }, $pc )
			unless $seen{ $pc } ++;
		}

		# �б�����jr̿���pc����
		pop( @jr_pc );
		# ���ꥢ
		undef @table;
		$table_flag = $false;
	    }elsif( /^\s.+$/ ){
		next;
	    }else{
		die "jump table error";
	    }
	}
    }
}

# ��jr $2̿���pc�ˤĤ��ơ��б�����jump_table�����
sub print_jump_table{
    my( $pc, $target_pc );

    open( JR_TABLE, ">$jr_table" ) || die " can't open $jr_table";

    foreach $pc ( sort keys %jump_table ){
	printf JR_TABLE "%x:", $pc;
	foreach $target_pc ( @{ $jump_table{ $pc } } ){
	    printf JR_TABLE "0x%x,", $target_pc;
	}
	printf JR_TABLE "\n";
    }

    close( JR_TABLE );
    print STDERR "\rcreate jump table\n";
}

# ���ؿ���bb_info��׻�
sub make_all_bb_info{
    open( OBJ, "$objdump" ) || die "Can't open $objdump:";

    # �ե���������å��ȥإå��򥹥��å�
    while( <OBJ> ){
	if( /^$/ ){
	    next;
	}

	if( /^Disassembly of section .text:$/ ){
	    if( <OBJ> =~ /^$/ ){
		last;
	    }else{
		die "File format error $objdump";
	    }
	}elsif( !/^.+\sfile format ss-coff-little$/ ){
	    die "File format error $objdump:";
	}
    }

    my( $func, $func_start_pc, $func_end_pc, $fname ) = 0;

    # �ؿ����bb_info�����
    while( <OBJ> ){
	if( /^\s+.+:.+$/ ){
	    # nop̿��ʳ���̿��
	    # �ؿ����ʬ��̿�ᡢjump̿�ᡢ�ؿ��ƤӽФ��򸡺�
	    ( $func_end_pc ) = &branch_jump_pc( $_, $fname, $func_end_pc );
	}elsif( /^00(\w+)\s\<(\w+)\>:$/ ){
	    # �ؿ��γ���
	    ( $func_start_pc, $fname ) = ( hex $1, $2 );
	}elsif( /^$/ ){
	    # �ؿ����bb_info�����
	    &make_bb_info( $func_start_pc, $func_end_pc );
	    &print_bb_info( $func, $fname );
	    $func ++;
	    &clear;
	}elsif( &eof_text( $_ ) == $true ){
	    # �ؿ����bb_info�����
	    &make_bb_info( $func_start_pc, $func_end_pc );
	    &print_bb_info( $func, $fname );
	    $func ++;
	    &clear;
	    print STDERR "\n!!! create all bb_info $func!!!\n";
	    return;
	}elsif( /^1.+$/ ){
	    die "section .rdata";
	}
    }
}

# ���δؿ��Τ���ν���
sub clear{
    undef %target;
    undef @j_pc;
    undef @return_pc;
    undef @break_pc;
    undef @leader;
    undef %succ;
    undef %pred;
}

# �ؿ����ʬ��̿�ᡢjump̿�ᡢ�ؿ��ƤӽФ��򸡺�
sub branch_jump_pc{
    my( $fname, $pc, $op, $target_pc, $target_fname, $reg, $end_pc );
    ( $_, $fname, $end_pc ) = @_;

    if( /^\s+(\w+):\s.+\s\s(.+)$/ ){
	( $pc, $_ ) = ( hex $1, $2 );
    }

    if( /^.+\>$/ ){
	# ...<...>
	if( /^(b\w+)\s.+,(\w+)\s\<(\w+)\+\w+\>$/ ){
	    # op $d,$d,pc <fname+offset>
	    # ʬ��̿��
	    ( $op, $target_pc, $target_fname ) = ( $1, hex $2, $3 );

	    if( $fname eq $target_fname ){
		if( $fname eq "__brk" && $op eq "bne" ){
		    # syscall̿��μ���bne̿�� (<syscall_error>�ؿ�������)
		    # ������ʬ���褬1�Ĥ�ʬ��̿��Ȥ���������
		    push( @{ $target{ $pc } }, $pc + 8 );
		}else{
		    # �̾��ʬ��̿��
		    push( @{ $target{ $pc } }, $pc + 8 );
		    push( @{ $target{ $pc } }, $target_pc );
		}
	    }else{
		# syscall̿��μ���bne̿��
		if( $fname eq "_exit" ){
		    # _exit�ؿ��ν�übne̿�� (�ؿ����jr $ra̿�᤬�ʤ�)
		    # �������ؿ��ν�ü�Ȥ��ư���������
		    push( @return_pc, $pc );
		}else{
		    # syscall̿��μ���bne̿�� (<syscall_error>�ؿ�������)
		    # ������ʬ���褬1�Ĥ�ʬ��̿��Ȥ���������
		    push( @{ $target{ $pc } }, $pc + 8 );
		}
	    }
	}elsif( /^j\s(\w+)\s\<\w+\+\w+\>$/ ){
	    # op pc <fname+offset>
	    # j̿�� (�����褬�ؿ���)
	    $target_pc= hex $1;
	    push( @{ $target{ $pc } }, $target_pc );
	    push( @j_pc, $pc );
	}elsif( /^j\s(\w+)\s\<syscall_error\>$/ ){
	    # j̿�� (�����褬�ؿ���)
	    if( $fname eq "__handler" ){
		# �������ؿ��ν�ü�Ȥ��ư���������
		push( @return_pc, $pc );
	    }else{
		# ������nop̿��Ȥ��ư���������
		return( $end_pc );
	    }
	}elsif( /^jal\s\w+\s\<(\w+)\>$/ ){
	    # op pc <fname>
	    # jal̿��
	    $target_fname = $1;

	    if( $fname eq "__start" && $target_fname eq "exit" ){
		# __start�ؿ��ν�üjalr̿�� (�ؿ����jr $ra̿�᤬�ʤ�)
		# �������ؿ��ν�ü�Ȥ��ư���������
		push( @return_pc, $pc );
	    }else{
		push( @{ $target{ $pc } }, $pc + 8 );
	    }
	}elsif( /^b\w+\s(\w+)\s\<\w+\+\w+\>$/ ){
	    # op pc <fname+offset>
	    # bc1fʬ��̿��
	    $target_pc = hex $1;

	    push( @{ $target{ $pc } }, $pc + 8 );
	    push( @{ $target{ $pc } }, $target_pc );
	}elsif( /^b\w+\s.+,(\w+)\s\<(\w+)\>$/ ){
	    # op pc <fname>
	    ( $target_pc, $target_fname ) = ( hex $1, $2 );

	    # �̾��ʬ��̿�� (�ؿ�����Ƭ��ʬ��)
	    push( @{ $target{ $pc } }, $pc + 8 );
	    push( @{ $target{ $pc } }, $target_pc );
	}elsif( /^j\s(\w+)\s\<(\w+)\>$/ && $fname eq "__setjmp" ){
	    # __setjmp�ؿ��ν�ü��j̿��
	    # �������ؿ��ν�ü�Ȥ��ư���������
	    push( @return_pc, $pc );
	}else{
	    print;
	    die;
	}
    }elsif( /^jalr\s.+$/ ){
	# jalr̿�� (function call)
	push( @{ $target{ $pc } }, $pc + 8 );
    }elsif( /^jr\s\$(\d+)$/ ){
	# jr̿��
	$reg = $1;

	if( $reg == 31 ){
	    # jr $ra̿�� (�ؿ��ν�λ)
	    push( @return_pc, $pc );
	}elsif( $reg == 2 ){
	    # jr $2̿�� (�б�����jump table��target)
	    push( @{ $target{ $pc } }, @{ $jump_table{ $pc } } );
	}else{
	    die "$_\n jr $reg";
	}
    }elsif( $fname eq "__sigreturn" && /^syscall\s$/ ){
	# __sigreturn�ؿ���syscall̿�� (�ؿ����jr $ra̿�᤬�ʤ�)
	# �������ؿ��ν�ü�Ȥ��ư���������
	push( @return_pc, $pc );
    }elsif( /^break\s$/ ){
	# break̿��
	# ľ����ʬ��̿�᤬����
	# �������ؿ��ν�λ������
	push( @break_pc, $pc );

	# abort�ؿ���break̿��ϡ�ľ����ʬ��̿�᤬�ʤ�
	# ���������������ɤ����ɡ�leader����˵��Ƥ���������
	if( $fname eq "abort" ){
	    push( @leader, $pc + 8 );
	}
    }

    # �ؿ��ν�λpc (leader����뤿�ᡢ8­��)
    return( $pc + 8 );
}

# �ؿ����leader��׻�����successor��predecessor��׻�
sub make_bb_info{
    my( $start_pc, $end_pc ) = @_;
    my( %start, %end, $pc, $target_pc, $bb, $to );

    # %target, @return_pc, $start_pc, $end_pc��ꡢleader�����
    push( @leader, $start_pc );
    push( @leader, $end_pc );
    foreach $pc ( keys %target ){
	push( @leader, @{ $target{ $pc } } );
    }
    foreach $pc ( @return_pc ){
	push( @leader, $pc + 8 );
    }

    # ̵���j̿��μ��δ��ܥ֥�å������椬�ܹԤ��ʤ���
    # �ü�ʴؿ��������롣����ʳ��δؿ��ˤ����Ƥϡ���Ĺ
    foreach $pc ( @j_pc ){
	push( @leader, $pc + 8 );
    }

    # @leader��sort, unique
    my( %seen, @temp );

    @temp = sort @leader;
    undef @leader;
    foreach $pc ( @temp ){
	push( @leader, $pc ) unless $seen{ $pc } ++;
    }

    # start pc/end pc�η׻�
    foreach( $bb = 0; $bb < $#leader; $bb ++ ){
	$start{ $leader[$bb] } = $bb;
	$end{ $leader[$bb + 1] - 8 } = $bb;
    }

    # �ؿ�������
    #push( @{ $pred{ 0 } }, "in" );

    # target��ꡢsuccessor��predecessor�����
    foreach $pc ( keys %target ){
	my( $bb ) = $end{ $pc };

	foreach $target_pc ( @{ $target{ $pc } } ){
	    # ʬ��, j, jal, jalr, jr $2̿���pc���顢target pc�ؤ�ή��
	    my( $to ) = $start{ $target_pc };

	    if( !defined $bb || !defined $to ){
		die;
	    }

	    push( @{ $succ{ $bb } }, $to );
	    push( @{ $pred{ $to } }, $bb );
	}
    }

    # return_pc��ꡢsuccessor��exit������
    foreach $pc ( @return_pc ){
	my( $bb, $to ) = ( $end{ $pc }, $start{ $pc + 8 } );

	push( @{ $succ{ $bb } }, "exit" );

	# jr ra̿�᤬�ؿ����ʣ��¸�ߤ�����
	if( defined $to ){
	    push( @{ $pred{ $to } }, "IN" );
	}
    }

    # break̿��
    #�������Ȥꤢ�����ؿ��ν�λ������
    foreach $pc ( @break_pc ){
	my( $bb ) = ( $end{ $pc } );

	if( defined $bb ){
	    push( @{ $succ{ $bb } }, "break" );
	}else{
	    die;
	}
    }

    # leader�μ�����̿�᤬���̾��̿��ξ��
    foreach( $bb = 0; $bb < $#leader; $bb ++ ){
	if( !defined $succ{ $bb } && ( $bb == 0 || defined $pred{ $bb } ) ){
	    $to = $bb + 1;
	    push( @{ $succ{ $bb } }, $to );
	    push( @{ $pred{ $to } }, $bb );
	}
    }
}

# �ؿ����bb_info�����
sub print_bb_info{
    my( $func, $fname, $bb );
    ( $func, $fname ) = @_;

    open( INFO, ">>$bb_info" ) || die " can't open $bb_info";

    # start function
    print INFO "{$func:$fname\n";

    foreach( $bb = 0; $bb < $#leader; $bb ++ ){
	# bb number, start pc, end pc
	printf INFO "%d:%x:%x:", $bb, $leader[$bb], $leader[$bb+1] - 8;

	# successor
	foreach $bb ( sort { $a <=> $b } @{ $succ{ $bb } } ){
	    print INFO "$bb ";
	}
	print INFO ":";

	# predecessor
	if( defined $pred{ $bb } ){
	    foreach $bb ( sort { $a <=> $b } @{ $pred{ $bb } } ){
		print INFO "$bb ";
	    }
	}
    	print INFO ";\n";
    }

    # end function
    print INFO "}\n";
    print STDERR "\rcreating bb_info ($func/$total_func)";
}

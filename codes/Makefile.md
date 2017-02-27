#general Makefile
This Makefile supports complex project.  

**目录结构：**  

>     include: 此目录用于存放所有 .h 头文件，可在其中创建子文件夹
>     src    : 此目录用于存放所有 .c 源文件，可在其中创建子文件夹
>     build  : 此目录用去存放编译生成的文件

**实现原理：**  
>     1. 在 build 目录中执行make，当前目录下的Makefile称为主Makefile，此Makefile会调用src目录下的子Makefile;  
>     2. src 目录下的子Makefile，会编译当前目录下的所有源文件，并检索当前目录下的所有目录；  
>     3. 若检索到的子目录中存在Makefile，则编译该目录下的所有源文件；  

##Details
###codes: build/Makefile  

		 1: #root_Makefile
		 2: include ../src/Make.defines
		 3:
		 4: all : $(PROGRAM)
		 5: 		
		 6: $(PROGRAM) : sub_make
		 7:		$(CC) $(wildcard $(OBJS_DIR)/*.o) -o $@ $(LIBS)
		 8:
		 9: sub_make : 
		10:	@$(MAKE) -C $(ROOT_DIR)/src
		11: 
		12: .PHONY : clean
		13: 
		14: clean :
		15:		rm -rf $(OBJS_DIR) $(PROGRAM)

**build/Makefile - instructions:**  

		Makefile最关键的两个词：目标和依赖。 想要执行目标下面的代码，则要求所有依赖均存在
		注： 目标下方的代码，必须以一个 Tab 按键开头，即缩进一个 Tab 键
		
		 1: "#" 号为注释，root_Makefile 说明此 Makefile 是主文件
		 2: 把 src 目录下的 Make.defines 包含进来，以使用该文件中定义的变量
		 3: 
		 4: ":" 左边为目标，右边为依赖
			此处目标为 all ,表示要编译所有文件，目标名为自定义
			此处的依赖为 PROGRAM，此变量在 Make.defines 中定义
			此处的依赖在下方再次声明其它依赖，又称为依赖嵌套
		 5: 		
		 6: 此处声明 PROGRAM 依赖，PROGRAM 为新的目标，而 PROGRAM　又依赖于sub_make
		 7:		CC、OIBJS_DIR、LIBS 为 Make.defines 中定义的变量
				wildcard是个函数，功能：显示目录下所有指定的类型的文件，此处显示所有 .o 目标文件
				$@ 表示 ":" 左边的变量
		 8:
		 9: 目标sub_make没有依赖，则无条件执行下方的代码
		10:	@ 表示屏蔽掉后方指令的显示
			$(MAKE) -C 表示在指定目录下执行 make 指令，即在 ROOT_DIR/src目录下执行 make 指令
			ROOT_DIR 为 Make.defines 中定义的变量
		11: 
		12: .PHONY 表示定义个伪目标，即便目标不存在，也可继续执行，此处把 clean 定义为伪目标
		13: 
		14: clean 是一个目标，是一个伪目标，没有依赖。当执行make clean 时，执行下方代码
		15:		强制删除 OBJS_DIR 目录和 PROGRAM 文件
		
		具体变量含义，参考 Make.defines 的说明

###codes: build/cp_makefile.sh
		
		#!/bin/bash
		
		if [ ! -z $1 ];then
			DIR_DEPTH=$1
		else
    		DIR_DEPTH=1
		fi 
		
		SRCS_PATH=`find ../src/ -maxdepth $DIR_DEPTH -type d | sed 's/^'"..\/src\/"'//g'`
		cd ../src/ 
		
		for dir in $SRCS_PATH;
		do
    		cp Makefile $dir
		done

		
**build/cp_makefile.sh - instructions:**  
>     此脚本用于拷贝Makefile  
>     在 src 目录下检索文件夹目录，检索深度为 DIR_DEPTH
>     DIR_DEPTH 默认值为 1，若要改变检索深度，只需在执行脚本时输入期望的检索深度即可
>     此脚本会将 src 目录下的 Makefile 复制到检索到所有子目录中

###codes: src/Make.defines
		
		 1. #define some variables
		 2. 
		 3. PROGRAM = test
		 4. ROOT_DIR = $(shell cd ../ && pwd)
		 5. INCLUDES_DIR = $(shell find $(ROOT_DIR)/include/ -maxdepth 2 -type d | sed 's/^/-I&/g')
		 6. OBJS_DIR = $(shell pwd)/object
		 7. CC := gcc
		 8. CC_OPT := -c -o
		 9. CFLAGS := $(INCLUDES_DIR)
		10. LIBS :=
		11. $(shell mkdir -p $(OBJS_DIR))
		12. 
		13. #modify the variables
		14. 
		15. CFLAGS += -g -Wall -Wno-strict-aliasing -O3
		16. 
		17. export

**src/Make.defines - instructions:** 
		
		 1. 注释说明，此文件用于定义一些变量
		 2. 
		 3. 最终生成的可执行文件的名字。将 PROGRAM 定义为 test，即命名最终生成的可执行文件的名字为 test
		 4. 当前工程项目最顶层的路径。执行 shell 脚本命令，定义路径，此路径为绝对路径
		 5. 头文件的路径，为绝对路径，
			执行 shell 脚本命令，在 include 目录下，向下检索所有目录，检索深度设置为2，
			即检索 include 目录下的所有文件夹即其对应的子文件夹，
			将检索到的所有路径，赋值给 INCLUDES_DIR 
		 6. 定义目标文件的路径，定义为 build 目录下的 object 目录
		 7. CC 为编译器，一般为 gcc ，在 arm 环境下，为 arm-linux-gcc
		 	:= 和 = 的区别，在 Makefile 中，给变量赋值有这两种方法。区别，如：
			echo $(CC)
			CC = abc
			执行结果为 abc ，即使用 = 赋值的变量，作用域是整个 Makefile ，即便在 echo 之后定义，但依旧可获取其值,
			若：
			echo $(CC)
			CC = abc
			CC = def
			则最终输出的是 def 而不是abc ，若要输出 abcdef，只需写为 CC += def 即可
			echo $(CC)
			CC := abc
			执行结果为空，即使用 := 赋值的变量，作用域在定义的下方，在定义的上方无法访问
			
		 8. 编译选项，-c 指定要编译的源文件，-o 指定编译后输出的文件名
		 9. 此处将 CFLAGS 定义为头文件的路径，此变量用于加在编译命令的末尾
		10. 定义库文件的路径，此处未定义，为预留，某些项目需要用到动静态库，用此选项指定即可
			使用： LIBS := -l... 或 -L...，前着指定系统带有的库，后置指定自己的库，后方直接加库路径即可
		11. 执行脚本命令，生成目标文件存放的路径，此路径在执行 make clean 指令后会被删除，故而需要创建
		12. 
		13. 注释说明，下方的操作是经常修改的，除了 export
		14. 
		15. CFLAGS 最开始赋值为 INCLUDE_DIR ，即头文件路径，此处增加赋值，
			-g gdb 调试用，若想在对最终生成的可执行文件进行调试操作，需要添加此选项
			-Wall ，告知编译器显示出某些警告，编译时最好增加此选项，然后消除所有警告
			-Wno-strict-aliasing
			-O3 ，编译器在编译时会进行优化，3表示优化等级
		16. 
		17. 表示输出所有变量，子 Makefile 中才能使用


###codes: src/Makefile

		 1. #sub_Makefile
		 2. cur_dir := $(notdir $(shell pwd))
		 3. 
		 4. #$(cur_dir)_sub_dir := $(shell find . -maxdepth 1 -type d | sed '1d' | sed 's/^'".\/"'//g')
		 5. #or use the built-in functions: subst
		 6. $(cur_dir)_sub_dir := $(subst ./, ,$(shell find . -maxdepth 1 -type d | sed '1d'))
		 7. 
		 8. $(cur_dir)_srcs := $(wildcard *.c)
		 9. 
		10. $(cur_dir)_objs := $(patsubst %.c, $(OBJS_DIR)/%.o, $($(cur_dir)_srcs))
		11. 
		12. all : $($(cur_dir)_objs)
		13.	@for dir in $($(cur_dir)_sub_dir); \
		14. 	do \
		15. 	if [ -f $$dir/Makefile ];then \
		16. 		$(MAKE) -C $$dir; \
		17. 	fi; \
		18. 	done
		19. 
		20. $(OBJS_DIR)/%.o : %.c
		21. 	$(CC) $(CC_OPT) $@ $< $(CFLAGS)
		22. 
		23. .PHONY : clean
		24. 
		25. clean :

**src/Makefile - instructions:**   

		
		 1. 注释说明，此 Makefile 为子 Makefile
		 	注意：
			cp_makefile.sh 脚本中是将此脚本复制到 src 目录的子目录中
		 2. 取当前目录的路径，只保留目录名，删除所有绝对路径
		 3. 
		 4. 第4 - 6 行是在当前目录下检索所有子目录，检索深度设置为1，即只检索当前目录的子目录
		 5. 第4行是 shell 脚本的实现方式，第6行是用 Makefile 自带的函数实现
		 6. 由于检索到的目录是 ./ 开头的，所以要教将其去除，subst 是替换函数，将 ./ 替换为空 
		 7. 
		 8. 将当前目录下的所有 .c 源文件列出
		 9. 
		10. patsubst 如同 subst，是替换用的函数，patsubst 只替换字符串末尾，
			这里表示将当前目录下的所有 .c 源文件，后缀替换为 .o，并添加上 OBJS_DIR 路径
			注意：这里仅仅是将文件名替换，并非在 OBJS_DIR 下生成该文件
			如：当前目录下存在 main.c test.c， 则此处的变量值为 OBJS_DIR/main.o OBJS_DIR/test.o，
				至于 OBJS_DIR 的值，取决于 Make.defines 中赋予的值
		11. 
		12. all 为目标， $(cur_dir)_objs 为依赖
		13.	
		14. 13 - 18 行表示，在当前目录的子文件中，若存在 Makefile 文件，
		15. 则在该子文件夹中执行 make 指令。
		16. 以此类推，检索完所有子目录，满足条件的均执行 make 指令
		17. 
		18. 
		19. 
		20. 声明目标和依赖，%.o : %.c 表示为每一个 .o ，生成一个 .c 依赖
			如，目标为 OBJS_DIR/main.o OBJS_DIR/test.o ，则 Makefile 会自动把依赖补上，即
					OBJS_DIR/main.o OBJS_DIR/test.o : mian.c test.c 
		21. 替换编译，编译选项，$@ 表示取 : 号左侧的目标，$< 表示取 : 号右侧中其对应的依赖
			末尾再替换编译额外选项，一般为头文件路径、是否开启提示编译警告等
		22. 
		23. 声明 clean 为为目标
		24. 
		25. 所有编译过程生成的目标文件等均存放在 build/project 中，
			在主 Makefile 中执行 make clean 即可完成清除操作，此处无需执行任何操作，
			且子 Makefile 不可单独执行，依赖于主 Makefile ，所以此处不执行任何指令，
			在此处写上 clean : 是因为每个 Makefile 中都应该有一个 clean 目标，


###使用方法及执行流程
>     必须存在目录 src include build   
>     在 build 中执行 ./cp_makefile.sh  
>     在 build 中执行 make 指令即可  

执行流程（实例说明）：  
1. 工程项目文件夹为c_test，绝对路径为 `/root/c_test`  
2. c_test 中有 src include build 三个文件夹  
3. src 中有 3t common 子文件夹，同时还有源文件 main.c ,以及 Make.define Makefile;  
　3t 中存在 3t_test.c；common 中存在 log.c；  
5. build 中有 cp_makefile.sh Makefile；  
6. include 中有子文件夹 3t，和 log.h；  
　3t 中存在 3t_test.h  

>     进入 build 目录，执行 ./cp_makfile.sh ，实现：将 src 中的 Makefile 复制到 src/3t/ 及 src/common 中，
>     因为 src/3t 和 src/common 目录中存有源文件，需要编译；
>     在 build 目录下执行 make 指令；
>     执行流程：*******
>     将 src/Make.defines 文件中定义的变量，加入到当前 Makefile 中
>     **执行 src/Make.defines 下的所有操作，流程：
>     PROGRAM 为 test
>     ROOT_DIR 为 /root/c_test
>     INCLUDE_DIR 为 -I/root/c_test/include  -I/root/c_test/include/3t
>     OBJS_DIR 为 /root/c_test/build/object
>     CC 为 gcc
>     CC_OPT 为 -c -o
>     CFLAGS 为 INCLUDE_DIR 的值，即为 -I/root/c_test/include  -I/root/c_test/include/3t
>     LIBS 为空
>     执行 mkdir -p /root/c_test/build/object 命令，创建 object 文件夹
>     CFLAGS 增加赋值，最终的值为:
>          -I/root/c_test/include  -I/root/c_test/include/3t  -g -Wall -Wno-strict-aliasing -O3
>     输出所有变量，子 Makefile 中可使用
>     *执行 src/Make.defines 完毕，返回 build/Makefile 中
>     在 Makefile 中查找目标，目标为 all ；
>     all 目标依赖于 test，其下方不执行任何代码，因为目标下无代码
>     查找依赖 test
>     找到依赖 test，test 声明为目标，依赖于 sub_make，其下方有代码，但必须先查找到依赖才可执行
>     查找依赖 sub_make
>     找到依赖 sub_make，sub_make 无依赖，且下方有代码，无条件执行下方的代码
>     @ 不显示本行代码，只执行即可
>     在 /root/c_test/src 下执行 make 执行
>     **执行 src/Makefile ，下方为 src/Makefile 执行流程
>     cur_dir 为 src
>     src_sub_dir 为 3t common
>     src_srcs 为 mian.c
>     src_objs 为 /root/c_test/build/project/main.o
>     查找目标，目标为 all ，依赖于 /root/c_test/build/project/main.o
>     下方有代码，但依赖文件不存在，所以不执行，
>     向下查找依赖 /root/c_test/build/project/main.o，找到依赖，并声明为新的目标
>     /root/c_test/build/project/main.o ： main.c
>     目标依赖于 main.c
>     当前目录下存在 main.c，所以执行下方代码
>     执行: 
>     gcc -c -o /root/c_test/build/project/main.o mian.c \
>         -I/root/c_test/include -I/root/c_test/include/3t -g -Wall -Wno-strict-aliasing -O3
>     编译 main.c 文件，mian.c 中需要的头文件，在 /root/c_test/include 和 /root/c_test/include/3t 中查找
>     生成的目标文件main.o，存放于 /root/c_test/build/project中
>     返回目标 all，其依赖 /root/c_test/build/project/main.o 已经存在，故而执行下方的代码
>     由于 src_sub_dir 为 3t common，故而依次在 3t 和 common 下执行下方操作
>     dir 为 3t
>     若 3t 目录下存在 Makefilez，则执行代码，由先前执行过 cp_makefile.sh ，所以 3t 下有 Makefile
>     执行代码，在 3t 目录下执行 make 指令
>     ***跳转到 src/3t/Makefile
>     由于此 Makefile 和 /src/Makefile 是一样的，故而执行流程也是一模一样，此处不详说，
>     参考 src/Makefile 流程即可
>     **执行 src/3t/Makefile 完毕，返回 src/Makefile
>     dir 为common， 且 common 下存在 Makefile 所以在 common 下执行 make 指令
>     ***跳转到 src/common/Makefile
>     由于此 Makefile 和 /src/Makefile 是一样的，故而执行流程也是一模一样，此处不详说，
>     参考 src/Makefile 流程即可
>     **执行 src/common/Makefile 完毕，返回 src/Makefile
>     跳出循环
>     *执行 src/Makefile 完毕，返回 build/Makefile
>     sub_make 目标下方代码执行完毕，返回到目标 PROGRAM ，即返回到 test
>     执行目标 test 下方代码，执行：
>     gcc /root/c_test/build/project/main.o /root/c_test/build/project/3t_test.o \
>         /root/c_test/build/project/log.o -o test
>     生成 test 目标文件，即为最终的可执行文件
>     执行 src/Makefile 完毕，退出
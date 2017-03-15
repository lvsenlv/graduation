#sub_Makefile
cur_dir := $(notdir $(shell pwd))

#$(cur_dir)_sub_dir := $(shell find . -maxdepth 1 -type d | sed '1d' | sed 's/^'".\/"'//g')
#or use the built-in functions: subst
$(cur_dir)_sub_dir := $(subst ./, ,$(shell find . -maxdepth 1 -type d | sed '1d'))

$(cur_dir)_srcs := $(wildcard *.c)

$(cur_dir)_objs := $(patsubst %.c, $(OBJS_DIR)/%.o, $($(cur_dir)_srcs))
 
all : $($(cur_dir)_objs)
	@for dir in $($(cur_dir)_sub_dir); \
	do \
	if [ -f $$dir/Makefile ];then \
		$(MAKE) -C $$dir; \
	fi; \
	done

$(OBJS_DIR)/%.o : %.c
	$(CC) $(CC_OPT) $@ $< $(CFLAGS)

.PHONY : clean

clean :


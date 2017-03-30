#root_Makefile
include ../src/Make.defines

all : $(PROGRAM)

$(PROGRAM) : sub_make
	$(CC) $(wildcard $(OBJS_DIR)/*.o) -o $@ $(LIBS)

sub_make : 
	@$(MAKE) -C $(ROOT_DIR)/src

.PHONY : clean
	
clean :
	rm -rf $(OBJS_DIR) $(PROGRAM) log.txt

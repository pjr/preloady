TARGET    = preloady
SRCDIR    = src
OBJDIR    = build
INCDIRS   = -Iinclude
VPATH     = $(OBJDIR)

LDFLAGS   = $(shell python-config --ldflags) -dynamiclib
CFLAGS    = -Wall ${INCDIRS} $(shell python-config --cflags | sed -e 's/\-O.//g')
OBJS      = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(wildcard $(SRCDIR)/*.c))

.PHONY: all clean install
.PRECIOUS: $(OBJDIR)/%.o
all: $(TARGET)
debug: CC += -O0 -DDEBUG -g 
debug: $(TARGET) 

tcptest:
	$(CC) $(CFLAGS) test/tcp_test.c -o test/tcp_test

gdb: debug tcptest
	@rm -f test/gdb.sh
	@echo '#!/bin/bash'                               >> test/gdb.sh
	@echo '( echo set env DYLD_INSERT_LIBRARIES $(CURDIR)/build/$(TARGET).dylib ' >> test/gdb.sh
	@echo 'echo set env DYLD_FORCE_FLAT_NAMESPACE 1 ; ' >> test/gdb.sh
	@echo "echo set env PYTHONPATH $(CURDIR)/py " >> test/gdb.sh
	@echo 'echo run 192.168.1.13 testing ;' >> test/gdb.sh
	@echo 'cat ) | gdb $(CURDIR)/test/tcp_test' >> test/gdb.sh
	@chmod 0700 test/gdb.sh

preloady: build/$(TARGET).dylib 
	@rm -f $(TARGET).sh
	@echo '#!/bin/bash'                               >> $(TARGET).sh
	@echo 'export DYLD_INSERT_LIBRARIES=$(CURDIR)/$<' >> $(TARGET).sh
	@echo 'export DYLD_FORCE_FLAT_NAMESPACE=1'        >> $(TARGET).sh
	@echo 'PYTHONPATH="py" $$@'                       >> $(TARGET).sh
	@chmod 700 $(TARGET).sh

$(OBJDIR)/%.dylib: $(OBJDIR) $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) -c $(CFLAGS) -o $@ $<

$(OBJDIR):
	@echo "Creating '$(OBJDIR)' directory. All intermediary objects will go there.."
	@mkdir -p $@

clean:
	rm -rf *.o build/*.o test/*.o build $(TARGET).sh

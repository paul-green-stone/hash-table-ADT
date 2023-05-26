CC := gcc
OBJDIR := objects
CFLAGS := -c
AR := ar
ARFLAGS := -r -c

a.out: $(OBJDIR)/main.o $(OBJDIR)/table.o $(OBJDIR)/list.o $(OBJDIR)/guard.o
	$(CC) -g -o $@ $^

$(OBJDIR)/guard.o: linked-list-ADT/guard/guard.h linked-list-ADT/guard/guard.c
	$(CC) -g $(CFLAGS) -o $@ linked-list-ADT/guard/guard.c

$(OBJDIR)/list.o: linked-list-ADT/list.h linked-list-ADT/list.c
	$(CC) -g $(CFLAGS) -o $@ linked-list-ADT/list.c

$(OBJDIR)/table.o: src/table.h src/table.c
	$(CC) -g $(CFLAGS) -o $@ src/table.c

$(OBJDIR)/main.o: main.c
	$(CC) -g $(CFLAGS) -o $@ main.c

# Compile a static library with guard, linked-list, and hash-table object files
lib: $(OBJDIR)/table.o $(OBJDIR)/list.o $(OBJDIR)/guard.o
	$(AR) $(ARFLAGS) libtable.a $^

.PHONY: clean

clean:
	rm -rf $(OBJDIR) ./*.a ./*.o *.out

$(shell mkdir -p $(OBJDIR))
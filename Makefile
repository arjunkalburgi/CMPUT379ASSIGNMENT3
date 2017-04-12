CC=gcc 
main=./dependencies/main.c
tlb=./dependencies/tlb.c
ma=./dependencies/measurementarray.c
pgtbl=./dependencies/pgtbl.c
ffl=./dependencies/freeframeslist.c
# pgsize?="hey1"
# tlbentries?="hey2"
# gflag?="hey3"
# quantum?="hey4"
# physpages?="hey5"
# mflag?="hey6"
# trace1?="hey7"

# tvm379 pgsize tlbentries { g | p } 
	# quantum physpages { f | l } 
	# trace1 trace2 . . . tracen

default: $(main) $(tlb) $(ma) $(pgtbl) $(ffl)
	@$(CC) $< $(tlb) $(ma) $(pgtbl) $(ffl) -o tvm379 -lm

clean:
	for n in tests; do $(MAKE) -C $$n clean; done


# ./tvm379 16 heyy2 heyy3 10 heyy5 heyy6 heyy7
# @./tvm379 ${pgsize} ${tlbentries} ${gflag} ${quantum} ${physpages} ${mflag} ${trace1} 

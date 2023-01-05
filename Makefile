SRC = Application/app.c\
	  Card/card.c\
	  Terminal/terminal.c\
	  Server/server.c
	  
testSRC = 	tests/test.c\
 			Card/card.c\
	  		Terminal/terminal.c\
	  		Server/server.c
	  
	  
INCLUDE  = -I ./Application
INCLUDE += -I ./Card
INCLUDE += -I ./Server
INCLUDE += -I ./Terminal

CFLAGS = -Og -ggdb -g

app:$(SRC)
	gcc $(INCLUDE) $(CFLAGS) $(SRC) -o $@



tests = cardExpirayDateTest
tests += cardHolderNameTest
tests += cardPANTest
tests += TransactionDateTest
tests += TransactionAmountTest


# tests_not_interactive = CardExpiredTest
# tests_not_interactive += BelowMaxAmountTest
# tests_not_interactive += MaxAmountTest
# tests_not_interactive = recieveTransDataTest
# tests_not_interactive = ValidAccountTest
# tests_not_interactive = BlockedAccountTest
# tests_not_interactive = AmountAvailableTest
tests_not_interactive = saveTransTest







# inputTests = $(addsuffix .txt,$(addprefix tests/input/,$(tests)))
# outputTests = $(subst input,output,$(inputTests))

test:$(testSRC)
	@for tst in $(tests);do \
		# echo $$tst; \
		gcc $(INCLUDE) $(CFLAGS) $(testSRC) -D$$tst -o $$tst;\
		./$$tst <tests/input/$$tst.txt 1>/dev/null 2>tests/output/$$tst.txt;\
		echo =================;\
		cat tests/output/$$tst.txt;\
	done



test2:$(testSRC)
	@for tst in $(tests_not_interactive);do \
		# echo $$tst; \
		gcc $(INCLUDE) $(CFLAGS) $(testSRC) -D$$tst -o $$tst;\
		./$$tst 2>tests/output/$$tst.txt;\
		echo =================;\
		cat tests/output/$$tst.txt;\
	done

clean:
	@for tst in $(tests_not_interactive);do \
		rm $$tst;done
	
	@for tst in $(tests);do \
		rm $$tst;done
	
	rm tests/output/*.txt


# test:$(testSRC)
# 	$(foreach tst,$(tests),\
# 		$(shell echo $(tst))\
# 		$(shell gcc $(INCLUDE) $(CFLAGS) $(testSRC) -D$(tst) -o $(tst))\
# 		$(eval input = $(addsuffix .txt,$(addprefix tests/input/,$(tst)))) \
# 		$(eval output = $(subst input,output,$(input))) \
# 		$(shell ./$(tst) < $(input) 1>/dev/null 2>$(output))\
# 		cat $(output);\
# 	)

ec:
	@echo $(tests)
	@$(eval tests = fun)
	@echo $(tests)

	@echo $(inputTests)
	@echo $(outputTests)
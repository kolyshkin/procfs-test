BIN=mnt-umnt

all: test
.PHONY: all

test: $(BIN)
	bash ./test
.PHONY: test

%: %.c
	gcc -O2 -Wall $< -o $@
	sudo chown root $@
	sudo chmod +s $@

clean:
	rm -rf $(BIN) testdata
.PHONY: clean

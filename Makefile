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
	rm -rf testdata 2>/dev/null || true
	test -d testdata && find testdata -type d | xargs -r sudo umount || true
	rm -rf $(BIN) testdata
.PHONY: clean

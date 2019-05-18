This repository demonstrates a problem of `seq_file` in the Linux kernel.
In particular, when many mounts are added and removed in parallel, reading
`/proc/self/mounts` (or `mountinfo`, or `mountstats`) is unreliable. This
bug appears to be not filesystem specific, not it is kernel version specific
(to the best of my knowledge).

## Running

To run this on your Linux system:

```
git clone https://github.com/kolyshkin/procfs-test
cd procfs-test
make test
```

Note you need to either run `make test` as root, or have sudo working
to run a binary that does mount/umount (sudo might ask you for a password).

### Expected results

In case of no bugs, the output should look like
```
bash ./test
```
i.e. no errors of any kind.

### Actual results

When the bug is present, you'll see something like
```
bash ./test
./mnt-umnt: can't find /home/user/git/procfs-test/testdata/8
./mnt-umnt: can't find /home/user/git/procfs-test/testdata/75
....
```

## Test description

This test runs a number of short-lived processes in parallel.
Each of these processes does the following
(see [mnt-umnt.c](./mnt-umnt.c) for details):
1. mounts a tmpfs to a directory provided
2. finds that mount in `/proc/self/mounts`
3. unmounts that tmpfs

## History

This was found while trying to optimize aufs graph driver in Docker
engine, by making its locking more fine-grained. Previously, there
was a single global lock between Get and Put (i.e. mount and unmount),
so only one mount or unmount operation was possible at a time. With
finer-grained locking, multiple mounts and unmounts can happen in
parallel, and that revealed this very bug.

## Links

* https://github.com/moby/moby/pull/39107

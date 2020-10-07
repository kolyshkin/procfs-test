This repository demonstrates a Linux kernel /proc bug.
In particular, when many mounts are added and removed in parallel, reading
`/proc/self/mounts` (or `mountinfo`, or `mountstats`) is unreliable.

The bug existed in the Linux kernel for about 10+ years, and is recently fixed
in the kernel 5.8 by [commit 9f6c61f96f2d97](https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/commit/?id=9f6c61f96f2d97cbb5f7fa85607bc398f843ff0f). For older kernels, the only known
workaround is to re-read the whole file.

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
* https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/commit/?id=9f6c61f96f2d97cbb5f7fa85607bc398f843ff0f

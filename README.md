# procfs-test

This repository demonstrates a problem of `seq_file` in the Linux kernel.
In particular, when many mounts are added and removed in parallel, reading
`/proc/self/mounts` (or `mountinfo`, or `mountstats`) is unreliable.

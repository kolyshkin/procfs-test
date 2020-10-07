#include <stdio.h>
#include <err.h>
#include <mntent.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mount.h>

int main(int argc, char **argv)
{
	FILE *fp;
	struct mntent m, *p;
	char a[4096];
	char buf[4096 + 1024];
	int r, found=0, lines=0;
	char *target;

	if (argc != 2) {
		errx(1, "Usage: %s <mountpoint>", argv[0]);
	}
	target = argv[1];
	if (target[0] != '/') {
		errx(1, "%s: absolute path required", argv[0]);
	}

	/* 1. mount target */
	r = mount("none", target, "tmpfs", 0, NULL);
	if (r != 0) {
		err(2, "mount %s", target);
	}

	/* 2. look it up */
	fp = setmntent("/proc/self/mounts", "r");
	if (!fp) {
		err(3, "setmntent");
	}
	setvbuf(fp, a, _IOLBF, sizeof(a));
	while ((p = getmntent_r(fp, &m, buf, sizeof(buf)))) {
		lines++;
		if (!strcmp(p->mnt_dir, target)) {
			found++;
		}
	}
	endmntent(fp);
//	printf("%s: found %d entries for %s (%d lines seen)\n", argv[0], found, target, lines);
	if (found == 0) {
		fprintf(stderr, "%s: can't find %s\n", argv[0], target);
	}

	/* 3. umount */
	if (umount(target) != 0) {
		err(5, "umount %s", target);
	}

	return 0;
}


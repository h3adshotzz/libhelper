#define FAT(p) ((*(unsigned int *)(p) & ~1) == 0xbebafeca)

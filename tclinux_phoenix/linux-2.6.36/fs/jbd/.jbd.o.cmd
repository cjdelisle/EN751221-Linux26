cmd_fs/jbd/jbd.o := mips-buildroot-linux-gnu-ld  -m elf32btsmip   -r -o fs/jbd/jbd.o fs/jbd/transaction.o fs/jbd/commit.o fs/jbd/recovery.o fs/jbd/checkpoint.o fs/jbd/revoke.o fs/jbd/journal.o 

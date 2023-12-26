

do:
	gcc test/logtest.c src/logger.c src/worker.c -g /usr/lib/libutil.a

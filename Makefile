##### global settings #####

.PHONY: nemu entry testcase kernel run gdb test submit clean count count_
# .PHONY ����αĿ�꣬αĿ�겢��ֱ�Ӷ�Ӧʵ���ļ������Ǳ�ʾһЩ��������,�˴�������ԭ�еĻ�����,���count��count_����

# ---	Makefile
# +++	Makefile
# @@	-56,2	+56,2	@@
# -USERPROG	=	obj/testcase/mov
# +USERPROG	=	obj/testcase/mov-c
# ENTRY	=	$(USERPROG)

CC := gcc # ���������Ϊ gcc
LD := ld  # ����������Ϊ ld
CFLAGS := -MMD -Wall -Werror -c
# ����ѡ�
# -MMD: ���������ļ��������Զ������ļ�����
# -Wall: �������о���
# -Werror: ��������Ϊ��������е�ʱ��ͦ���ĵģ��������������ɱ�̵ĺ�ϰ�ߣ�
# -c: ֻ���룬������

count:
	@echo "Counting lines of code (including empty lines)..."
	@find . -name '*.[ch]' | xargs wc -l

count_:
	@echo "Counting lines of code (excluding empty lines)..."
	@find . -name '*.[ch]' | xargs grep -v '^\s*$$' | wc -l
# countͳ�Ƶ�ǰĿ¼������Ŀ¼������ `.c` �� `.h` �ļ�����������������
# count_ͳ�Ƶ�ǰĿ¼������Ŀ¼������ `.c` �� `.h` �ļ�������������������(ʹ�� grep -v '^\s*$$' ���˵����У�Ȼ��ʹ�� wc -l ͳ��ʣ������)

LIB_COMMON_DIR := lib-common  # ������ͨ��Ŀ¼·��
LIBC_INC_DIR := $(LIB_COMMON_DIR)/uclibc/include # ���� uclibc ��ͷ�ļ�·��
LIBC_LIB_DIR := $(LIB_COMMON_DIR)/uclibc/lib  # ���� uclibc �Ŀ��ļ�·��
LIBC := $(LIBC_LIB_DIR)/libc.a  # ���� uclibc �Ŀ��ļ�

#FLOAT := obj/$(LIB_COMMON_DIR)/FLOAT/FLOAT.a
# ���帡������⣨Ŀǰ��ע�͵��ˣ�

include config/Makefile.git
include config/Makefile.build
# ������������ Makefile �ļ�����Щ�ļ����Զ�����Ŀ�� git ���ú͹�����ص�ͨ�ù���

all: nemu
# Ĭ��Ŀ���� `nemu`�����û�ֱ��ִ�� `make` ʱ����ִ�����Ŀ��

##### rules for building the project #####

include nemu/Makefile.part
include testcase/Makefile.part
include lib-common/FLOAT/Makefile.part
include kernel/Makefile.part
include game/Makefile.part

nemu: $(nemu_BIN)  # ���幹�� `nemu` �Ĺ��������ڱ��� `$(nemu_BIN)`��`$(nemu_BIN)` Ӧ������ `nemu/Makefile.part` �ж����
testcase: $(testcase_BIN)  # ���幹�� `testcase` �Ĺ��������ڱ��� `$(testcase_BIN)`
kernel: $(kernel_BIN)  # ���幹�� `kernel` �Ĺ��������ڱ��� `$(kernel_BIN)`
game: $(game_BIN)  # ���幹�� `game` �Ĺ��������ڱ��� `$(game_BIN)`


##### rules for cleaning the project #####

clean-nemu:  # ɾ�� nemu ��Ӧ�Ĺ������Ŀ¼
	-rm -rf obj/nemu 2> /dev/null

clean-testcase:  # ɾ�� nemu ��Ӧ�Ĺ������Ŀ¼
	-rm -rf obj/testcase 2> /dev/null

clean-kernel:  # ɾ�� kernel ��Ӧ�Ĺ������Ŀ¼
	-rm -rf obj/kernel 2> /dev/null

clean-game:  # ɾ�� game ��Ӧ�Ĺ������Ŀ¼
	-rm -rf obj/game 2> /dev/null

clean: clean-cpp
	-rm -rf obj 2> /dev/null
	-rm -f *log.txt entry $(FLOAT) 2> /dev/null
# `clean` Ŀ����� `clean-cpp`�����Ŀ��Ӧ���ڰ����� Makefile �ж��壩��Ȼ��ɾ�� obj Ŀ¼��
# ɾ����־�ļ��� entry �ļ����Լ�����⣨��������� FLOAT��

##### some convinient rules #####

USERPROG := obj/testcase/mov  # �����û�����·��
# USERPROG := obj/testcase/quadratic-eq
ENTRY := $(USERPROG)  # �� ENTRY ����Ϊ�û�����·��
# ENTRY := $(kernel_BIN)

entry: $(ENTRY)  # `entry` Ŀ�꣺ʹ�� `objcopy`
	objcopy -S -O binary $(ENTRY) entry 

run: $(nemu_BIN) $(USERPROG) entry
	$(call git_commit, "run")
	$(nemu_BIN) $(USERPROG)

gdb: $(nemu_BIN) $(USERPROG) entry
	$(call git_commit, "gdb")
	gdb -s $(nemu_BIN) --args $(nemu_BIN) $(USERPROG)

test: $(nemu_BIN) $(testcase_BIN) entry
	$(call git_commit, "test")
	bash test.sh $(testcase_BIN)

submit: clean
	cd .. && zip -r $(STU_ID).zip $(shell pwd | grep -o '[^/]*$$')

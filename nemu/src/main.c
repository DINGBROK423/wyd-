void init_monitor(int, char *[]);
void reg_test();
void restart();
void ui_mainloop();

int main(int argc, char *argv[]) {

	/* Initialize the monitor. */
	init_monitor(argc, argv);  //������ʼ��,��log�ļ��������ַ����ͷ��ű�����������ʽ����ʼ�����ӵ�ṹ��

	/* Test the implementation of the `CPU_state' structure. */
	reg_test();//���ԼĴ����ṹ

	/* Initialize the virtual computer system. */
	restart();//���������  0x100000λ�ü��س��� load_entry

	/* Receive commands from user. */
	ui_mainloop();//ģ������ѭ�� ����c����ָ��ִ����ѭ�� cpu_exec()

	return 0;
}
// ָ��ǰ׺Ĭ��ֻ��Operand-Size ���� �������һ����ʮ������66   ��ʾָ��������ĳ� ����֤����������16λ�������ڿ�����8��32λ           
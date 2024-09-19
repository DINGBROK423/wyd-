#include "cpu/exec/helper.h"
//ģ�⴦����ָ��ִ���еġ����á���CALL��ָ��


//��������������һ����ʽ��CALLָ�����Ŀ���ַ��ͨ��ĳ����������Immediate Value�������������Short Immediate����ʽָ���ġ�
make_helper(call_si) {
	int len = decode_si_l(eip + 1);  //�ӵ�ǰָ��ָ�루eip��֮��ĵ�ַ�����Ŀ���ַ�ĳ���
	swaddr_t ret_addr = cpu.eip + len + 1;  //���㷵�ص�ַ������ǰָ��ִ����Ϻ�Ӧ�÷��صĵ�ַ������ͨ������ǰָ��ָ�루eip������Ŀ���ַ�ĳ��Ⱥ�1����ǰָ��ĳ��ȣ���ΪCALLָ������Ҳռ��һ����ַ����ʵ�ֵġ�
	swaddr_write(cpu.esp - 4, 4, ret_addr);  //�����ص�ַѹ��ջ�У�cpu.esp - 4����������ջָ�루cpu.esp��
	cpu.esp -= 4;  
	cpu.eip += op_src->val;  //cpu.eip����ΪĿ���ַ��ͨ��op_src->val��ȡ
	print_asm("call %x", cpu.eip + 1 + len);

	return len + 1;  //����ָ��ĳ��ȼ�1���Ա�ģ��ִ����һ��ָ�
}

//����������������һ����ʽ��CALLָ�����Ŀ���ַ��ͨ���Ĵ������ڴ��ַ��Register or Memory�����ָ����
make_helper(call_rm) {
	int len = decode_rm_l(eip + 1);  //ͬ��
	swaddr_t ret_addr = cpu.eip + len + 1;  //ͬ��
	swaddr_write(cpu.esp - 4, 4, ret_addr);  //ͬ��
	cpu.esp -= 4;  //ͬ��
	cpu.eip = op_src->val - (len + 1);  //����Ĳ�֮ͬ�����ڣ�Ŀ���ַ����ֱ�Ӹ����ģ����Ǵ洢��ĳ���Ĵ������ڴ��ַ�С���ˣ�cpu.eip������Ϊop_src->val - (len + 1)
	print_asm("c all *%s", op_src->str);

	return len + 1;  //ͬ��
}



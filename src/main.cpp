#include <iostream>
#include <modbus.h>
#include "registers.h"

using namespace std;

int main()
{
    setlocale(LC_ALL, "RUS");

	modbus_t* ctx;
	ctx = modbus_new_rtu("\COM2", 19200, 'N', 8, 1);
    if (ctx == NULL) {
        std::cout << "�� ������� ������� �������� Modbus" << std::endl;
        return 1;
    }

    // ��������� ������ ���������� 
    modbus_set_slave(ctx, 1);

    // ��������� �� RS-485
    modbus_rtu_set_serial_mode(ctx, MODBUS_RTU_RS485);

    // ����������� � ���������� 
    if (modbus_connect(ctx) == -1) {
        std::cout << "�� ������� ������������ � ����������" << std::endl;
        modbus_free(ctx);
        return 1;
    }
    else cout << "�������� �����������\n";
   


    int reg_count = 2;
    int buffer_size = 2;
                                            // ������ ������ �� ��������� 
    uint16_t regs[10];                      // ��� �������� ������ �� 2-� ��������� 
    int k = 0;
  
        int rc = modbus_read_registers(
            ctx,
            REACTIVE_POWER_TOTAL_FLOAT,                               // ������ ������� ��������
            reg_count,                                  // ���������� ���������
            regs);
        cout << rc << endl;
        if (rc == -1) {
            std::cout << "������ ������ ���������" << std::endl;
            // cout << modbus_strerror(errno) << endl;
            modbus_close(ctx);
            modbus_free(ctx);
            return 1;
        }

         float reg_content = modbus_get_float(regs);
         //float reg_content = modbus_get_float_dcba(regs);

         cout << "�������� �������� " << reg_content << endl;
         //����� ������ 
       /* for (int i = 0; i < buffer_size; ++i) {
            std::cout << "�������� �������� " << i << ": " << regs[i] << std::endl;
        }*/
       
  
    // �������� ���������� � ������������ ��������� 
    modbus_close(ctx);
    modbus_free(ctx);

	return 0;
}
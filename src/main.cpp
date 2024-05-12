

#include <iostream>
#include <modbus.h>
#include "registers.h"
#include "data_logger.h"



using namespace std;

int main()
{
    setlocale(LC_ALL, "RUS");



 //   modbus_t* ctx;
	//ctx = modbus_new_rtu("\COM2", 19200, 'N', 8, 1);
 //   if (ctx == NULL) {
 //       std::cout << "�� ������� ������� �������� Modbus" << std::endl;
 //       return 1;
 //   }

 //   // ��������� ������ ���������� 
 //   modbus_set_slave(ctx, 1);

 //   // ��������� �� RS-485
 //   modbus_rtu_set_serial_mode(ctx, MODBUS_RTU_RS485);

 //   // ����������� � ���������� 
 //   if (modbus_connect(ctx) == -1) {
 //       std::cout << "�� ������� ������������ � ����������" << std::endl;
 //       modbus_free(ctx);
 //       return 1;
 //   }
 //   else cout << "�������� �����������\n";
   
    Data_logger data_logger;
    data_logger.init();


    data_logger.read_data();
    data_logger.write_data();
    data_logger.print_setup();


    int reg_count = 1;
    int buffer_size = 1;
                                            // ������ ������ �� ��������� 
    uint16_t regs[10];                      // ��� �������� ������ �� 2-� ��������� 
   
  
    //// 1 data update rate sec
    //    int rc = modbus_read_registers(
    //        ctx,
    //        REAL_POWER_TOTAL_INT,                               // ������ ������� ��������
    //        reg_count,                                  // ���������� ���������
    //        regs);
    //    cout << rc << endl;

    //    if (rc == -1) {
    //        std::cout << "������ ������ ���������" << std::endl;
    //        // cout << modbus_strerror(errno) << endl;
    //        modbus_close(ctx);
    //        modbus_free(ctx);
    //        return 1;
    //    }

    //     //float reg_content = modbus_get_float(regs);
    //     //float reg_content = modbus_get_float_dcba(regs);

    //     //cout << "�������� �������� " << reg_content << endl;
    //     //����� ������ 
    //    for (int i = 0; i < buffer_size; ++i) {
    //        std::cout << "�������� �������� " << i << ": " << regs[i] << std::endl;
    //    }
    //   
  
    //// �������� ���������� � ������������ ��������� 
    //modbus_close(ctx);
    //modbus_free(ctx);

	return 0;
}
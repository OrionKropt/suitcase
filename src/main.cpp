

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

   // data_logger.write_data();
    if (data_logger.read_data())
    {
        return 0;
    }
    data_logger.write_data();
  // data_logger.print_setup();



	return 0;
}
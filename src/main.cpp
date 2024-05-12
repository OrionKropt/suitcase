

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
 //       std::cout << "Не удалось создать контекст Modbus" << std::endl;
 //       return 1;
 //   }

 //   // Установка адреса устройства 
 //   modbus_set_slave(ctx, 1);

 //   // Установка на RS-485
 //   modbus_rtu_set_serial_mode(ctx, MODBUS_RTU_RS485);

 //   // Подключение к устройству 
 //   if (modbus_connect(ctx) == -1) {
 //       std::cout << "Не удалось подключиться к устройству" << std::endl;
 //       modbus_free(ctx);
 //       return 1;
 //   }
 //   else cout << "Успешное подключение\n";
   
    Data_logger data_logger;
    data_logger.init();


    data_logger.read_data();
    data_logger.write_data();
    data_logger.print_setup();


    int reg_count = 1;
    int buffer_size = 1;
                                            // Чтение данных из регистров 
    uint16_t regs[10];                      // Для хранения данных из 2-х регистров 
   
  
    //// 1 data update rate sec
    //    int rc = modbus_read_registers(
    //        ctx,
    //        REAL_POWER_TOTAL_INT,                               // Адресс первого регистра
    //        reg_count,                                  // Количестов регистров
    //        regs);
    //    cout << rc << endl;

    //    if (rc == -1) {
    //        std::cout << "Ошибка чтения регистров" << std::endl;
    //        // cout << modbus_strerror(errno) << endl;
    //        modbus_close(ctx);
    //        modbus_free(ctx);
    //        return 1;
    //    }

    //     //float reg_content = modbus_get_float(regs);
    //     //float reg_content = modbus_get_float_dcba(regs);

    //     //cout << "Значение регистра " << reg_content << endl;
    //     //Вывод данных 
    //    for (int i = 0; i < buffer_size; ++i) {
    //        std::cout << "Значение регистра " << i << ": " << regs[i] << std::endl;
    //    }
    //   
  
    //// Закрытие соединения и освобождение контекста 
    //modbus_close(ctx);
    //modbus_free(ctx);

	return 0;
}